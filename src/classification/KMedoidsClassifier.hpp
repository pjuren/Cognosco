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

#ifndef KMEDOIDS_CLASSIFIER_HPP_
#define KMEDOIDS_CLASSIFIER_HPP_

#include "Dataset.hpp"
#include "NaiveBayes.hpp"

namespace Classifiers {
  class KMedoids {
  public:
    // constructors

    // public inspectors
    double membership_probability(const Instance &test_instance,
                                  const std::string &class_label) const;
    double posterior_probability(const Instance &test_instance,
                                 const std::string &class_label) const;
    std::string to_string() const;

    // public mutators
    void learn(const Dataset &training_instances,
               const std::string &class_label,
               const std::set<size_t> &ignore_instance_ids = std::set<size_t>());
  private:
    std::string learned_class;
    NaiveBayes nb_classifier;
    std::vector<std::string> medoid_names;
  };
}

#endif
