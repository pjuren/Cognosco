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

#ifndef ATTRIBUTE_HPP_
#define ATTRIBUTE_HPP_

#include <string>
#include <vector>

enum AttributeType {NUMERIC, ORDINAL, NOMINAL, NULL_ATTRIBUTE_TYPE};

class AttributeDescription {
public:
  AttributeDescription() : name(""), attr_type(NULL_ATTRIBUTE_TYPE) {};
  AttributeDescription(std::string name, AttributeType type) : name(name),
    attr_type(type) {};
  const std::string& get_name() const {return this->name;}
  const AttributeType& get_attribute_type() const {return this->attr_type;}
  void set_type(const AttributeType &type) {this->attr_type = type;}
private:
  std::string name;
  AttributeType attr_type;
};

class AttributeOccurrence {
public:
  AttributeOccurrence(const AttributeDescription *attr_desc) :
    attr_desc(attr_desc) {};
  virtual std::string value_as_string() const = 0;
  const std::string& get_attribute_name() const {return attr_desc->get_name();}
private:
  const AttributeDescription *attr_desc;
};

class NumericAttributeOccurrence : public AttributeOccurrence {
public:
  NumericAttributeOccurrence(const double val,
                             const AttributeDescription *attr_desc) :
                             AttributeOccurrence(attr_desc), value(val) {};
  std::string value_as_string() const {
    return std::to_string(this->value);
  }
private:
  double value;
};

class NominalAttributeOccurrence : public AttributeOccurrence {
public:
  NominalAttributeOccurrence(const std::string &val,
                             const AttributeDescription *attr_desc) :
                             AttributeOccurrence(attr_desc), value(val) {};
  std::string value_as_string() const {
    return this->value;
  }
private:
  std::string value;
};

#endif
