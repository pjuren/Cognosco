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

// stl includes
#include <sstream>
#include <string>

// local includes
#include "Instance.hpp"
#include "CognoscoError.hpp"

// bring the following into the local namespace
using std::string;
using std::vector;

// init. static variables in Instance class
size_t Instance::instance_counter = 0;


/*****************************************************************************
 *                       CONSTRUCTORS AND DESTRUCTORS                        *
 *****************************************************************************/

Instance::Instance() : instance_id (Instance::instance_counter) {
  this->instance_counter += 1;
}

Instance::Instance(const Instance &inst) : instance_id(inst.instance_id) {
  for (size_t i = 0; i < inst.attributes.size(); ++i) {
    AttributeOccurrence *cpy = inst.attributes[i]->clone();
    this->attributes.push_back(cpy);
  }
}

Instance::~Instance() {
  for (size_t i = 0; i < this->attributes.size(); ++i) {
    delete this->attributes[i];
  }
}


/*****************************************************************************
 *                               INSPECTORS                                  *
 *****************************************************************************/

const AttributeOccurrence*
Instance::get_att_occurrence(const size_t i) const {
  if (i >= this->attributes.size()) {
    std::stringstream ss;
    ss << "cannot get attribute number " << i << " as isntance has only "
       << this->attributes.size() << " attributes";
    throw CognoscoError(ss.str());
  }
  return this->attributes[i];
}

/**
 * \brief
 * \TODO fix nasty O(n) performance.
 */
const AttributeOccurrence*
Instance::get_att_occurrence(const std::string &name) const {
  for (size_t i = 0; i < this->attributes.size(); ++i) {
    if (this->attributes[i]->get_attribute_name() == name)
      return this->attributes[i];
  }
  std::stringstream ss;
  ss << "No such attribute: " << name;
  throw CognoscoError(ss.str());
}

size_t
Instance::get_instance_id() const {
  return this->instance_id;
}

/*****************************************************************************
 *                                MUTATORS                                   *
 *****************************************************************************/

void
Instance::add_attribute_occurrence(const double value,
                                    const Attribute *att_desc_p) {
  AttributeOccurrence *x = new NumericAttributeOccurrence(value, att_desc_p);
  this->attributes.push_back(x);
};

void
Instance::add_attribute_occurrence(const std::string value,
                                    const Attribute *att_desc_p) {
  AttributeOccurrence *x = new NominalAttributeOccurrence(value, att_desc_p);
  this->attributes.push_back(x);
};

void
Instance::delete_attribute_occurrence(const string &att_name) {
  auto new_end = std::remove_if(this->attributes.begin(),
                                this->attributes.end(),
                                [=](AttributeOccurrence* ao) {
                                  return ao->get_attribute_name() == att_name;
                                });
  for (auto it = new_end; it != this->attributes.end(); ++it)
    delete (*it);
  this->attributes.erase(new_end, this->attributes.end());
}
