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
#include <sstream>
#include <iostream>

// local Cognosco includes
#include "Dataset.hpp"
#include "CognoscoError.hpp"

// bring these into the local namespace
using std::string;

void
Dataset::add_instance(const Instance &inst) {
  this->instances.push_back(inst);
}

void
Dataset::add_attribute(const Attribute &att_desc) {
  att_descr_ptrs.push_back(new Attribute(att_desc));
}

const Attribute*
Dataset::get_attribute_description_ptr(size_t k) const {
  if (k >= this->att_descr_ptrs.size()) {
    std::stringstream ss;
    ss << "Cannot get attribute description " << k << "; dataset only has "
       << this->att_descr_ptrs.size() << " attributes";
    throw CognoscoError(ss.str());
  }
  return this->att_descr_ptrs[k];
}

string
Dataset::to_csv(const string &sep) const {
  std::stringstream ss;
  for (size_t i = 0; i < this->att_descr_ptrs.size(); ++i) {
    if (i != 0) ss << sep << " ";
    ss << this->att_descr_ptrs[i]->get_name();
  }
  ss << std::endl;
  for (size_t i = 0; i < this->instances.size(); ++i) {
    for (size_t j = 0; j < this->att_descr_ptrs.size(); ++j) {
      std::cerr << "instance " << i << "; attribute " << j << std::endl;
      if (j != 0) ss << sep << " ";
      ss << this->instances[i].get_att_occurrence(j)->to_string();
    }
    ss << std::endl;
  }
  return ss.str();
}

void
Dataset::set_attribute_type(const size_t k, const AttributeType &type) {
  if (k >= this->att_descr_ptrs.size()) {
    std::stringstream ss;
    ss << "Cannot set attribute type for attribtue numer " << k
       << "; dataset only has " << this->att_descr_ptrs.size()
       << " attributes";
    throw CognoscoError(ss.str());
  }
  this->att_descr_ptrs[k]->set_type(type);
}

const AttributeType&
Dataset::get_attribute_type(const size_t k) const {
  return this->get_attribute_description_ptr(k)->get_attribute_type();
}


/*****************************************************************************
 *                            DatasetSplit CLASS                             *
 *****************************************************************************/

const std::vector<size_t>&
DatasetSplit::get_fold(size_t k) const {

}

/**
 * for a given attribute, count the number of times each of its values appears
 * in each fold.
 *
 * \return unordered_map indexed first by attribute-value, then by fold number
 *         the value being the count of occurrences for that label in that
 *         fold.
 */
DiscreteAttFoldCounts
DatasetSplit::count_label_occurrences(const std::string att_name) const {
  unordered_map<string, vector<size_t> > res;

  for (size_t k = 0; k < instances_in_fold.size(); ++k) {
    for (size_t j = 0; j < instances_in_fold[k].size(); ++k) {
      const string inst_label = d[instances_in_fold[k][j]][class_label];
      if (res.find(inst_label) == res.end())
        res[inst_label].resize(instances_in_fold.size());
      res[inst_label][k] += 1;
    }
  }

  return res;
}

const double
DatasetSplit::distance(const PartialAttFoldCounts &perfect_counts) {

}


void
DatasetSplit::swap(const FoldInstancePair &one,
                   const FoldInstancePair &two) {

}

std::pair<FoldInstancePair, FoldInstancePair>
DatasetSplit::random_swap() {

}


/*****************************************************************************
 *                          DatasetSplitter CLASS                            *
 *****************************************************************************/

DatasetSplit
DatasetSplitter::get_stratified_folds(const Dataset &d,
                                      const std::string &class_label) const {
  PartialAttFoldCounts ideal_counts =\
    this->compute_ideal_counts(d, class_label);
  DiscreteAttFoldCounts curr_counts =\
    assignments.count_label_occurrences(class_label);
  double current_score = distance(ideal_counts, curr_counts);
  for (size_t i = 0; i < this->MAX_NUM_SWAPS; ++i) {
    std::pair<FoldInstancePair, FoldInstancePair> swapped =\
      assignments.random_swap();
    DiscreteAttFoldCounts new_counts =\
      assignments.count_label_occurrences(class_label);
    double new_score = distance(ideal_counts, curr_counts);
    if (new_score < current_score) {
      current_score = new_score
    } else {
      // swap back
      assignments.swap(swapped.second, swapped.first);
    }
  }
}


/**
 * for a given attribute name, compute the ideal number of times each of its
 * values should appear per-fold in k folds if they are stratified by this
 * aatribute.
 */
PartialAttFoldCounts
DatasetSplitter::compute_ideal_counts(const Dataset &d,
                                      const string &att_name) const {
  unordered_map<string, vector<size_t> > r_count;
  for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
    r_count[inst[att_name].resize(k)
    for (size_t j = 0; j < k; ++j) r_count[inst[att_name][j] += 1
  }
  for (unordered_map<string, double>::iterator it = res.begin();
       it != r_count.end(); ++it) {
    for (size_t j = 0; j < k; ++j) it->second[j] /= k;
  }
  return r_count;
}
