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

#ifndef CSV_LOADER_HPP_
#define CSV_LOADER_HPP_

#include "Dataset.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class CSVLoader {
public:
  CSVLoader() : seperator(",") {};
  CSVLoader(const std::string &sep) : seperator(sep) {};

  void load(const std::string &filename, Dataset &dataset,
            const bool VERBOSE=false) const;
private:
  bool expect_header;
  std::string seperator;
};

#endif
