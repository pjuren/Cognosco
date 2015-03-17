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
#include <cassert>
#include <sstream>
#include <set>

// local Cognosco includes
#include "DecisionStump.hpp"

// bring these into the local namespace
using std::string;
using std::vector;
using std::set;

/*****************************************************************************
 *                               INSPECTORS                                  *
 *****************************************************************************/

string
Classifiers::DecisionStump::to_string() const {
  if (this->learned_class.empty() || this->rule == NULL)
    return "[UNTRAINED DecisionStump CLASSIFIER]";

  return this->rule->to_string();
}

std::string
Classifiers::DecisionStump::usage() const {
  return "DecisionStump specific options";
}

double
Classifiers::DecisionStump::class_probability(const Instance &test_instance,
                                              const string &class_label,
                                              const set<string> &exclude_atts) const {
  if (this->rule == NULL) {
    throw DecisionStumpError("not trained");
  }
  if (this->rule->get_predicted_label() == class_label)
    return this->rule->get_prob(test_instance);
  else
    return 1 - this->rule->get_prob(test_instance);
}

double
Classifiers::DecisionStump::get_cost(const Dataset &d,
                                     const std::set<size_t> &ig_instance_ids) const {
  double cost = 0;
  for (auto inst_it = d.begin(); inst_it != d.end(); ++inst_it) {
    size_t inst_id = inst_it->get_instance_id();
    if (ig_instance_ids.find(inst_id) != ig_instance_ids.end()) continue;
    string actual_class((*inst_it)[this->rule->get_class_attribute_name()]->to_string());
    string predicted_label(this->rule->get_predicted_label());
    string other_label(this->rule->get_other_label());
    double pr_pos_class = this->rule->get_prob(*inst_it);

    auto act_pred_pair = std::make_pair(actual_class, predicted_label);
    auto act_other_pair = std::make_pair(actual_class, other_label);
    cost += (pr_pos_class * cost_matrix[act_pred_pair]);
    cost += ((1-pr_pos_class) * cost_matrix[act_other_pair]);
  }
  return cost;
}


/*****************************************************************************
 *                                MUTATORS                                   *
 *****************************************************************************/

void
Classifiers::DecisionStump::learn(const Dataset &train_insts,
                                  const string &class_label,
                                  const set<size_t> &ignore_inst_ids,
                                  const set<string> &ig_atts) {
  const bool DEBUG = false;
  this->clear();

  set<string> class_labels;
  string pos_class_lab = "";
  string neg_class_lab = "";
  for (auto inst_it = train_insts.begin(); inst_it != train_insts.end(); ++inst_it) {
    string val = (*inst_it)[class_label]->to_string();
    if (DEBUG)
      std::cerr << "examining instance with class " << val << std::endl;
    class_labels.insert(val);
    if (pos_class_lab.empty()) {
      if (DEBUG)
        std::cerr << "selected " << val << " as positive class" << std::endl;
      pos_class_lab = val;
    } else if (neg_class_lab.empty() && val != pos_class_lab) {
      if (DEBUG)
        std::cerr << "selected " << val << " as negative class" << std::endl;
      neg_class_lab = val;
    }
  }
  assert(pos_class_lab != neg_class_lab);

  if (class_labels.size() != 2) {
    std::stringstream ss;
    ss << "expected two classes, found " << class_labels.size();
    throw DecisionStumpError(ss.str());
  }

  BinaryDecisionRule *candidate;
  for (auto at_it = train_insts.begin_attributes(); at_it != train_insts.end_attributes(); ++at_it) {
    Attribute* at_ptr (*at_it);
    string att_name (at_ptr->get_name());

    if (att_name == class_label || (ig_atts.find(att_name) != ig_atts.end())) continue;

    for (auto inst_it = train_insts.begin(); inst_it != train_insts.end(); ++inst_it) {
      size_t inst_id (inst_it->get_instance_id());
      if (ignore_inst_ids.find(inst_id) != ignore_inst_ids.end()) continue;

      // nasty hack to make double
      const AttributeOccurrence* att_occ_ptr((*inst_it)[att_name]);
      double at_value ((*att_occ_ptr) * 1.0);

      // if att_name > thresh --> pos_class
      candidate = new BinaryDecisionRule(class_label, pos_class_lab,
                                         neg_class_lab, att_name, at_value);
      if (this->rule == NULL) {
        this->rule = candidate;
      } else {
        double old_cost = this->get_cost(train_insts);
        std::swap(this->rule, candidate);
        if (this->get_cost(train_insts) >= old_cost) std::swap(this->rule, candidate);
        delete candidate;
      }

      // if att_name > thresh --> neg_class
      candidate = new BinaryDecisionRule(class_label, neg_class_lab,
                                         pos_class_lab, att_name, at_value);
      double old_cost = this->get_cost(train_insts);
      std::swap(this->rule, candidate);
      if (this->get_cost(train_insts) >= old_cost) std::swap(this->rule, candidate);
      delete candidate;
    }
  }
  this->learned_class = class_label;
}

void
Classifiers::DecisionStump::set_classifier_specific_options(Commandline &cmdline) {
  // no classifier specific options
}

void
Classifiers::DecisionStump::clear() {
  if (this->rule != NULL) {
    delete this->rule;
    this->rule = NULL;
  }
  this->learned_class = "";
}
