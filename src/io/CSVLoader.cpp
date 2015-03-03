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
#include<string>
#include<vector>
#include<fstream>
#include<iostream>

// local Cognosco includes
#include "CSVLoader.hpp"
#include "Dataset.hpp"
#include "StringUtils.hpp"

// bring these into local namespace
using std::string;
using std::vector;
using std::ifstream;

void
CSVLoader::load(const std::string &filename, Dataset &dataset) const {
  ifstream strm(filename.c_str());
  string line;
  bool first;
  while (strm.good()) {
    getline(strm, line);
    line = strip(line);
    if (line.empty()) continue;

    vector<string> parts;
    tokenize(line, parts, seperator);

    if (first) {
      // if this is the first line and we're expecting a header, use it to
      // create attributes
      for (size_t i = 0; i < parts.size(); ++i) {
        // TODO remove assumption that everything is numeric...
        dataset.add_attribute(AttributeDescription(parts[i], NUMERIC));
      }
    } else {
      // if it's not the first line, then use it to create instances
      Instance instance;
      for (size_t i = 0; i < parts.size(); ++i) {
        // TODO remove assumption that everything is numeric...
        instance.add_attribute_occurrance(std::stof(parts[i]),
                                          dataset.get_attribute_description_ptr(i));
      }
      dataset.add_instance(instance);

    }
  }
}
