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

#ifndef DECISION_STUMP_HPP_
#define DECISION_STUMP_HPP_

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
#include "MisclassificationCostMatrix.hpp"


/*****************************************************************************
 *                                    TYPES                                  *
 *****************************************************************************/
typedef std::unordered_map<std::pair<std::string, std::string>, double,
                           string_pair_hash>
        CostMatrix;

/*****************************************************************************
 *                                ERROR-HANDLING                             *
 *****************************************************************************/

class DecisionStumpError : public CognoscoError {
public:
  DecisionStumpError(const std::string msg) : CognoscoError(msg) {};
};


/*****************************************************************************
 *                                THE CLASSIFIER                             *
 *****************************************************************************/

namespace Classifiers {
  class BinaryDecisionRule {
  public:
    BinaryDecisionRule(const std::string &cls_name, const std::string &predicted_label,
                       const std::string &other_label, const std::string &att_name,
                       const double thresh) :
                    class_att_name (cls_name), predicted_label (predicted_label),
                    other_label(other_label), att_name(att_name), thresh(thresh) {};
    BinaryDecisionRule(const BinaryDecisionRule &bcr) :
      class_att_name(bcr.class_att_name), predicted_label(bcr.predicted_label),
      other_label(bcr.other_label),
      att_name(bcr.att_name), thresh(bcr.thresh) {};

    // public inspectors
    double get_prob(const Instance &inst) const {
      const AttributeOccurrence *att_oc_ptr = inst[att_name];
      // nasty hack tog et double...
      if (((*att_oc_ptr) * 1.0) > thresh) return 1.0;
      else return 0.0;
    }
    const std::string &get_class_attribute_name() const {
      return class_att_name;
    }
    const std::string &get_predicted_label() const {
      return predicted_label;
    }
    const std::string &get_other_label() const {
      return other_label;
    }
    std::string to_string() const {
      std::stringstream ss;
      ss << "if " << this->att_name << " > " << this->thresh
         << " --> " << class_att_name << " = " << this->predicted_label
         << " else --> " << class_att_name << " = " << this->other_label;
      return ss.str();
    }

  private:
    std::string class_att_name;
    std::string predicted_label;
    std::string other_label;
    std::string att_name;
    double thresh;
  };

  class DecisionStump : public Classifier {
  public:
    // constructors
    using Classifier::Classifier;
    DecisionStump() :
      rule(NULL), cost_matrix(MisclassificationCostMatrix()) {}
    DecisionStump(const DecisionStump &ds) :
      rule(new BinaryDecisionRule((*ds.rule))),
      cost_matrix(MisclassificationCostMatrix(ds.cost_matrix)) {};
    DecisionStump(const MisclassificationCostMatrix &m) :
      rule(NULL), cost_matrix(m) {}
    ~DecisionStump() { delete rule; }

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
    // private instance variables
    BinaryDecisionRule *rule;
    MisclassificationCostMatrix cost_matrix;

    // private inspectors
    double get_cost(const Dataset &d, const std::set<size_t> &ig_instance_ids =\
                    std::set<size_t>()) const;
  };
}

#endif
