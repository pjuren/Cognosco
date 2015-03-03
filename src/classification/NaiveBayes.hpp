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

class NaiveBayesError : public ClassifierError {};

class NaiveBayes {
public:
  // cosntructors

  // public inspectors
  double membership_probability(const Instance &test_instance,
                                const std::string &class_label) const;

  // public mutators
  void learn(vector<Instance> training_instances);
private:
  unordered_map<std::string, double> class_priors;
  unordered_map<std::pair<std::string, std::string>, double> class_variances;
  unordered_map<std::pair<std::string, std::string>, double> class_means;
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
  double standardDeviation() const { return sqrt(Variance()); }

private:
  int m_n;
  double m_oldM, m_newM, m_oldS, m_newS;
};


#endif
