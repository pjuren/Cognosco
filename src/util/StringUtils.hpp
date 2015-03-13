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

#ifndef STRING_UTILS_HPP_
#define STRING_UTILS_HPP_

#include <string>
#include <vector>
#include <set>
#include <sstream>

/******************************************************************************
 *                                  HASHING                                   *
 ******************************************************************************/

struct string_pair_hash {
  size_t operator()(const std::pair<std::string, std::string> &names) const {
    return std::hash<std::string>()(names.first) ^ \
           std::hash<std::string>()(names.second);
  }
};



template < class ContainerT >
void
tokenize(const std::string& str, ContainerT& tokens,
         const std::string& delimiter = " ", bool trimEmpty = true) {
   std::string::size_type pos, lastPos = 0;

   using value_type = typename ContainerT::value_type;
   using size_type  = typename ContainerT::size_type;

   while(true) {
      pos = str.find(delimiter, lastPos);
      if(pos == std::string::npos) {
         pos = str.length();

         if(pos != lastPos || !trimEmpty)
            tokens.push_back(value_type(str.data()+lastPos,
                  (size_type)pos-lastPos ));

         break;
      }
      else {
         if(pos != lastPos || !trimEmpty)
            tokens.push_back(value_type(str.data()+lastPos,
                  (size_type)pos-lastPos ));
      }

      lastPos = pos + 1;
   }
}

std::string rstrip(const std::string &s);
std::string lstrip(const std::string &s);
std::string strip(const std::string &s);

template <class T>
std::string
join(const std::vector<T> &parts, const std::string &sep) {
  // TODO -- nasty doing all these string concats... better way later..
  std::stringstream res;
  for (auto it = parts.begin(); it != parts.end(); ++it) {
    if (it != parts.begin()) res << sep;
    res << (*it);
  }
  return res.str();
}

template <class T>
std::string
join(const std::set<T> &parts, const std::string &sep) {
  // TODO -- nasty doing all these string concats... better way later..
  std::stringstream res;
  for (auto it = parts.begin(); it != parts.end(); ++it) {
    if (it != parts.begin()) res << sep;
    res << (*it);
  }
  return res.str();
}

#endif
