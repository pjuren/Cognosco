/* The following applies to this software package and all subparts therein
 *
 * Cognosco Copyright (C) 2015 Philip J. Uren
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

// stl includes
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cmath>

// local Cognosco includes
#include "NaiveBayes.hpp"

// bring these into the local namespace
using std::pair;
using std::string;
using std::unordered_map;

/*****************************************************************************
 *                               INSPECTORS                                  *
 *****************************************************************************/

/**
 * \brief Get the prior probability of the given class label. Classifier must
 *        have already been trained, otherwise these priors have not been
 *        computed.
 */
double
NaiveBayes::get_prior_prob(const string &class_label) const {
  unordered_map<string, double>::const_iterator it =\
    this->class_priors.find(class_label);
  if (it == this->class_priors.end()) {
    std::stringstream ss;
    ss << "Failed to get prior probability for class "
       << class_label << "; no such class";
    throw NaiveBayesError(ss.str());
  }
  return it->second;
}

double
NaiveBayes::get_mean(const string &class_name, const string &att_name) const {
  pair<string, string> cls_att_pair = std::make_pair(class_name, att_name);
  AttClassMap::const_iterator m_it = this->class_means.find(cls_att_pair);
  if (m_it == this->class_means.end()) {
    throw NaiveBayesError("unknown class and attribute pair: " + class_name
                          + ", " + att_name);
  }
  return m_it->second;
}


double
NaiveBayes::get_variance(const string &class_name, const string &att_name) const {
  pair<string, string> cls_att_pair = std::make_pair(class_name, att_name);
  AttClassMap::const_iterator v_it = this->class_variances.find(cls_att_pair);
  if (v_it == this->class_variances.end()) {
    throw NaiveBayesError("unknown class and attribute pair: " + class_name
                          + ", " + att_name);
  }
  return v_it->second;
}

/**
 * \brief Get the conditional probability of the given attribute value given
 *        a particular class; we assume all attribute are numeric and follow
 *        a Gaussian distribution -- TODO remove this assumption later some
 *        time. Classifier must have already been trained, otherwise the
 *        distribution parameters have not been learned.
 */
double
NaiveBayes::get_conditional_prob(const AttributeOccurrence *value,
                                 const string &class_name) const {
  const string &att_name (value->get_attribute_name());
  pair<string, string> cls_att_pair = std::make_pair(class_name, att_name);
  AttClassMap::const_iterator v_it = this->class_variances.find(cls_att_pair);
  AttClassMap::const_iterator m_it = this->class_means.find(cls_att_pair);
  if ((v_it == this->class_variances.end()) ||
      (m_it == this->class_means.end())) {
    throw NaiveBayesError("unknown class and attribute pair: " + class_name
                          + ", " + att_name);
  }
  const double mu(m_it->second);
  const double var(v_it->second);
  const double val((*value) * 1.0);
  const double exponent((val-mu) * (val-mu) / (2*var));
  return exp(-exponent) / sqrt(2 * M_PI * var);
}


/**
 * \brief compute the probability that a given Instance belongs to a given
 *        class using this classifier.
 */
double
NaiveBayes::posterior_probability(const Instance &test_instance,
                                   const string &class_label) const {
  double res = 1;
  for (Instance::const_iterator it = test_instance.begin();
       it != test_instance.end(); ++it) {
    if ((*it)->get_attribute_name() == this->learned_class) continue;
    res *= this->get_conditional_prob(*it, class_label);
  }
  return res * this->get_prior_prob(class_label);
}

double
NaiveBayes::membership_probability(const Instance &test_instance,
                                   const string &class_label) const {
  double sum = 0;
  std::vector<string> class_labels;
  for (auto p : this->class_priors) class_labels.push_back(p.first);
  for (size_t i = 0; i < class_labels.size(); ++i)
    sum += this->posterior_probability(test_instance, class_labels[i]);
  return this->posterior_probability(test_instance, class_label) / sum;
}


string
NaiveBayes::to_string() const {
  if (this->learned_class.empty()) return "[NULL NB CLASSIFIER]";

  std::stringstream ss;

  std::vector<string> class_labels;
  for (auto p : this->class_priors) class_labels.push_back(p.first);
  for (size_t i = 0; i < class_labels.size(); ++i) {
    ss << "[" << class_labels[i] << "]" << std::endl;
    ss << "prior: " << this->get_prior_prob(class_labels[i]) << std::endl;
    for (auto cm : this->class_means) {
      if (cm.first.first != class_labels[i]) continue;
      ss << "mean " << cm.first.second << ": " << cm.second << "; ";
      ss << "variance " << cm.first.second << ": "
         << this->get_variance(cm.first.first, cm.first.second) << std::endl;
    }
  }
  return ss.str();
}

/*****************************************************************************
 *                                MUTATORS                                   *
 *****************************************************************************/

void
NaiveBayes::learn(const Dataset &training_instances,
                  const string &class_label,
                  const std::set<size_t> &ignore_inst_ids) {
  // for computing class prior probabilities
  unordered_map<string, double> class_counts;

  // for computing mean and variance
  typedef unordered_map<pair<string, string>, RunningStat, string_pair_hash> ClsAttMap;
  ClsAttMap running_stats;

  size_t skipped = 0;
  for (Dataset::const_iterator inst = training_instances.begin();
       inst != training_instances.end(); ++inst) {
    if (ignore_inst_ids.find(inst->get_instance_id()) != ignore_inst_ids.end()) {
      skipped += 1;
      continue;
    }
    const string &instance_class_label =\
      inst->get_att_occurrence(class_label)->to_string();
    class_counts[instance_class_label] += 1;

    for (Instance::const_iterator it = inst->begin(); it != inst->end(); ++it) {
      const string& attribute_name = (*it)->get_attribute_name();
      if (attribute_name == class_label) continue;
      const double att_value = (**it) * 1.0;
      std::pair<string, string> att_class_pair =\
        std::make_pair(instance_class_label, attribute_name);
      running_stats[att_class_pair].push(att_value);
    }
  }

  for (ClsAttMap::const_iterator it = running_stats.begin();
       it != running_stats.end(); ++it) {
    const pair<string, string> class_attr_pair(it->first);
    this->class_means[class_attr_pair] = it->second.mean();
    this->class_variances[class_attr_pair] = it->second.variance();
  }

  const size_t effective_size = training_instances.size() - skipped;
  for (unordered_map<string,double>::iterator it = class_counts.begin();
       it != class_counts.end(); ++it) {
    this->class_priors[it->first] = it->second / effective_size;
  }

  this->learned_class = class_label;
}
