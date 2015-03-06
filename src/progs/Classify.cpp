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
#include <cstdlib>
#include <iostream>

// local Cognosco includes
#include "Dataset.hpp"
#include "CSVLoader.hpp"
#include "CognoscoError.hpp"
#include "NaiveBayes.hpp"

// bring these into the current namespace..
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
output_classification(const Instance &inst, const NaiveBayes &nb) {
  for (size_t j = 0; j < attribute_names.size(); j++) {
    if (j != 0) cout << "\t";
    cout << inst->get_att_occurrence(attribute_names[j])->to_string();
  }
  cout << "\t" << nb.membership_probability(*inst, pos_class_val) << endl;
}

/**
 * for a given attribute, count the number of times each of its values appears
 * in each fold.
 *
 * \return unordered_map indexed first by attribute-value, then by fold number
 *         the value being the count of occurrences for that label in that
 *         fold.
 */
unordered_map<string, vector<size_t> >
count_occurrences (const Dataset &d, const string class_label,
                   const vector<vector<size_t> > &instances_in_fold) {
  unordered_map<string, vector<size_t> > res;

  for (size_t k = 0; k < instances_in_fold.size(); ++k) {
    for (size_t j = 0; j < instances_in_fold[k].size(); ++k) {
      const string inst_label = d[instances_in_fold[k][j]][class_label];
      if (res.find(inst_label) == res.end())
        res[inst_label].resize(instances_in_fold.size());
      res[inst_label][k] += 1;
    }
  }

  return res;
}


/**
 * for a given attribute name, compute the ideal number of times each of its
 * values should appear per-fold in k folds if they are stratified by this
 * aatribute.
 */
unordered_map<string, vector<size_t> >
compute_ideal_split(const Dataset &d, const string &att_name,
                    const size_t k = 10) {
  unordered_map<string, vector<size_t> > r_count;
  for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
    r_count[inst[att_name].resize(k)
    for (size_t j = 0; j < k; ++j) r_count[inst[att_name][j] += 1
  }
  for (unordered_map<string, double>::iterator it = res.begin();
       it != r_count.end(); ++it) {
    for (size_t j = 0; j < k; ++j) it->second[j] /= k;
  }
  return r_count;
}

/**
 * compute distnce between two sets of attribute counts
 */
unordered_map<string, vector<size_t> >
dist(const unordered_map<string, vector<size_t> > &a, const unordered_map<string, vector<size_t> > &b) {
  // do shit...

  // these should all be in Dataset ... only one that should be expoosed is the
  // stratify_folds function
}

swap(vector<vector<size_t> > &instances_in_fold, i, j, k, l) {
  // swap the ith instance in fold j with the kth instance in fold l
}

random_swap(vector<vector<size_t> > &instances_in_fold) {
  // randomly pick two folds and two instances within them and swap them.
}

/**
 * shuffle assignment of instances to folds to distirbute classes evenly to
 * each one.
 */
void
stratify_folds(const Dataset &d,
               const vector<vector<size_t> > &instances_in_fold,
               const string &class_label) {

}

void
k_fold_cross_validation(const Dataset &d, const size_t k) {
  DatasetSplit instances_in_fold;
  instances_in_fold.resize(k);
  size_t i = 0;
  vector<size_t> instance_ids (d.get_instance_ids());
  while (i < d.size()) instances_in_fold[i % k].push_back(instance_id[i]);

  // for each fold, learn the classifier and output
  for (size_t fold_num = 0; fold_num < k, ++fold_num) {
    NaiveBayes nb;
    nb.learn(d, class_label, instances_in_fold[k]);
    for (Dataset::const_iterator inst = d.begin(); inst != d.end(); ++inst) {
      if (instances_in_fold[k].find(inst->get_instance_id()) !=
          instances_in_fold[k].end()) {
        output_classification(inst, nb);
      }
    }
  }
}

int
main(int argc, const char* argv[]) {
  try {
    const bool VERBOSE = true;

    if (argc == 4) {

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
