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

  // inspectors
  const Attribute* get_attribute_description_ptr(size_t k) const;
  std::string to_csv(const std::string &sep = ",") const;
  const_iterator begin() const { return this->instances.begin(); }
  const_iterator end() const { return this->instances.end(); }
  size_t size() const {return instances.size(); }

  // mutators
  void add_attribute(const Attribute &att_desc);
  void add_instance(const Instance &instance);
  void set_attribute_type(const size_t k, const AttributeType &type);
  const AttributeType& get_attribute_type(const size_t k) const;
  iterator begin() { return this->instances.begin(); }
  iterator end() { return this->instances.end(); }

private:

  std::vector<Instance> instances;
  std::vector<Attribute*> att_descr_ptrs;
};

#endif
