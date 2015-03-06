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
#include <vector>
#include <string>
#include <sstream>
#include <set>

// local Cognosco includes
#include "Dataset.hpp"
#include "DistanceMatrix.hpp"
#include "PairwiseDistanceLoader.hpp"
#include "CognoscoError.hpp"
#include "KMedoids.hpp"

// bring these into the current namespace..
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;

int
main(int argc, const char* argv[]) {
  try {
    if (argc != 3) {
      cout << "USAGE: ./cluster num_clusters distance_matrix.dat" << endl;
    } else {
      // get k
      int k;
      try {
        k = std::stoi(argv[1]);
      } catch (const std::invalid_argument &e) {
        std::stringstream ss;
        ss << "not a valid value for number of clusters: " << argv[2] << endl;
        throw CognoscoError(ss.str());
      }

      // load distance matrix
      DistanceMatrix d;
      PairwiseDistanceLoader loader;
      loader.load(argv[2], d);

      set<string> instance_ids_s;

      for(auto kv : d) instance_ids_s.insert(kv.first.first);
      vector<string> instance_ids;
      instance_ids.reserve(instance_ids_s.size());
      for (set<string>::iterator it = instance_ids_s.begin(); it != instance_ids_s.end(); ++it) {
        instance_ids.push_back(*it);
      }
      //std::cerr << "got " << instance_ids.size() << " instance ids from distance matrix" << endl;

      KMedoidsClusterer clstr(k, d, instance_ids);

      set<string> m(clstr.get_medoids());
      vector<string> medoids(m.begin(), m.end());
      for (size_t i = 0; i < instance_ids.size(); ++i) {
        cout << instance_ids[i] << "\t";
        for (size_t j = 0; j < medoids.size(); ++j) {
          double dist(clstr.get_distance(instance_ids[i], medoids[j]));
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
