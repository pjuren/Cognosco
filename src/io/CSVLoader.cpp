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
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// local Cognosco includes
#include "CSVLoader.hpp"
#include "Dataset.hpp"
#include "StringUtils.hpp"
#include "CognoscoError.hpp"

// bring these into local namespace
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

void
CSVLoader::load(const std::string &filename, Dataset &dataset,
                const bool VERBOSE) const {
  ifstream strm(filename.c_str());

  if (!strm.good()) {
    std::stringstream ss;
    ss << "failed to open file: " << filename;
    throw CognoscoError(ss.str());
  }

  string line;
  bool first = true;
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
        dataset.add_attribute(Attribute(parts[i], NULL_ATTRIBUTE_TYPE));
      }
      first = false;
    } else {
      // if it's not the first line, then use it to create instances
      Instance instance;
      for (size_t i = 0; i < parts.size(); ++i) {
        const Attribute* ad_ptr =\
          dataset.get_attribute_description_ptr(i);
        const AttributeType att_type (dataset.get_attribute_type(i));
        if (att_type == NULL_ATTRIBUTE_TYPE) {
          try {
            // try to treat as a floating point number
            double d_val (std::stof(parts[i]));
            instance.add_attribute_occurrance(d_val, ad_ptr);
            dataset.set_attribute_type(i, NUMERIC);
          } catch (const std::invalid_argument &e) {
            // if we fail to parse as a float, treat as nominal
            instance.add_attribute_occurrance(strip(parts[i]), ad_ptr);
            dataset.set_attribute_type(i, NOMINAL);
          }
        } else if (att_type == NOMINAL) {
          instance.add_attribute_occurrance(strip(parts[i]), ad_ptr);
        } else if (att_type == NUMERIC) {
          try {
            double d_val (std::stof(parts[i]));
            instance.add_attribute_occurrance(d_val, ad_ptr);
          } catch (const std::invalid_argument &e) {
            std::stringstream ss;
            ss << "failed to parse " << parts[i] << " as numeric";
            throw CognoscoError(ss.str());
          }
        } else {
          std::stringstream ss;
          ss << "failed to parse " << parts[i] << "; unknown attribute type";
          throw CognoscoError(ss.str());
        }
      }
      dataset.add_instance(instance);
    }
  }

  if (VERBOSE) {
    cerr << "loaded dataset from " << filename << " with attributes: ";
    for (Dataset::const_attribute_iterator it = dataset.begin_attributes();
         it != dataset.end_attributes(); ++it) {
      if (it != dataset.begin_attributes()) cerr << ", ";
      cerr << (*it)->get_name();
    }
    cerr << endl;
  }
}
