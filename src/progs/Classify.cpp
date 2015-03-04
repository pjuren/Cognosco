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

int
main(int argc, const char* argv[]) {
  try {
    if (argc != 5) {
      cout << "USAGE: ./classify training.dat testing.data class_label "
           << "positive_class_value" << endl;
    } else {
      Dataset train, test;
      CSVLoader csv_loader(" ");
      csv_loader.load(argv[1], train);
      csv_loader.load(argv[2], test);
      const string class_label(argv[3]);
      const string pos_class_val(argv[4]);

      NaiveBayes nb;
      nb.learn(train, class_label);

      for (Dataset::const_iterator inst = test.begin();
           inst != test.end(); ++inst) {
        cerr << "prob: "
             << nb.membership_probability(*inst, pos_class_val) << endl;
      }
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
