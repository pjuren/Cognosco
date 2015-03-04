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

#include <sstream>

#include "Instance.hpp"
#include "CognoscoError.hpp"

size_t Instance::instance_counter = 0;


/*****************************************************************************
 *                       CONSTRUCTORS AND DESTRUCTORS                        *
 *****************************************************************************/

Instance::Instance() {
  this->instance_counter += 1;
}

Instance::Instance(const Instance &inst) {
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
Instance::add_attribute_occurrance(const double value,
                                    const Attribute *att_desc_p) {
  AttributeOccurrence *x = new NumericAttributeOccurrence(value, att_desc_p);
  this->attributes.push_back(x);
  this->instance_counter += 1;
};

void
Instance::add_attribute_occurrance(const std::string value,
                                    const Attribute *att_desc_p) {
  AttributeOccurrence *x = new NominalAttributeOccurrence(value, att_desc_p);
  this->attributes.push_back(x);
  this->instance_counter += 1;
};
