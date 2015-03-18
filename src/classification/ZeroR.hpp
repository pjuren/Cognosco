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

#ifndef ZERO_R_CLASSFR_HPP_
#define ZERO_R_CLASSFR_HPP_

// stl includes
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

// local Cognosco includes
#include "CLI.hpp"
#include "Classifier.hpp"
#include "Dataset.hpp"
#include "CognoscoError.hpp"
#include "StringUtils.hpp"

namespace Classifiers {
  class ZeroR : public Classifier {
  public:
    // constructors
    using Classifier::Classifier;

    // public inspectors
    std::string to_string() const;

    double class_probability(const Instance &test_instance,
                             const std::string &class_label,
                             const std::set<std::string> &exclude_atts =\
                               std::set<std::string>()) const;
    std::string usage() const;

    // public mutators
    void learn(const Dataset &training_instances,
               const std::string &class_label,
               const std::set<size_t> &ignore_instance_ids = std::set<size_t>(),
               const std::set<std::string> &ig_atts = std::set<std::string>());
    void set_classifier_specific_options(Commandline &cmdline);
    void clear();

  private:
    std::unordered_map<std::string, double> priors;
  };
}

#endif
