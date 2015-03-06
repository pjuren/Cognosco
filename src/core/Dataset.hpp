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
#include <unordered_map>

#include "Instance.hpp"
#include "Attribute.hpp"

class Dataset {
public:
  // constructors and destructors
  //Dataset();
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

  // mutators
  void add_attribute(const Attribute &att_desc);
  void add_instance(const Instance &instance);
  void set_attribute_type(const size_t k, const AttributeType &type);
  const AttributeType& get_attribute_type(const size_t k) const;
  iterator begin() { return this->instances.begin(); }
  iterator end() { return this->instances.end(); }

private:
  // private instance variables
  std::vector<Instance> instances;
  std::vector<Attribute*> att_descr_ptrs;

  // private types
  typedef std::unordered_map<std::string, std::vector<size_t> >
          AttributeFoldCounts;

  // private helper functions for stratified dataset splitting

};

typedef std::unordered_map<std::string, std::vector<size_t> >
        DiscreteAttFoldCounts;
typedef std::unordered_map<std::string, std::vector<size_t> >
        PartialAttFoldCounts;
typedef std::pair<size_t, size_t> FoldInstancePair;

class DatasetSplit {
public:
  // inspectors
  const std::vector<size_t>& get_fold(size_t k) const;
  DiscreteAttFoldCounts
  count_label_occurrences(const std::string att_name) const;
  const double distance(const PartialAttFoldCounts &perfect_counts);

  // mutators
  void swap(const size_t inst_1, const size_t fold_1,
            const size_t inst_2, const size_t fold_2);
  void random_swap();

private:
  std::vector<std::vector<size_t> > assignments;
};

class DatasetSplitter {
public:
  DatasetSplitter(size_t k) : num_folds(k) {};
  DatasetSplit get_stratified_folds(const Dataset &d,
                                    const std::string &class_label) const;
private:
  // private instance variables
  size_t num_folds;
  DatasetSplit assignments;
  const size_t MAX_NUM_SWAPS=1000;

  // private inspectors
  PartialAttFoldCounts compute_ideal_counts(const std::string &cls_labl) const;
  double distance(PartialAttFoldCounts &p, DiscreteAttFoldCounts &q) const;
};

#endif
