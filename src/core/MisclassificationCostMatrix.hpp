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

#ifndef MISCLASS_COST_HPP_
#define MISCLASS_COST_HPP_

// stl includes
#include <string>
#include <unordered_map>
#include <sstream>

// local includes
#include "CognoscoError.hpp"
#include "StringUtils.hpp"

class MisclassificationCostMatrix {
public:
  // constructors
  MisclassificationCostMatrix() :
    map (std::unordered_map<std::pair<std::string, std::string>,
                            double, string_pair_hash>()),
    degenerate(true) {}
  MisclassificationCostMatrix(const std::string &str_rep) :degenerate(false) {
    // good:poor:4 poor:good:1 poor:poor:0 good:good:1
    std::vector<std::string> parts;
    tokenize(str_rep, parts);
    for (std::string entry : parts) {
      std::vector<std::string> peices;
      tokenize(entry, peices, ":");
      if (peices.size() != 3) {
        throw CognoscoError("oops");
      }
      map[std::make_pair(peices[0], peices[1])] = std::stof(peices[2]);
    }

  }

  const double& operator[](const std::pair<std::string, std::string> k) const {
    if (this->degenerate) {
      if (k.first == k.second) return DEFAULT_MATCH_COST;
      else return DEFAULT_MISMATCH_COST;
    }
    auto misclass_it = map.find(k);
    if (misclass_it == map.end()) {
      std::stringstream ss;
      ss << "no such actual/predicted class pair in missclassification "
            "cost matrix; actual class: " << k.first
            << " predicted class " << k.second;
      throw CognoscoError(ss.str());
    }
    return misclass_it->second;
  }
private:
  std::unordered_map<std::pair<std::string, std::string>, double,
                     string_pair_hash> map;
  bool degenerate;

  static const double DEFAULT_MATCH_COST;
  static const double DEFAULT_MISMATCH_COST;
};

#endif
