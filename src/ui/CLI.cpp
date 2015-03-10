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
#include <vector>
#include <string>
#include <cassert>
#include <sstream>

// local includes
#include "CLI.hpp"

// bring these names into the local namespace
using std::vector;
using std::string;


/*****************************************************************************
 *                             OPTION CLASSES                                *
 *****************************************************************************/

void
StringOption::parse(OptionInstance &inst, std::string &dest) const {
  if (!this->accpt_vals.empty() && this->accpt_vals.find(inst.get_value()) ==\
      this->accpt_vals.end()) {
    stringstream ss;
    ss << "Value for option " << this->get_long_name()
       << "is not valid; accepted values are " << join(this->acceptable_vals);
  }
  dest = inst.get_value();
}

void
IntegerOption::parse(const OptionInstance& inst, double &dest) const {
  int r;
  this->parse(inst, r);
  dest = double(r);
}

void
IntegerOption::parse(const OptionInstance& inst, std::string &dest) const {
  int res;
  try {
    res = std::stoi(inst.get_value());
  } catch (const std::invalid_argument &e) {
    stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as integer; "
       << "invalid format";
    throw OptionError(ss.str());
  }
  if ((res > this->max_val) || (res < this->min_val)) {
    stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as integer; "
       << "value outside of range (" << this->min_val << " - "
       << this->max_val << " )";
    throw OptionError(ss.str());
  }
  if (!this->accpt_vals.empty() && this->accpt_vals.find(inst.get_value()) ==\
      this->accpt_vals.end()) {
    stringstream ss;
    ss << "Value for option " << this->get_long_name()
       << "is not valid; accepted values are " << join(this->acceptable_vals);
    throw OptionError(ss.str());
  }
  dest = res;
}


/*****************************************************************************
 *                        CommandLineInterface CLASS                         *
 *****************************************************************************/



/*****************************************************************************
 *                             Commandline CLASS                             *
 *****************************************************************************/

Commandline::Commandline(int argc, const char* argv[]) {
  string option_name;
  for (size_t i = 0; i < argc; ++i) {
    string token(argv[i]);
    assert(token.size() >= 1);

    if (token[0] == '-') {
      if (!option_name.empty())
        op_insts.push_back(OptionInstance(option_name, "true"));
      option_name = token.substr(1);
      if (option_name[0] == '-') option_name = option_name.substr(1);
    } else {
      if (option_name.empty())
        this->arguments.push_back(token);
      else {
        op_insts.push_back(OptionInstance(option_name, token));
      }
    }
  }
}

/**
 * Find the option instance that matches this option in the command line,
 * check that it satisfies all constraints, remove it and return it to the
 * caller.
 * TODO: fix nasty O(n) complexity; instances can be in set, no need for vector.
 */
OptionInstance
Commandline::consume_option(const Option &option) {
  OptionInstance res;
  bool found = false;
  vector<OptionInstance>::const_iterator it;
  for (it = this->op_insts.begin(); it != this->op_insts.end(); ++it) {
    if (it->get_name() == option.get_short_name() ||
        it->get_name() == option.get_long_name()) {
      res = (*it);
      found = true;
    }
  }

  if (!found)
    throw OptionError("No such option: " + option.get_long_name());

  this->op_insts.erase(it);
  return res;
}
