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
#include <cstdlib>
#include <iostream>
#include <cassert>

// local Cognosco includes -- core
#include "Dataset.hpp"
#include "CognoscoError.hpp"
// local Cognosco includes -- ui
#include "CLI.hpp"
// local Cognosco includes -- io
#include "CSVLoader.hpp"
// local Cognosco includes -- classifiers
#include "NaiveBayes.hpp"
#include "KMedoidsClassifier.hpp"
#include "DecisionStump.hpp"
#include "Random.hpp"
#include "ZeroR.hpp"

// bring these into the current namespace..
using std::set;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

/*****************************************************************************
 *                            CLASSIFIER FACTORY                             *
 *****************************************************************************/

static Classifier*
build_classifier(const string &name) {
  if (name == "NaiveBayes") return new NaiveBayes();
  if (name == "KMedoids") return new Classifiers::KMedoids();
  if (name == "DecisionStump") return new Classifiers::DecisionStump();
  if (name == "Random") return new Classifiers::Random();
  if (name == "ZeroR") return new Classifiers::ZeroR();
  throw CognoscoError("Unknown classifier type: " + name);
}

static Classifier*
build_classifier(const string &name, const MisclassificationCostMatrix &m) {
  if (name == "NaiveBayes") return new NaiveBayes(m);
  if (name == "KMedoids") return new Classifiers::KMedoids(m);
  if (name == "DecisionStump") return new Classifiers::DecisionStump(m);
  if (name == "Random") return new Classifiers::Random(m);
  if (name == "ZeroR") return new Classifiers::ZeroR(m);
  throw CognoscoError("Unknown classifier type: " + name);
}

static void
destroy_classifier(Classifier* c_ptr) {
  delete c_ptr;
}

/*****************************************************************************
 *                              CLASSIFICATION                               *
 *****************************************************************************/

/**
 * output an instance including all its attributes and one extra column for
 * its positive class probability.
 */
static void
output_classification(const Instance &inst, const Classifier &clsfr,
                      const vector<string> &att_names,
                      const string &pos_class_val,
                      const set<string> &exclude_atts) {
  for (size_t j = 0; j < att_names.size(); j++) {
    if (j != 0) cout << "\t";
    cout << inst.get_att_occurrence(att_names[j])->to_string();
  }
  cout << "\t" << clsfr.class_probability(inst, pos_class_val,
                                          exclude_atts) << endl;
}

static void
output_classification(const Dataset &d, const Classifier &clsfr,
                      const string &pos_class_val,
                      const set<string> &exclude_atts) {
  vector<string> attribute_names;
  for (Dataset::const_attribute_iterator it = d.begin_attributes();
       it != d.end_attributes(); ++it) {
    attribute_names.push_back((*it)->get_name());
  }
  for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
    output_classification(*inst, clsfr, attribute_names,
                          pos_class_val, exclude_atts);
  }
}


/*****************************************************************************
 *                             CROSS-VALIDATION                              *
 *****************************************************************************/

/**
 * \param ex_atts_with_id_val if not empty string, any attributes that have the
 *                            same name as the value of this attribute for the
 *                            instances held out in in the current fold will
 *                            not be used for training. For example, this might
 *                            be an ID attribute like 'instance_name'.
 */
static void
k_fold_cross_validation(Classifier *clsfr, const Dataset &d,
                        const size_t k, const string &class_label,
                        const string &pos_class_val,
                        const set<string> &exclude_atts,
                        const string &ex_atts_with_id_val = "",
                        const bool VERBOSE = false) {
  cerr << "creating splitter" << endl;
  DatasetSplitter x_validation_splitter(k);
  cerr << "splitting dataset" << endl;
  DatasetSplit instances_in_fold(x_validation_splitter.split(d, class_label));

  cerr << "extracting attributes" << endl;
  vector<string> att_names;
  for (auto it = d.begin_attributes(); it != d.end_attributes(); ++it) {
    Attribute* att_ptr = (*it );
    att_names.push_back(att_ptr->get_name());
  }

  cerr << "processing folds" << endl;
  // for each fold, learn the classifier and output
  for (size_t fold_num = 0; fold_num < k; ++fold_num) {
    clsfr->clear();

    set<size_t> exclude_insts(instances_in_fold[fold_num].begin(),
                              instances_in_fold[fold_num].end());
    set<string> exclude_atts_expanded(exclude_atts.begin(), exclude_atts.end());
    if (!ex_atts_with_id_val.empty()) {
      for (auto e_i_it = exclude_insts.begin(); e_i_it != exclude_insts.end(); ++e_i_it) {
        string att_val = d[*e_i_it][ex_atts_with_id_val]->to_string();
        if (d.has_attribute(att_val)) {
          cerr << "adding " << att_val << " to excluded attributes" << endl;
          exclude_atts_expanded.insert(att_val);
        }
      }
    }
    clsfr->learn(d, class_label, exclude_insts, exclude_atts_expanded);
    if (VERBOSE)
      cerr << clsfr->to_string() << endl;
    for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
      if (exclude_insts.find(inst->get_instance_id()) != exclude_insts.end()) {
        output_classification(*inst, *clsfr, att_names,
                              pos_class_val, exclude_atts_expanded);
      }
    }
  }
}

