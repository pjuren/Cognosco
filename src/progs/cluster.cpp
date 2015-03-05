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
#include "clustering.hpp"
#include "PairwiseDistanceLoader.hpp"
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
      cout << "USAGE: ./cluster num_clusters distance_matrix.dat" << endl;
    } else {
      DistanceMatrix d;
      PairwiseDistanceLoader loader();
      loader.load(argv[1], d);
      int k(stoi(argv[2]));
      vector<string> instance_ids(d.get_instance_ids());
      KMedoidsClusterer clstr(k, d, instance_ids);

      set<string> medoids(clstr.get_medoid_members());
      for (size_t i = 0; i < instance_ids.size(); ++i) {
        cout << instance_ids[i] << "\t";
        for (size_t j = 0; j < medoids.size(); ++j) {
          double dist(clstr.get_distance(instance_ids[i], medoids[j]);
          cout << dist << "\t";
        }
        cout << endl;
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
