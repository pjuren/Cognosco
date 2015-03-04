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
#include <string>

// local Cognosco includes
#include "Attribute.hpp"

// bring these into the local namespace
using std::string;

/*****************************************************************************
 *                       ATTRIBUTE DESCRIPTION CLASS                         *
 *****************************************************************************/

Attribute::Attribute() : name(""),
  attr_type(NULL_ATTRIBUTE_TYPE) {};

Attribute::Attribute(string name, AttributeType type) :
  name(name), attr_type(type) {};

const std::string&
Attribute::get_name() const {
  return this->name;
}

const AttributeType&
Attribute::get_attribute_type() const {
  return this->attr_type;
}

void
Attribute::set_type(const AttributeType &type) {
  this->attr_type = type;
}


/*****************************************************************************
 *                      BASE ATTRIBTUE OCCURRENCE CLASS                      *
 *****************************************************************************/

AttributeOccurrence::AttributeOccurrence(const Attribute *attr_desc) :
    attr_desc(attr_desc) {};

const string&
AttributeOccurrence::get_attribute_name() const {
  return attr_desc->get_name();
}


/*****************************************************************************
 *                     NUMERIC ATTRIBTUE OCCURRENCE CLASS                    *
 *****************************************************************************/

NumericAttributeOccurrence::NumericAttributeOccurrence(const double val,
                             const Attribute *attr_desc) :
                             AttributeOccurrence(attr_desc), value(val) {};

string
NumericAttributeOccurrence::to_string() const {
  return std::to_string(this->value);
}


/*****************************************************************************
 *                     NOMINAL ATTRIBTUE OCCURRENCE CLASS                    *
 *****************************************************************************/

NominalAttributeOccurrence::NominalAttributeOccurrence(const std::string &val,
                           const Attribute *attr_desc) :
                           AttributeOccurrence(attr_desc), value(val) {};

string NominalAttributeOccurrence::to_string() const {
  return this->value;
}
