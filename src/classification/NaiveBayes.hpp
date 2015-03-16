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

/******************************************************************************
 *                                   TYPES                                    *
 ******************************************************************************/

typedef std::unordered_map<std::pair<std::string, std::string>,
                           double, string_pair_hash> AttClassMap;


/*****************************************************************************
 *                                ERROR-HANDLING                             *
 *****************************************************************************/

class NaiveBayesError : public CognoscoError {
public:
  NaiveBayesError(const std::string msg) : CognoscoError(msg) {};
};


/*****************************************************************************
 *                                THE CLASSIFIER                             *
 *****************************************************************************/

class NaiveBayes : public Classifier {
public:
  // constructors
  using Classifier::Classifier;

  // public inspectors
  double membership_probability(const Instance &test_instance,
                                const std::string &class_label,
                                const std::set<std::string> &exclude_atts =\
                                  std::set<std::string>()) const;
  double posterior_probability(const Instance &test_instance,
                               const std::string &class_label,
                               const std::set<std::string> &exclude_atts =\
                                 std::set<std::string>()) const;
  double get_prior_prob(const std::string &class_label) const;
  double get_conditional_prob(const AttributeOccurrence *value,
                              const std::string &class_label) const;
  std::string to_string() const;
  double get_variance(const std::string &class_name,
                      const std::string &att_name) const;
  double get_mean(const std::string &class_name,
                  const std::string &att_name) const;
  double class_probability(const Instance &test_instance,
                           const std::string &class_label,
                           const std::set<std::string> &exclude_atts =\
                             std::set<std::string>()) const {
    return this->membership_probability(test_instance, class_label,
                                        exclude_atts);
  }
  std::string usage() const;

  // public mutators
  void learn(const Dataset &training_instances,
             const std::string &class_label,
             const std::set<size_t> &ignore_instance_ids = std::set<size_t>(),
             const std::set<std::string> &ig_atts = std::set<std::string>());
  void set_classifier_specific_options(Commandline &cmdline);
  void clear();


private:
  std::unordered_map<std::string, double> class_priors;
  AttClassMap class_variances;
  AttClassMap class_means;
};

/**
 * Adapted from code by John D. Cook; an implementation of Knuth's algorithm
 * for computing running variance and mean.
 * http://www.johndcook.com/blog/standard_deviation/
 */
class RunningStat {
public:
  RunningStat() : m_n(0) {}

  void clear() {
    m_n = 0;
  }

  void push(const double x) {
    m_n++;

    // See Knuth TAOCP vol 2, 3rd edition, page 232
    if (m_n == 1) {
      m_oldM = m_newM = x;
      m_oldS = 0.0;
    }
    else {
      m_newM = m_oldM + (x - m_oldM)/m_n;
      m_newS = m_oldS + (x - m_oldM)*(x - m_newM);

      // set up for next iteration
      m_oldM = m_newM;
      m_oldS = m_newS;
    }
  }

  int numDataValues() const { return m_n;}
  double mean() const { return (m_n > 0) ? m_newM : 0.0; }
  double variance() const { return ( (m_n > 1) ? m_newS/(m_n - 1) : 0.0 ); }
  double standardDeviation() const { return sqrt(variance()); }

private:
  int m_n;
  double m_oldM, m_newM, m_oldS, m_newS;
};


#endif
