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
private:
  std::string name;
  AttributeType attr_type;
};

class AttributeOccurrence {
public:
  AttributeOccurrence(const double val, const AttributeDescription *attr_desc) :
    value(val), attr_desc(attr_desc) {};
private:
  double value;
  const AttributeDescription *attr_desc;
};

#endif
