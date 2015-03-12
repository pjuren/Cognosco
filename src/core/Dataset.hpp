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

#ifndef DATASET_HPP_
#define DATASET_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "Instance.hpp"
#include "Attribute.hpp"
#include "CognoscoError.hpp"

class Dataset {
public:
  // constructors and destructors
  Dataset() : instances(std::vector<Instance>()),
              att_descr_ptrs(std::vector<Attribute*>()) {}
  Dataset(const Dataset &d);
  Dataset(const Dataset &d, const std::vector<size_t> exclude_insts);
  //~Dataset();

  // types
  typedef std::vector<Instance>::iterator iterator;
  typedef std::vector<Instance>::const_iterator const_iterator;
  typedef std::vector<Attribute*>::const_iterator const_attribute_iterator;

  // inspectors
  const Attribute* get_attribute_description_ptr(size_t k) const;
  std::string to_csv(const std::string &sep = ",") const;
  const_iterator begin() const { return this->instances.begin(); }
  const_iterator end() const { return this->instances.end(); }
  const_attribute_iterator begin_attributes() const { return this->att_descr_ptrs.begin(); }
  const_attribute_iterator end_attributes() const { return this->att_descr_ptrs.end(); }
  size_t size() const {return instances.size(); }
  const AttributeType& get_attribute_type(const size_t k) const;
  const Instance& operator[] (const int instance_id) const {
    // TODO fix nasty O(n)
    for (size_t i = 0; i < this->instances.size(); ++i) {
      if (this->instances[i].get_instance_id() == instance_id)
        return this->instances[i];
    }
    std::stringstream ss;
    ss << "No instance with id " << instance_id;
    throw CognoscoError(ss.str());
  }

  // mutators
  void add_attribute(const Attribute &att_desc);
  void add_instance(const Instance &instance);
  void set_attribute_type(const size_t k, const AttributeType &type);
  void delete_attribute(Attribute *att_desc);
  iterator begin() { return this->instances.begin(); }
  iterator end() { return this->instances.end(); }

private:
  // private instance variables
  std::vector<Instance> instances;
  std::vector<Attribute*> att_descr_ptrs;
};

typedef std::unordered_map<std::string, std::vector<double> > AttFoldCounts;
typedef std::pair<size_t, size_t> FoldInstancePair;

class DatasetSplit {
public:
  // constructors
  DatasetSplit(const Dataset &d, const size_t k, const std::string &label);

  // inspectors
  const std::vector<size_t>& get_fold(size_t k) const;
  double distance(const AttFoldCounts &counts) const;
  double distance(const DatasetSplit &split) const;
  const std::vector<size_t>& operator[] (const int nIndex) const {
    if (nIndex >= this->assignments.size()) {
      std::stringstream ss;
      ss << "trying to get fold " << nIndex << " from dataset split with only "
         << this->assignments.size() << " folds";
      throw CognoscoError(ss.str());
    }
    return assignments[nIndex];
  }

  // mutators
  void swap(const FoldInstancePair &one, const FoldInstancePair &two,
            const Dataset &d);
  std::pair<FoldInstancePair, FoldInstancePair> random_swap(const Dataset &d);

private:
  // private instance variables
  std::string label_split_upon;
  std::vector<std::vector<size_t> > assignments;
  AttFoldCounts att_counts_per_fold;

  // private inspectors
  const std::vector<double>& get_fold_counts(const std::string &att_label) const;

  // private mutators
  void update_label_counts(const Dataset &d);
};

class DatasetSplitter {
public:
  // constructors
  DatasetSplitter(size_t k) : num_folds(k) {};

  // inspectors
  DatasetSplit split(const Dataset &d, const std::string &class_label) const;

private:
  // private instance variables
  size_t num_folds;
  const size_t MAX_NUM_SWAPS=1000;

  // private inspectors
  AttFoldCounts compute_ideal_counts(const Dataset &d,
                                     const std::string &cls_labl) const;
};

#endif
