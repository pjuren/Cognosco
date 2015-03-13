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
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <set>

// local inludes
#include "CognoscoError.hpp"
#include "StringUtils.hpp"

/**
 * This file defines the command line parsing machinery. There are a number of
 * classes here, but they're basically three types: Classes for and specifying
 * an interface itself, classes for modeling a command line, and those for
 * linking the two. A Producer-consumer paradigm is used here. When a
 * command line is parsed, a CommandLine object is created; Interfaces can
 * then consume arguments and options from it.
 */

// prototypes
class Commandline;
class OptionInstance;

/*****************************************************************************
 *                              ERROR HANDLING                               *
 *****************************************************************************/

typedef CognoscoError OptionError;

/*****************************************************************************
 *                      CLASSES FOR MODELLING OPTIONS                        *
 *****************************************************************************/

class Option {
public:
  // constructors and destructors
  Option(const char short_name, const std::string &long_name,
         const std::string &descr, const bool required) :
            short_name(short_name),
            long_name(long_name),
            description(descr),
            required(required) {}
  Option(const char short_name, const std::string &long_name,
         const std::string &descr) :
            short_name(short_name),
            long_name(long_name),
            description(descr),
            required(false) {}
  Option(const Option &o) : short_name(o.short_name), long_name(o.long_name),
                            description(o.description), required(o.required) {};
  virtual ~Option() {};

  // inspectors
  const char &get_short_name() const {return this->short_name;}
  const std::string &get_long_name() const {return this->long_name;}
  const std::string &get_description() const {return this->description;}
  bool is_required() const {return this->required;}

  // parsers -- derived classes can overrride these to provide functionality
  virtual void parse(const OptionInstance& inst, bool &dest) const {
    throw OptionError("Not implemented");
  };
  virtual void parse(const OptionInstance& inst, std::string &dest) const {
    throw OptionError("Not implemented");
  };
  virtual void parse(const OptionInstance& inst, int &dest) const {
    throw OptionError("Not implemented");
  };
  virtual void parse(const OptionInstance& inst, double &dest) const {
    throw OptionError("Not implemented");
  };
  virtual void parse(const OptionInstance& inst, size_t &dest) const {
    throw OptionError("Not implemented");
  };

  // for setting default values
  // -- derived class override to provide functionality
  virtual void parse_default(bool &dst) const {
    std::stringstream ss;
    ss << "conversion to bool for default value of option -" << short_name
       << " not possible";
    throw OptionError(ss.str());
  }
  virtual void parse_default(std::string &dest) const {
    std::stringstream ss;
    ss << "conversion to string for default value of option -" << short_name
       << " not possible";
    throw OptionError(ss.str());
  };
  virtual void parse_default(int &dest) const {
    std::stringstream ss;
    ss << "conversion to integer for default value of option -" << short_name
       << " not possible";
    throw OptionError(ss.str());
  };
  virtual void parse_default(double &dest) const {
    std::stringstream ss;
    ss << "conversion to double for default value of option -" << short_name
       << " not possible";
    throw OptionError(ss.str());
  };
  virtual void parse_default(size_t &dest) const {
    std::stringstream ss;
    ss << "conversion to size_t for default value of option -" << short_name
       << " not possible";
    throw OptionError(ss.str());
  };
private:
  char short_name;
  std::string long_name;
  std::string description;
  bool required;
};

class StringOption : public Option {
public:
  StringOption(const std::string &long_name,
               const char short_name,
               const std::string &descr,
               const std::string &default_val,
               const std::set<std::string> &acceptable) :
                    Option(short_name, long_name, descr),
                    accpt_vals(acceptable),
                    has_default(true),
                    default_val(default_val) {};
  StringOption(const std::string &long_name,
               const char short_name,
               const std::string &descr,
               const std::set<std::string> &acceptable) :
                    Option(short_name, long_name, descr, true),
                    accpt_vals(acceptable),
                    has_default(false),
                    default_val("") {};
  StringOption(const std::string &long_name,
               const char short_name,
               const std::string &descr) :
                    Option(short_name, long_name, descr, true),
                    accpt_vals(std::set<std::string>()),
                    has_default(false),
                    default_val("") {};
  ~StringOption() {}

