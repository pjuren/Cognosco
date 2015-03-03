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

// stl includes
#include <sstream>

// local Cognosco includes
#include "Dataset.hpp"
#include "CognoscoError.hpp"

void
Dataset::add_instance(const Instance &inst) {
  this->instances.push_back(inst);
}

void
Dataset::add_attribute(const AttributeDescription &att_desc) {
  att_descr_ptrs.push_back(new AttributeDescription(att_desc));
}

const AttributeDescription*
Dataset::get_attribute_description_ptr(size_t k) const {
  if (k >= this->att_descr_ptrs.size()) {
    std::stringstream ss;
    ss << "Cannot get attribute description " << k << "; dataset only has "
       << this->att_descr_ptrs.size() << " attributes";
    throw CognoscoError(ss.str());
  }
}
