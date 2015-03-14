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
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>

// local includes
#include "CLI.hpp"
#include "StringUtils.hpp"

// bring these names into the local namespace
using std::vector;
using std::string;
using std::set;


/*****************************************************************************
 *                             OPTION CLASSES                                *
 *****************************************************************************/

void
StringOption::parse(const OptionInstance &inst, std::string &dest) const {
  if (!this->accpt_vals.empty() && this->accpt_vals.find(inst.get_value()) ==\
      this->accpt_vals.end()) {
    std::stringstream ss;
    ss << "Value for option " << this->get_long_name()
       << "is not valid; accepted values are " << join(this->accpt_vals, ", ");
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
    std::stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as integer; "
       << "invalid format";
    throw OptionError(ss.str());
  }
  if ((res > this->max_val) || (res < this->min_val)) {
    std::stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as integer; "
       << "value outside of range (" << this->min_val << " - "
       << this->max_val << " )";
    throw OptionError(ss.str());
  }
  if (!this->accpt_vals.empty() && this->accpt_vals.find(res) ==\
      this->accpt_vals.end()) {
    std::stringstream ss;
    ss << "Value for option " << this->get_long_name()
       << "is not valid; accepted values are " << join(this->accpt_vals, ", ");
    throw OptionError(ss.str());
  }
  dest = res;
}

void
StringOption::parse_default(string &dest) const {
  if (!has_default) {
    std::stringstream ss;
    ss << "No default value for option " << this->get_long_name();
    throw OptionError(ss.str());
  }
  dest = this->default_val;
}

void
BooleanOption::parse(const OptionInstance& inst, bool &dest) const {
  string str_val = strip(inst.get_value());
  if ((str_val == "TRUE") || (str_val == "True") || (str_val == "true"))
    dest = true;
  else if ((str_val == "FALSE") || (str_val == "False") || (str_val == "false"))
    dest = false;
  else {
    std::stringstream ss;
    ss << "Value for option " << this->get_long_name() << "is not valid";
    throw OptionError(ss.str());
  }
}

void
BooleanOption::parse_default(bool &dest) const {
  if (!has_default) {
    std::stringstream ss;
    ss << "No default value for option " << this->get_long_name();
    throw OptionError(ss.str());
  }
  dest = this->default_val;
}

void
SizeOption::parse(const OptionInstance& inst, size_t &dest) const {
  size_t res;
  int i_res;
  try {
    i_res = std::stoi(inst.get_value());
  } catch (const std::invalid_argument &e) {
    std::stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as size; "
       << "invalid format";
    throw OptionError(ss.str());
  }
  if (i_res < 0) {
    std::stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as size; "
       << "value is less than zero";
    throw OptionError(ss.str());
  }
  // i_res known to be > 0
  res = i_res;

  if ((res > this->max_val) || (res < this->min_val)) {
    std::stringstream ss;
    ss << "failed to parse " << inst.get_value() << " as size; "
       << "value outside of range (" << this->min_val << " - "
       << this->max_val << " )";
    throw OptionError(ss.str());
  }
  if (!this->accpt_vals.empty() && this->accpt_vals.find(res) ==\
      this->accpt_vals.end()) {
    std::stringstream ss;
    ss << "Value for option " << this->get_long_name()
       << "is not valid; accepted values are " << join(this->accpt_vals, ", ");
    throw OptionError(ss.str());
  }
  dest = res;
}

// TODO -- can parse default functsion be replaced by template in super?
// all basically the same.
void
SizeOption::parse_default(size_t &dest) const {
  if (!has_default) {
    std::stringstream ss;
    ss << "No default value for option " << this->get_long_name();
    throw OptionError(ss.str());
  }
  dest = this->default_val;
}

void
StringlistOption::parse(const OptionInstance &inst, set<string> &dest) const {
  vector<string> tmp;
  tokenize(inst.get_value(), tmp, this->separator);
  dest.clear();
  dest.insert(tmp.begin(), tmp.end());
}

// TODO -- replace with template? just container type varies...
void
StringlistOption::parse(const OptionInstance &inst,
                        vector<string> &dest) const {
  dest.clear();
  tokenize(inst.get_value(), dest, this->separator);
}

void
StringlistOption::parse_default(std::set<std::string> &dest) const {
  if (!has_default) {
    std::stringstream ss;
    ss << "No default value for option " << this->get_long_name();
    throw OptionError(ss.str());
  }
  vector<string> tmp;
  tokenize(this->default_val, tmp, this->separator);
  dest.clear();
  dest.insert(tmp.begin(), tmp.end());
}

// TODO -- replace with template? just container type varies...
void
StringlistOption::parse_default(std::vector<std::string> &dest) const {
  if (!has_default) {
    std::stringstream ss;
    ss << "No default value for option " << this->get_long_name();
    throw OptionError(ss.str());
  }
  dest.clear();
  tokenize(this->default_val, dest, this->separator);
}

/*****************************************************************************
 *                        CommandLineInterface CLASS                         *
 *****************************************************************************/

CommandlineInterface::CommandlineInterface(const string &program_name,
                                           const string &description) :
  program_name(program_name), program_description (description), min_args(0),
  max_args (std::numeric_limits<size_t>::max()), options (vector<Option*>()) {}

