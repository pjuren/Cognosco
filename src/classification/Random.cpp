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
#include "Random.hpp"

// bring these into the local namespace
using std::set;
using std::pair;
using std::string;
using std::unordered_map;

/*****************************************************************************
 *                               INSPECTORS                                  *
 *****************************************************************************/

double
Classifiers::Random::class_probability(const Instance &test_instance,
                                       const std::string &class_label,
                                       const set<string> &exclude_atts) const {
  return this->prob;
}

string
Classifiers::Random::to_string() const {
  if (this->learned_class.empty()) return "[NULL RANDOM CLASSIFIER]";
  std::stringstream ss;
  ss << "prob = " << prob;
  return ss.str();
}

string
Classifiers::Random::usage() const {
  return "Classifiers::Random has no specific options";
}


/*****************************************************************************
 *                                MUTATORS                                   *
 *****************************************************************************/

void
Classifiers::Random::learn(const Dataset &train_insts,
                           const string &class_label,
                           const set<size_t> &ignore_inst_ids,
                           const set<string> &ig_atts) {
  set<string> class_labels;
  for (auto inst = train_insts.begin(); inst != train_insts.end(); ++inst) {
    if (ignore_inst_ids.find(inst->get_instance_id()) != ignore_inst_ids.end()) {
      continue;
    }

    const string &instance_class_label =\
      inst->get_att_occurrence(class_label)->to_string();
    class_labels.insert(instance_class_label);
  }
  this->prob = 1.0 / class_labels.size();
  this->learned_class = class_label;
}

void
Classifiers::Random::set_classifier_specific_options(Commandline &cmdline) {
  // No specific options...
}

void
Classifiers::Random::clear() {
  this->learned_class = "";
}
