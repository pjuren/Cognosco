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

  // inspectors
  const AttributeDescription* get_attribute_description_ptr(size_t k) const;
  std::string to_csv(const std::string &sep = ",") const;

  // mutators
  void add_attribute(const AttributeDescription &att_desc);
  void add_instance(const Instance &instance);
  void set_attribute_type(const size_t k, const AttributeType &type);
  const AttributeType& get_attribute_type(const size_t k) const;

private:

  std::vector<Instance> instances;
  std::vector<AttributeDescription*> att_descr_ptrs;
};

#endif
