/* The following appliess to this software package and all subparts therein
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

#ifndef PAIRWISE_DIST_LOAD_HPP_
#define PAIRWISE_DIST_LOAD_HPP_

// stl includes
#include <set>
#include <string>
#include <vector>
#include <iostream>

// Cognosco includes
#include "DistanceMatrix.hpp"

class PairwiseDistanceLoader {
public:
  void load(const std::string &filename, DistanceMatrix &d)  const;
  void load(std::istream &in, DistanceMatrix &d) const;
};

#endif
