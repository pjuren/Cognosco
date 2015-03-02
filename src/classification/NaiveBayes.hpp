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

#ifndef NAIVE_BAYES_HPP_
#define NAIVE_BAYES_HPP_

#include "Instance.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class NaiveBayes {
public:
  // cosntructors

  // public inspectors

  // public mutators
  void learn(vector<Instance> training_instances);
private:
  unordered_map<std::string, double> class_priors;
  unordered_map<std::pair<std::string, std::string>, double> class_variances;
  unordered_map<std::pair<std::string, std::string>, double> class_means;
};

#endif