/**
 * \param ex_atts_with_id_val if not empty string, any attributes that have the
 *                            same name as the value of this attribute for the
 *                            instances held out in in the current fold will
 *                            not be used for training. For example, this might
 *                            be an ID attribute like 'instance_name'.
 */
static void
leave_one_out_cross_validation(Classifier *clsfr, const Dataset &d,
                               const size_t k, const string &class_label,
                               const string &pos_class_val,
                               const set<string> &exclude_atts,
                               const string &ex_atts_with_id_val = "",
                               const bool VERBOSE = false) {
  cerr << "extracting attributes" << endl;
  vector<string> att_names;
  for (auto it = d.begin_attributes(); it != d.end_attributes(); ++it) {
    Attribute* att_ptr = (*it );
    att_names.push_back(att_ptr->get_name());
  }

  for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
    set<string> exclude_atts_expanded(exclude_atts.begin(), exclude_atts.end());
    if (!ex_atts_with_id_val.empty()) {
      string att_val = (*inst)[ex_atts_with_id_val]->to_string();
      if (d.has_attribute(att_val)) {
        cerr << "adding " << att_val << " to excluded attributes" << endl;
        exclude_atts_expanded.insert(att_val);
      }
    }

    set<size_t> exclude_insts;
    exclude_insts.insert(inst->get_instance_id());
    clsfr->clear();
    clsfr->learn(d, class_label, exclude_insts, exclude_atts_expanded);
    output_classification(*inst, *clsfr, att_names,
                          pos_class_val, exclude_atts_expanded);
  }
}

/*****************************************************************************
 *                         UI AND MAIN ENTRY POINT                           *
 *****************************************************************************/

static CommandlineInterface
get_cli(const string &prog_name) {
  const size_t MIN_ARGS = 1;
  const size_t MAX_ARGS = 2;
  const vector<bool> reqd_args_bit_mask{true};
  CommandlineInterface cli (prog_name, "for classifying stuff...",
                            MIN_ARGS, MAX_ARGS);
  cli.add_boolean_option("verbose", 'v', "output additional status messages "
                         "during run to stderr", false);
  cli.add_string_option("classifier", 'c', "classifier to learn",
                        set<string>{"NaiveBayes"});
  cli.add_string_option("cross-validation", 'r', "type of cross-validation "
                        "to use", set<string>{"stratified_ten_fold",
                        "hold-one-out"}, "stratified_ten_fold");
  cli.add_string_option("class-attribute", 'a', "name of the attribute to use "
                        "as class");
  cli.add_string_option("positive-value", 'p', "value for the class attribute "
                        "that should be considered the positive class");
  cli.add_string_option("misclassification-cost-matrix", 'm', "NxN matrix "
                        "specifying relative cost of clasifying class A as "
                        "class B. Example: "
                        "\"good:poor:4 poor:good:1 poor:poor:0 good:good:1\"",
                        "");
  cli.add_boolean_option("whitespace-sep", 'w', "treat input files as having "
                         "whitespace-separated fields, rather than comma "
                         "separated", false);
  cli.add_stringlist_option("exclude-attributes", 'e', "do not use these "
                            "attribtues for model training; if more than one, "
                            "provide as a quoted comma-separated list", "");
  cli.add_string_option("exclude-att-val", 'x', "do not use attributes "
                        "for training if their name is the same as the "
                        "value of this attribute in a held-out instance",
                        "");
  return cli;
}

