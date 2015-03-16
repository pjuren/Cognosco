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

// stl includes
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// local Cognosco includes
#include "CognoscoError.hpp"

// TODO probably not required; occurrences have virtual behavior, so just
// do what you want to do and if it isn't possible, it'll fail
enum AttributeType {NUMERIC, ORDINAL, NOMINAL, NULL_ATTRIBUTE_TYPE};

class Attribute {
public:
  // constructors
  Attribute();
  Attribute(std::string name, AttributeType type);
  ~Attribute() {}

  // inspectors
  const std::string& get_name() const;
  const AttributeType& get_attribute_type() const;
  std::string get_attribute_type_string() const;

  // mutators
  void set_type(const AttributeType &type);
private:
  std::string name;
  AttributeType attr_type;
};

class AttributeOccurrence {
public:
  // constructors and destructors
  AttributeOccurrence(const Attribute *attr_desc);
  virtual ~AttributeOccurrence() {};
  virtual AttributeOccurrence *clone() const = 0;

  // inspectors
  virtual std::string to_string() const = 0;
  const std::string& get_attribute_name() const;

  // numeric operations
  virtual double operator *(const double d) {
    std::stringstream ss;
    ss << "multiplication by double for attribute "
       << this->get_attribute_name() << " undefined";
    throw CognoscoError(ss.str());
  }
  virtual double operator *(const double d) const {
    std::stringstream ss;
    ss << "multiplication by double for attribute "
       << this->get_attribute_name() << " undefined";
    throw CognoscoError(ss.str());
  }

protected:
  // constructors
  AttributeOccurrence(const AttributeOccurrence &ac) :
    attr_desc(ac.attr_desc) {};

private:
  const Attribute *attr_desc;
};

class NumericAttributeOccurrence : public AttributeOccurrence {
public:
  // constructors
  NumericAttributeOccurrence(const double val,
                             const Attribute *attr_desc);
  ~NumericAttributeOccurrence() {};


  // inspectors
  std::string to_string() const override;
  NumericAttributeOccurrence *clone() const {
    return new NumericAttributeOccurrence(*this);
  }

  // numeric operations
  double operator *(const double d) { return d * this->value; }
  double operator *(const double d) const { return d * this->value; }
protected:
  NumericAttributeOccurrence(const NumericAttributeOccurrence &o) :
    AttributeOccurrence(o), value(o.value) {}
private:
  double value;
};

class NominalAttributeOccurrence : public AttributeOccurrence {
public:
  // constructors
  NominalAttributeOccurrence(const std::string &val,
                             const Attribute *attr_desc);
  ~NominalAttributeOccurrence() {};

  // inspectors
  std::string to_string() const override;
  NominalAttributeOccurrence *clone() const {
    return new NominalAttributeOccurrence(*this);
  }
protected:
  NominalAttributeOccurrence(const NominalAttributeOccurrence &o) :
    AttributeOccurrence(o), value(o.value) {}
private:
  std::string value;
};

#endif