CommandlineInterface::CommandlineInterface(const string &program_name,
                                           const string &description,
                                           const size_t min_args,
                                           const size_t max_args) :
  program_name(program_name), program_description (description),
  min_args(min_args), max_args (max_args), options (vector<Option*>()) {}



void
CommandlineInterface::add_string_option(const std::string &long_name,
                                        const char &short_name,
                                        const std::string &desc,
                                        const std::set<std::string> &accpt_vals,
                                        const std::string default_value) {
  options.push_back(new StringOption(long_name, short_name, desc,
                                     default_value, accpt_vals));
}

void
CommandlineInterface::add_string_option(const std::string &long_name,
                                        const char &short_name,
                                        const std::string &desc,
                                        const std::set<std::string> &accpt_vals) {
  options.push_back(new StringOption(long_name, short_name, desc, accpt_vals));
}

/**
 * Add a string option with just names and description. Any string will be a
 * a valid value for this option. Since no default is provided, this is
 * assumed to be a required option.
 */
void
CommandlineInterface::add_string_option(const std::string &long_name,
                                        const char &short_name,
                                        const std::string &desc) {
  options.push_back(new StringOption(long_name, short_name, desc));
}

void
CommandlineInterface::add_boolean_option(const std::string &long_name,
                                         const char &short_name,
                                         const std::string &desc,
                                         const bool default_value) {
  options.push_back(new BooleanOption(long_name, short_name, desc,
                                      default_value));
}

void
CommandlineInterface::add_size_option(const std::string &long_name,
                                      const char &short_name,
                                      const std::string &desc,
                                      const size_t default_value) {
  options.push_back(new SizeOption(long_name, short_name, desc,
                                   default_value));
}

void
CommandlineInterface::add_stringlist_option(const string &long_name,
                                            const char &short_name,
                                            const string &descr,
                                            const string default_val,
                                            const string sep) {
  options.push_back(new StringlistOption(long_name, short_name, descr,
                                         default_val, sep));
}

/**
 * consume one or more arguments from the command line given. The results are
 * placed, in the order specified, into the back of the vector args. They
 * are not actually removed from the command line (TODO: maybe they should be?)
 */
void
CommandlineInterface::consume(Commandline &cmdline, const vector<int> &indexes,
                              vector<string> &args) {
  if ((cmdline.num_arguments() > this->max_args) ||
      (cmdline.num_arguments() < this->min_args)) {
    std::stringstream ss;
    ss << "command line contained " << cmdline.num_arguments() << " arguments; "
       << "expected between " << this->min_args << " and " << this->max_args;
    throw OptionError(ss.str());
  }
  for (size_t i = 0; i < indexes.size(); ++i) {
    args.push_back(cmdline.get_argument(indexes[i]));
  }
}

void
CommandlineInterface::consume(Commandline &cmdline, const int idx,
                              string &dest) {
  if ((cmdline.num_arguments() > this->max_args) ||
      (cmdline.num_arguments() < this->min_args)) {
    std::stringstream ss;
    ss << "command line contained " << cmdline.num_arguments() << " arguments; "
       << "expected between " << this->min_args << " and " << this->max_args;
    throw OptionError(ss.str());
  }
  dest = cmdline.get_argument(idx);
}

string
CommandlineInterface::usage() const {
  std::stringstream ss;
  ss << this->program_name << std::endl;
  ss << this->program_description << std::endl;

  size_t longest_name_length = 0;
  for (auto it = this->options.begin(); it != options.end(); ++it) {
    Option* opt_ptr = (*it);
    if (opt_ptr->get_long_name().size() > longest_name_length)
      longest_name_length = opt_ptr->get_long_name().size();
  }

  ss << std::endl << "options" << std::endl;
  for (auto it = this->options.begin(); it != options.end(); ++it) {
    Option* opt_ptr = (*it);
    ss << "-" << opt_ptr->get_short_name() << " ";
    if (opt_ptr->is_required())
      ss << "(required)";
    else
      ss << "          ";
    string padding(longest_name_length - opt_ptr->get_long_name().size(), ' ');
    ss << " --" << opt_ptr->get_long_name() << padding;
    ss << " " << opt_ptr->get_description() << std::endl;
  }

  return ss.str();
}

/*****************************************************************************
 *                             Commandline CLASS                             *
 *****************************************************************************/

Commandline::Commandline(int argc, const char* argv[]) {
  string option_name;
  for (size_t i = 1; i < argc; ++i) {
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
        option_name = "";
      }
    }
  }
}


/**
 * TODO: fix nasty O(n) complexity; instances can be in set, no need for vector.
 */
bool
Commandline::has_option(const Option &option) const {
  for (auto it = this->op_insts.begin(); it != this->op_insts.end(); ++it) {
    if (it->get_name() == string(1, option.get_short_name()) ||
        it->get_name() == option.get_long_name()) {
      return true;
    }
  }
  return false;
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
    if (it->get_name() == string(1, option.get_short_name()) ||
        it->get_name() == option.get_long_name()) {
      res = (*it);
      found = true;
      break;
    }
  }

  if (!found)
    throw OptionError("No such option: " + option.get_long_name());

  this->op_insts.erase(it);
  return res;
}
