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

// stl includes
#include <string>
#include <vector>
#include <fstream>

// local Cognosco includes
#include "PairwiseDistanceLoader.hpp"

// bring these into the local namespace
using std::string;

void
PairwiseDistanceLoader::load(const string &fn, DistanceMatrix &d) const {
  std::ifstream strm(fn.c_str());
  load(strm, d);
}

void
PairwiseDistanceLoader::load(std::istream &in, DistanceMatrix &d) const {
  d.clear();
  string n1, n2;
  double dist;
  while(in.good()){
    in >> n1 >> n2 >> dist;
    d[std::make_pair(n1,n2)] = dist;
  }
}
