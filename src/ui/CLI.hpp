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

#ifndef CLI_HPP_
#define CLI_HPP_

// stl includes
#include <string>
#include <vector>


/*****************************************************************************
 *                               INSPECTORS                                  *
 *****************************************************************************/

class OptionName {

};

class LongOptionName : public OptionName {

};

class ShortOptionName : public OptionName {

};

class Option {
private:
  std::string description;
  LongOptionName long_name;
  ShortOptionName short_name;
};

class StringOption : public Option {

};

class IntegerOption : public Option {

};

class CommandLineInterface {
public:
  CommandLineInterface(const std::string &program_name,
                       const std::string &description,
                       const size_t min_args, const size_t max_args);
  std::string usage() const;

  void add_integer_option(const LongOptionName &long_name,
                          const ShortOptionName &short_name,
                          const std::string &desc);
  void add_string_option(const LongOptionName &long_name,
                         const ShortOptionName &short_name,
                         const std::string &desc);
  void add_boolean_option(const LongOptionName &long_name,
                          const ShortOptionName &short_name,
                          const std::string &desc);

private:
  std::vector<Option*> options;
};


class OptionInstance {
public:
  OptionInstance(const OptionName &name, const std::string &val) :
    name(name), string_val(val) {};
private:
  OptionName name; // might be long, might be short, who knows.. but just one
  std::string string_val;
};


class CommandLineInstance {
public:
  CommandLineInstance(const CommandLineInterface &cli, int argc, const char* argv[]);
private:
  CommandLineInterface interface;
  std::vector<OptionInstance> option_instances;
};



#endif