  void parse(const OptionInstance& inst, std::string &dest) const;
  void parse_default(std::string &dest) const;
private:
  std::set<std::string> accpt_vals; // if empty, anything acceptable
  bool has_default;
  std::string default_val;
};

class BooleanOption : public Option {
public:
  BooleanOption(const std::string &long_name,
                const char short_name,
                const std::string &descr) :
                    Option(short_name, long_name, descr, true),
                    has_default(false), default_val(false) {};
  BooleanOption(const std::string &long_name,
               const char short_name,
               const std::string &descr,
               const bool default_val) :
                    Option(short_name, long_name, descr),
                    has_default(true), default_val(default_val) {};
  ~BooleanOption() {}

  void parse(const OptionInstance& inst, bool &dest) const;
  void parse_default(bool &dest) const;
private:
  bool has_default;
  bool default_val;
};

class IntegerOption : public Option {
public:
  IntegerOption(const std::string &long_name,
                const char short_name,
                const std::string &descr,
                const bool required,
                const std::set<int> &acceptable) :
                    Option(short_name, long_name, descr, required),
                    accpt_vals(acceptable),
                    min_val(std::numeric_limits<int>::min()),
                    max_val(std::numeric_limits<int>::max()) {};
  IntegerOption(const std::string &long_name,
                const char short_name,
                const std::string &descr,
                const bool required,
                const int &min_v,
                const int &max_v) :
                    Option(short_name, long_name, descr, required),
                    accpt_vals(std::set<int>()),
                    min_val(min_v),
                    max_val(max_v) {};
  void parse(const OptionInstance& inst, int &dest) const;
  void parse(const OptionInstance& inst, double &dest) const;
  void parse(const OptionInstance& inst, std::string &dest) const;
private:
  std::set<int> accpt_vals; // if empty, anything in range is acceptable
  int min_val;
  int max_val;
};

class SizeOption : public Option {
public:
  SizeOption(const std::string &long_name,
             const char short_name,
             const std::string &descr,
             const std::set<size_t> &acceptable) :
                    Option(short_name, long_name, descr, true),
                    accpt_vals(acceptable),
                    has_default(false),
                    default_val(0),
                    min_val(std::numeric_limits<size_t>::min()),
                    max_val(std::numeric_limits<size_t>::max()) {};
  SizeOption(const std::string &long_name,
             const char short_name,
             const std::string &descr,
             const size_t default_val) :
                  Option(short_name, long_name, descr, false),
                  accpt_vals(std::set<size_t>()),
                  has_default(true),
                  default_val(default_val),
                  min_val(std::numeric_limits<size_t>::min()),
                  max_val(std::numeric_limits<size_t>::max()) {};
  SizeOption(const std::string &long_name,
                const char short_name,
                const std::string &descr,
                const int &min_v,
                const int &max_v) :
                    Option(short_name, long_name, descr, true),
                    accpt_vals(std::set<size_t>()),
                    has_default(false),
                    default_val(0),
                    min_val(min_v),
                    max_val(max_v) {};
  void parse(const OptionInstance& inst, size_t &dest) const;
  // TODO-- later...
  //void parse(const OptionInstance& inst, int &dest) const;
  //void parse(const OptionInstance& inst, double &dest) const;
  //void parse(const OptionInstance& inst, std::string &dest) const;
  void parse_default(size_t &dest) const;
private:
  std::set<size_t> accpt_vals; // if empty, anything in range is acceptable
  bool has_default;
  size_t default_val;
  size_t min_val;
  size_t max_val;
};



/*****************************************************************************
 *             CLASSES FOR MODELLING THE COMMAND LINE ITSELF                 *
 *****************************************************************************/



/**
 * An option instance is an actual occurrence on a real command line. For
 * example: -t 0.5
 * This class just wraps the name and the string representation of the value;
 * it knows nothing about what option this occurrence should map to, or the
 * type, range of acceptable values, etc.
 */
class OptionInstance {
public:
  // constructors
  OptionInstance() : name(""), string_val("") {};
  OptionInstance(const std::string &name, const std::string &val) :
    name(name), string_val(val) {};

  // inspectors
  const std::string& get_name() const {return this->name;}
  const std::string& get_value() const {return this->string_val;}

private:
  std::string name;
  std::string string_val;
};

