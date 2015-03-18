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
#include <set>

// local Cognosco includes
#include "ZeroR.hpp"

// bring these into the local namespace
using std::set;
using std::pair;
using std::string;
using std::unordered_map;

/*****************************************************************************
 *                               INSPECTORS                                  *
 *****************************************************************************/

double
Classifiers::ZeroR::class_probability(const Instance &test_instance,
                                       const std::string &class_label,
                                       const set<string> &exclude_atts) const {
  auto it = this->priors.find(class_label);
  if (it == this->priors.end()) {
    std::stringstream ss;
    ss << "ZeroR: learned no such class --> " << class_label;
    throw CognoscoError(ss.str());
  }
  return it->second;
}

string
Classifiers::ZeroR::to_string() const {
  if (this->learned_class.empty()) return "[NULL ZeroR CLASSIFIER]";
  std::stringstream ss;
  for (auto kvp : this->priors) {
    ss << kvp.first << ":" << kvp.second << " ";
  }
  return ss.str();
}

string
Classifiers::ZeroR::usage() const {
  return "Classifiers::Random has no specific options";
}


/*****************************************************************************
 *                                MUTATORS                                   *
 *****************************************************************************/

void
Classifiers::ZeroR::learn(const Dataset &train_insts,
                           const string &class_label,
                           const set<size_t> &ignore_inst_ids,
                           const set<string> &ig_atts) {
  this->clear();

  std::unordered_map<std::string, double> counts;
  size_t num_insts = 0;
  for (auto inst = train_insts.begin(); inst != train_insts.end(); ++inst) {
    if (ignore_inst_ids.find(inst->get_instance_id()) != ignore_inst_ids.end()) {
      continue;
    }

    const string &instance_class_label =\
      inst->get_att_occurrence(class_label)->to_string();

    counts[instance_class_label] += 1;
    num_insts += 1;
  }

  for (auto kvp : counts) {
    this->priors[kvp.first] = kvp.second / num_insts;
  }

  this->learned_class = class_label;
}

void
Classifiers::ZeroR::set_classifier_specific_options(Commandline &cmdline) {
  // No specific options...
}

void
Classifiers::ZeroR::clear() {
  this->learned_class = "";
  this->priors.clear();
}
