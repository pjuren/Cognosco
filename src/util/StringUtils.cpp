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
#include <sstream>

// local incudes
#include "StringUtils.hpp"

// bring these into the local namespace
using std::string;

std::string
rstrip(const std::string &s) {
  size_t trim = s.size();
  while(std::isspace(s[trim-1])) trim -= 1;
  return s.substr(0, trim);
}

std::string
lstrip(const std::string &s) {
  size_t trim = 0;
  while(std::isspace(s[trim])) trim += 1;
  return s.substr(trim, std::string::npos);
}

std::string
strip(const std::string &s) {
  return rstrip(lstrip(s));
}

string
join(const std::vector<std::string> &parts, const std::string &sep) {
  // TODO -- nasty doing all these string concats... better way later..
  string res = "";
  for (size_t i = 0; i < parts.size(); ++i) {
    if (i != 0) res += sep;
    res += parts[i];
  }
  return res;
}

string
join(const std::set<std::string> &parts, const std::string &sep) {
  // TODO -- nasty doing all these string concats... better way later..
  // TODO -- repats above -- make into template.
  string res = "";
  for (auto it = parts.begin(); it != parts.end(); ++it) {
    if (it != parts.begin()) res += sep;
    res += (*it);
  }
  return res;
}

string
join(const std::set<int> &parts, const std::string &sep) {
  // TODO -- nasty doing all these string concats... better way later..
  // TODO -- repats above -- make into template.
  std::stringstream res;
  for (auto it = parts.begin(); it != parts.end(); ++it) {
    if (it != parts.begin()) res << sep;
    res << (*it);
  }
  return res.str();
}
