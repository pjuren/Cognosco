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

// local Cognosco includes
#include "Dataset.hpp"
#include "CSVLoader.hpp"
#include "CognoscoError.hpp"
#include "NaiveBayes.hpp"

// bring these into the current namespace..
using std::set;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

/**
 * output an instance including all its attributes and one extra column for
 * its positive class probability.
 */
void
output_classification(const Instance &inst, const NaiveBayes &nb,
                      const vector<string> &att_names,
                      const string &pos_class_val) {
  for (size_t j = 0; j < att_names.size(); j++) {
    if (j != 0) cout << "\t";
    cout << inst.get_att_occurrence(att_names[j])->to_string();
  }
  cout << "\t" << nb.membership_probability(inst, pos_class_val) << endl;
}


void
k_fold_cross_validation(const Dataset &d, const size_t k,
                        const string &class_label,
                        const string &pos_class_val) {
  cerr << "creating splitter" << endl;
  DatasetSplitter x_validation_splitter(k);
  cerr << "splitting dataset" << endl;
  DatasetSplit instances_in_fold(x_validation_splitter.split(d, class_label));

  cerr << "extracting attributes" << endl;
  vector<string> att_names;
  for (Dataset::const_attribute_iterator it = d.begin_attributes(); it != d.end_attributes(); ++it) {
    Attribute* att_ptr = (*it );
    att_names.push_back(att_ptr->get_name());
  }

  cerr << "processing folds" << endl;
  // for each fold, learn the classifier and output
  for (size_t fold_num = 0; fold_num < k; ++fold_num) {
    NaiveBayes nb;
    //<< "pull out instance IDs for fold " << k << " when vec has length " << instances_in_fold.size();
    set<size_t> exclude(instances_in_fold[fold_num].begin(), instances_in_fold[fold_num].end());
    cerr << "learning NB classifier for fold " << fold_num << endl;
    nb.learn(d, class_label, exclude);
    for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
      if (exclude.find(inst->get_instance_id()) != exclude.end()) {
        output_classification(*inst, nb, att_names, pos_class_val);
      }
    }
  }
}

int
main(int argc, const char* argv[]) {
  try {
    const bool VERBOSE = true;

    if (argc == 4) {
      Dataset d;
      CSVLoader csv_loader("\t");
      csv_loader.load(argv[1], d);
      const string class_label(argv[2]);
      const string pos_class_val(argv[3]);
      k_fold_cross_validation(d, 10, class_label, pos_class_val);
    } else if (argc == 5) {

      Dataset train, test;
      CSVLoader csv_loader("\t");
      csv_loader.load(argv[1], train);
      csv_loader.load(argv[2], test);
      const string class_label(argv[3]);
      const string pos_class_val(argv[4]);

      if (VERBOSE) {
        cerr << "loaded training dataset with attributes: ";
        for (Dataset::const_attribute_iterator it = train.begin_attributes();
             it != train.end_attributes(); ++it) {
          if (it != train.begin_attributes()) cerr << ", ";
          cerr << (*it)->get_name();
        }
        cerr << endl;
      }

      NaiveBayes nb;
      nb.learn(train, class_label);
      cerr << nb.to_string() << endl;

      vector<string> attribute_names;
      for (Dataset::const_attribute_iterator it = test.begin_attributes();
          it != test.end_attributes(); ++it) {
        attribute_names.push_back((*it)->get_name());
      }

      for (Dataset::const_iterator inst = test.begin();
           inst != test.end(); ++inst) {

        for (size_t j = 0; j < attribute_names.size(); j++) {
          if (j != 0) cout << "\t";
          cout << inst->get_att_occurrence(attribute_names[j])->to_string();
        }
        cout << "\t" << nb.membership_probability(*inst, pos_class_val) << endl;
      }
    } else {
      cerr << "USAGE: ./classify training.dat testing.data class_label "
           << "positive_class_value" << endl;
      cerr << "USAGE: ./classify dataset.dat class_label "
           << "positive_class_value" << endl;
      cerr << "first version runs with test and training set, second "
              "perfroms ten-fold cross-validation" << endl;
    }


  } catch (const CognoscoError &e) {
    cerr << "ERROR:\t" << e.what() << endl;
    return EXIT_FAILURE;
  } catch (std::bad_alloc &ba) {
    cerr << "ERROR: could not allocate memory" << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
