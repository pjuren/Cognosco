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

#include "NaiveBayes.hpp"

#include <string>
#include <vector>
#include <unordered_map>

NaiveBayes::learn(const vector<Instance> &training_instances,
                  const string &class_label) {
  // for computing class prior probabilities
  unordered_map<string, double> class_counts;

  // for computing mean and variance
  typedef unordered_map<pair<string, string>, RunningStat> ClsAttMap;
  ClsAttMap running_stats;

  for (size_t i = 0; i < training_instances.size(); ++i) {
    const &string instance_class_label =\
      training_instances[i].get_attribute_value(class_label);
    class_counts[instance_class_label] += 1;

    for (Instance::const_iterator it = training_instances[i].begin();
         it != training_instances[i].end(); ++it) {
      const string& attribute_name = it->get_name();
      const double att_value = it->get_attribute_value_numeric();
      std::pair<string, string> att_class_pair =\
        std::make_pair(instance_class_label, attribute_name)
      running_stats[att_class_pair].push(att_value);
    }
  }

  for (ClsAtMap::const_iterator it = running_stats.begin();
       it != running_stats.end(); ++it) {
    const pair<string, string> class_attr_pair(it->first);
    this->class_means[class_attr_pair] = it->second.mean();
    this->class_variances[class_attr_pair] = it->second.variance();
  }

  for (unordered_map<string,double>::iterator it = class_counts.begin();
       it != class_counts.end(); ++it) {
    this->class_priors[it->first] = it->second / training_instances.size();
  }
}

void
NaiveBayes::membership_probability(const Instance &test_isntance,
                                   const std::string class_label) const {
  double res = 1;
  for (Instance::const_iterator it = training_instance.begin();
       it != training_instance[i].end(); ++it) {
    res *= this->get_conditional_prob(it->get_name(), class_label,
                                      it->get_attribute_value_numeric());
  }
  return res * this->get_prior_prob(class_label);
}

void
NaiveBayes::get_conditional_prob(const string &att_name,
                                 const string &class_name,
                                 const double attribute_value) const {
  typedef unordered_map<pair<string, string>, RunningStat> ClsAttMap;
  pair<string, string> cls_att_pair = std::make_pair(class_name, att_name);
  ClsAttMap::const_iterator v_it = this->class_variances.find(cls_att_pair);
  ClsAttMap::const_iterator m_it = this->class_variances.find(cls_att_pair);
  if ((v_it == this->class_variances.end()) || (m_it == this->class_means.end())) {
    throw NaiveBayesError("unknown class and attribute pair: " + class_name
                          + ", " + att_name);
  }
}