int
main(int argc, const char* argv[]) {
  try {
    bool VERBOSE = true;
    bool whitespace_sep;
    string classifier;
    string cross_validation_method;
    string class_attribute_name;
    string positive_class_value;
    string training_dataset_fn;
    string testing_dataset_fn = "";
    set<string> exclude_atts;
    string exclude_att_val = "";
    string misclass_matr_str;

    // process general options/arguments from command line.
    CommandlineInterface cli (get_cli(argv[0]));
    Commandline cmdline (argc, argv);
    Classifier *clsfr = NULL;
    try {
      cli.consume('v', cmdline, VERBOSE);
      cli.consume('c', cmdline, classifier);
      cli.consume('r', cmdline, cross_validation_method);
      cli.consume('a', cmdline, class_attribute_name);
      cli.consume('p', cmdline, positive_class_value);
      cli.consume('w', cmdline, whitespace_sep);
      cli.consume('e', cmdline, exclude_atts);
      cli.consume('x', cmdline, exclude_att_val);
      cli.consume('m', cmdline, misclass_matr_str);
      cli.consume(cmdline, 0, training_dataset_fn);
      if (cmdline.num_arguments() > 1) {
        cli.consume(cmdline, 1, testing_dataset_fn);
      }

      if (misclass_matr_str.empty()) {
        clsfr = build_classifier(classifier);
      } else {
        MisclassificationCostMatrix m (misclass_matr_str);
        clsfr = build_classifier(classifier, m);
      }

      cerr << "calling set specific.." << endl;
      clsfr->set_classifier_specific_options(cmdline);

      if (VERBOSE) {
        cerr << "running with options: " << endl;
        cerr << "\tVerbose: " << VERBOSE << endl;
        cerr << "\tseparate input fields on whitespace? " << whitespace_sep << endl;
        cerr << "\tcross-val method: " << cross_validation_method << endl;
        cerr << "\tclasifier: " << classifier << endl;
        cerr << "\tclass_att_name: " << class_attribute_name << endl;
        cerr << "\tpos val: " << positive_class_value << endl;
        cerr << "\tattributes to exclude from training: "
             << join(exclude_atts, ",") << endl;
        cerr << "\ttrain fn: " << training_dataset_fn << endl;
        cerr << "\ttest fn: " << testing_dataset_fn << endl;
        cerr << endl;
      }
    } catch (const OptionError &e) {
      cerr << e.what() << endl << endl;
      cerr << cli.usage() << endl << endl;
      if (clsfr != NULL)
        cerr << clsfr->usage() << endl << endl;
      return EXIT_FAILURE;
    }

    // prepare for loading
    string sep = ",";
    if (whitespace_sep) sep = "\t";
    CSVLoader csv_loader(sep);


    if (testing_dataset_fn.empty()) {
      cerr << "got here" << endl;
      Dataset d;
      csv_loader.load(training_dataset_fn, d, VERBOSE);

      assert(cross_validation_method == "stratified_ten_fold" ||
             cross_validation_method == "hold-one-out");
      if (cross_validation_method == "stratified_ten_fold")
        k_fold_cross_validation(clsfr, d, 10, class_attribute_name,
                                positive_class_value, exclude_atts,
                                exclude_att_val, VERBOSE);
      else
        leave_one_out_cross_validation(clsfr, d, 10, class_attribute_name,
                                       positive_class_value, exclude_atts,
                                       exclude_att_val, VERBOSE);
    } else {
      Dataset train, test;
      csv_loader.load(training_dataset_fn, train, VERBOSE);
      csv_loader.load(testing_dataset_fn, test, VERBOSE);
      set<size_t> exclude_insts;
      clsfr->learn(train, class_attribute_name, exclude_insts, exclude_atts);
      cerr << clsfr->to_string() << endl;
      output_classification(test, *clsfr, positive_class_value, exclude_atts);
    }

    // we're done... cleanup
    destroy_classifier(clsfr);
  } catch (const CognoscoError &e) {
    cerr << "ERROR:\t" << e.what() << endl;
    return EXIT_FAILURE;
  } catch (std::bad_alloc &ba) {
    cerr << "ERROR: could not allocate memory" << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
