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

// TODO the double de-reference required on the iterator is nasty; solution
// is to write custom iterator -- maybe later...

class Instance {
public:
  // constructors and destructors
  Instance();
  Instance(const Instance &inst);
  ~Instance();

  // types
  typedef std::vector<AttributeOccurrence*>::iterator iterator;
  typedef std::vector<AttributeOccurrence*>::const_iterator const_iterator;

  // inspectors
  const_iterator begin() const { return this->attributes.begin(); }
  const_iterator end() const { return this->attributes.end(); }
  const AttributeOccurrence* get_att_occurrence(const size_t i) const;
  const AttributeOccurrence* get_att_occurrence(const std::string &name) const;
  size_t get_instance_id() const;
  const AttributeOccurrence* operator[] (const int i) const {
    return this->get_att_occurrence(i);
  }
  const AttributeOccurrence* operator[] (const std::string &name) const {
    return this->get_att_occurrence(name);
  }
  std::string to_string() const;

  // mutators
  void add_attribute_occurrence(const std::string value,
                                const Attribute *att_desc_ptr);
  void add_attribute_occurrence(const double value,
                                const Attribute *att_desc_ptr);
  void delete_attribute_occurrence(const std::string &att_name);
  iterator begin() { return this->attributes.begin(); }
  iterator end() { return this->attributes.end(); }

private:
  // instance variables
  size_t instance_id;
  std::vector<AttributeOccurrence*> attributes;

  // static class variables
  static size_t instance_counter;
};

#endif
