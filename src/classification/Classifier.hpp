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

#ifndef CLASSIFIER_HPP_
#define CLASSIFIER_HPP_

#include "Instance.hpp"
#include "Dataset.hpp"
#include "CLI.hpp"

class Classifier {
public:
  // constructors and destructors
  Classifier() : learned_class ("") {};
  virtual ~Classifier() {};

  // public inspectors
  virtual double class_probability(const Instance &test_instance,
                                   const std::string &class_label) const = 0;
  virtual std::string to_string() const = 0;
  virtual std::string usage() const = 0;
  bool learned() { return this->learned_class.empty(); }

  // public mutators
  virtual void learn(const Dataset &training_instances,
                     const std::string &class_label,
                     const std::set<size_t> &ig_insts = std::set<size_t>()) = 0;
  virtual void set_classifier_specific_options(Commandline &cmd) = 0;
  virtual void clear() = 0;
protected:
  std::string learned_class;
};

#endif
