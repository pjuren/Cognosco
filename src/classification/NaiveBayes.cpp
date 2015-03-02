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
  typename unordered_map<std::pair<std::string, std::string>, double> ClsAtMap;
  ClsAtMap ms, ss;

  for (size_t i = 0; i < training_instances.size(); ++i) {
    const &string instance_class_label =\
      training_instances[i].get_attribute_value(class_label);

    class_counts[instance_class_label] += 1;
    const size_t c_cls_cnt(class_counts[instance_class_label]);

    for (Instance::const_iterator it = training_instances[i].begin();
         it != training_instances[i].end(); ++it) {
      const string& attribute_name = it->get_name();
      const double att_value = it->get_attribute_value();
      std::pair<string, string> att_class_pair =\
        std::make_pair(instance_class_label, attribute_name)
      if (m1s.find(att_class_pair) == m1s.end()) {
        assert(ss.find(att_class_pair) == s1s.end());
        m1s[att_class_pair] = att_val;
        ss[att_class_pair] = 0;
      } else {
        assert(ss.find(att_class_pair) != s1s.end());
        const double prev_m(ms[att_class_pair]);
        ms[att_class_pair] = ms[att_class_pair] +\
                             ((att_val - ms[att_class_pair]) / c_cls_cnt);
        ss[att_class_pair] = ss[att_class_pair] +\
                             ((att_val - prev_m) *\
                              (att_val - ms[att_class_pair]));
      }
    }
  }

  for (ClsAtMap::iterator it = ms.begin(); it != ms.end(); ++it) {
    const pair<string, string> class_attr_pair(it->first);
    
  }
}