/**
 * A Commandline object wraps the options and arguments that occur on a single
 * command line instance. Options and arguments can be consumed by interfaces.
 * This class is responsible for parsing the raw tokens into OptionInstances
 * and arguments. It is also responsible for matching these to Option objects,
 * which describe the properties of the options, when they're being consumed.
 */
class Commandline {
public:
  Commandline() {};
  Commandline(int argc, const char* argv[]);
  OptionInstance consume_option(const Option &option);
  bool has_option(const Option &option) const;
  size_t num_arguments() const {return this->arguments.size();}

  const std::string &get_argument(const size_t i) {
    if (i >= this->arguments.size()) {
      std::stringstream ss;
      ss << "trying to get argument number " << i
         << " but cmd line only had " << this->arguments.size() << " args: "
         << join(this->arguments, ", ");
      // TODO shouldn't be option error.. not an option...
      throw OptionError(ss.str());
    }
    return arguments[i];
  }
private:
  std::vector<std::string> arguments;
  std::vector<OptionInstance> op_insts;
};



/*****************************************************************************
 *                       CommandlineInterface CLASS                          *
 *****************************************************************************/

class CommandlineInterface {
public:
  // constructors and destructors
  CommandlineInterface(const std::string &program_name,
                       const std::string &description);
  CommandlineInterface(const std::string &program_name,
                       const std::string &description,
                       const size_t min_args, const size_t max_args);
  ~CommandlineInterface() {
    for (size_t i = 0; i < this->options.size(); ++i) delete options[i];
  }
  CommandlineInterface(const CommandlineInterface &cli) :
    program_name(cli.program_name), program_description(cli.program_description),
    min_args(cli.min_args), max_args(cli.max_args) {
    for (size_t i = 0; i < cli.options.size(); ++i) {
      this->options.push_back(new Option(*cli.options[i]));
    }
  }

  // inspectors
  std::string usage() const;

  // mutators for adding options
  void add_integer_option(const std::string &long_name,
                          const char &short_name,
                          const std::string &desc,
                          const std::set<int> &acceptable_vals,
                          const int default_value,
                          const bool required);
  void add_string_option(const std::string &long_name,
                         const char &short_name,
                         const std::string &desc,
                         const std::set<std::string> &acceptable_vals,
                         const std::string default_value);
  void add_string_option(const std::string &long_name,
                         const char &short_name,
                         const std::string &desc,
                         const std::set<std::string> &acceptable_vals);
  void add_string_option(const std::string &long_name,
                         const char &short_name,
                         const std::string &desc);
  void add_boolean_option(const std::string &long_name,
                          const char &short_name,
                          const std::string &desc,
                          const bool default_value);
  void add_size_option(const std::string &long_name,
                       const char &short_name,
                       const std::string &desc,
                       const size_t default_value);

  // consuming options from a command line
  template<class T>
  void consume(const char short_name, Commandline &cmdline, T &destination) {
    // TODO fix nasty O(n) here later...; replace vector with map
    Option *ptr_to_cli_option = NULL;

    for (auto it = this->options.begin(); it != this->options.end(); ++it) {
      Option *c_op_ptr = (*it);
      if (c_op_ptr->get_short_name() == short_name)
        ptr_to_cli_option = c_op_ptr;
    }

    if (ptr_to_cli_option == NULL) {
      std::stringstream ss;
      ss << "No such option in command line interface: " << short_name;
      throw OptionError(ss.str());
    } else {
      if (cmdline.has_option(*ptr_to_cli_option)) {
        OptionInstance inst = cmdline.consume_option(*ptr_to_cli_option);
        ptr_to_cli_option->parse(inst, destination);
      } else {
        if (ptr_to_cli_option->is_required()) {
          std::stringstream ss;
          ss << "Missing required option -" << short_name;
          throw OptionError(ss.str());
        } else {
          ptr_to_cli_option->parse_default(destination);
        }
      }
    }
  }

  // consuming arguments from a command line
  void consume(Commandline &cmdline, const std::vector<int> &indexes,
               std::vector<std::string> &args);
  void consume(Commandline &cmdline, const int idx, std::string &dest);


private:
  std::string program_name;
  std::string program_description;
  size_t min_args;
  size_t max_args;
  std::vector<Option*> options;
};





#endif
