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
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

// local Cognosco includes
#include "Dataset.hpp"
#include "CognoscoError.hpp"

// bring these into the local namespace
using std::string;
using std::vector;

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

 /**
  * ..
  */
DatasetSplit::DatasetSplit(const Dataset &d, const size_t k,
                           const std::string &labl) : label_split_upon(labl) {
  this->assignments.resize(k);
  size_t curr = 0;
  for (auto inst : d) {
    this->assignments[curr % k].push_back(inst.get_instance_id());
    curr += 1;
  }

  if (curr < k - 1) {
    std::stringstream ss;
    ss << "splitting dataset into " << k << " stratified folds failed; "
       << "dataset has only " << curr << " instances";
    throw CognoscoError(ss.str());
  }

  this->update_label_counts(d);
}

/**
 * for a given attribute, count the number of times each of its values appears
 * in each fold.
 *
 * \return unordered_map indexed first by attribute-value, then by fold number
 *         the value being the count of occurrences for that label in that
 *         fold.
 */
void
DatasetSplit::update_label_counts(const Dataset &d) {
  for (size_t k = 0; k < this->assignments.size(); ++k) {
    for (size_t j = 0; j < this->assignments[k].size(); ++j) {
      const size_t inst_id (this->assignments[k][j]);
      const string inst_label = d[inst_id][this->label_split_upon]->to_string();
      if (this->att_counts_per_fold.find(inst_label) ==\
          this->att_counts_per_fold.end()) {
        this->att_counts_per_fold[inst_label].resize(this->assignments.size());
      }
      this->att_counts_per_fold[inst_label][k] += 1;
    }
  }
}

double
DatasetSplit::distance(const AttFoldCounts &fcs) const {
  double dist = 0;
  for (auto att_counts : fcs) {
    const string attribute_name (att_counts.first);
    const vector<double> fold_counts_other (att_counts.second);
    const vector<double> &fold_counts_this =\
      this->get_fold_counts(attribute_name);
    this->att_counts_per_fold.find(attribute_name);
    assert(fold_counts_other.size() == fold_counts_this.size());
    for (size_t i = 0; i < fold_counts_other.size(); ++i) {
      dist += fabs(fold_counts_other[i] - fold_counts_this[i]);
    }
  }
  return dist;
}


const vector<double>&
DatasetSplit::get_fold_counts(const string &att_label) const {
  auto it = this->att_counts_per_fold.find(att_label);
  if (it == this->att_counts_per_fold.end()) {
    std::stringstream ss;
    ss << "no such label " << att_label;
    throw CognoscoError(ss.str());
  }
  return it->second;
}


void
DatasetSplit::swap(const FoldInstancePair &one,
                   const FoldInstancePair &two,
                   const Dataset &d) {
  size_t tmp_instance_id = this->assignments[one.first][one.second];
  this->assignments[one.first][one.second] =\
    this->assignments[two.first][two.second];
  this->assignments[two.first][two.second] = tmp_instance_id;

  // lazy .. could just update the ones we changed -- TODO
  this->update_label_counts(d);
}

std::pair<FoldInstancePair, FoldInstancePair>
DatasetSplit::random_swap(const Dataset &d) {
  size_t f_rand_fold = rand() % this->assignments.size();
  size_t s_rand_fold = rand() % this->assignments.size();
  size_t f_rand_instance = rand() % this->assignments[f_rand_fold].size();
  size_t s_rand_instance = rand() % this->assignments[s_rand_fold].size();

  FoldInstancePair p1 = std::make_pair(f_rand_fold, f_rand_instance);
  FoldInstancePair p2 = std::make_pair(s_rand_fold, s_rand_instance);
  swap(p1, p2, d);
  return std::make_pair(p1, p2);
}


/*****************************************************************************
 *                          DatasetSplitter CLASS                            *
 *****************************************************************************/

DatasetSplit
DatasetSplitter::split(const Dataset &d,
                       const std::string &class_label) const {
  DatasetSplit assignments(d, this->num_folds, class_label);
  AttFoldCounts ideal_counts =\
    this->compute_ideal_counts(d, class_label);
  double current_score = assignments.distance(ideal_counts);
  for (size_t i = 0; i < this->MAX_NUM_SWAPS; ++i) {
    std::pair<FoldInstancePair, FoldInstancePair> swapped =\
      assignments.random_swap(d);
    double new_score = assignments.distance(ideal_counts);
    if (new_score < current_score) {
      current_score = new_score;
    } else {
      // swap back
      assignments.swap(swapped.second, swapped.first, d);
    }
  }
  return assignments;
}


/**
 * for a given attribute name, compute the ideal number of times each of its
 * values should appear per-fold in k folds if they are stratified by this
 * aatribute.
 */
AttFoldCounts
DatasetSplitter::compute_ideal_counts(const Dataset &d,
                                      const string &att_name) const {
  AttFoldCounts r_count;
  for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
    const string att_value = (*inst)[att_name]->to_string();
    r_count[att_value].resize(this->num_folds);
    for (size_t j = 0; j < this->num_folds; ++j)
      r_count[att_value][j] += 1;
  }
  for (auto it = r_count.begin(); it != r_count.end(); ++it) {
    for (size_t j = 0; j < this->num_folds; ++j)
      it->second[j] /= this->num_folds;
  }
  return r_count;
}
