/* The following applys to this software package and all subparts therein
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

#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include <string>
#include <vector>
#include <iostream>

#include "Attribute.hpp"

class Instance {
public:
  Instance() {
    this->instance_counter += 1;
  };
  Instance(std::vector<AttributeOccurrence> att_occurances);
  void add_attribute_occurrance(const double value,
                                const AttributeDescription *att_desc_ptr) {
    AttributeOccurrence *x = new NumericAttributeOccurrence(value, att_desc_ptr);
    this->attributes.push_back(x);
    this->instance_counter += 1;
  };
  void add_attribute_occurrance(const std::string value,
                                const AttributeDescription *att_desc_ptr) {
    this->attributes.push_back(new NominalAttributeOccurrence(value, att_desc_ptr));
    this->instance_counter += 1;
  };

  const AttributeOccurrence* get_attribute_occurrence(const size_t i) const;
private:
  size_t instance_id;
  std::vector<AttributeOccurrence*> attributes;

  static size_t instance_counter;
};

#endif
