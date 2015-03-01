/* The following applys to this software package and all subparts therein
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

#ifndef KMEDOIDS_HPP_
#define KMEDOIDS_HPP_

// stl includes
#include <set>
#include <string>
#include <vector>

// Cognosco includes
#include "clustering.hpp"

class KMedoidsClusterer {
public:
  // constructors
  KMedoidsClusterer(const size_t k, const DistanceMatrix &dist_m,
                    const std::vector<std::string> &instance_ids);

  // public inspectors
  const std::set<std::string>&  get_medoids() const;
  std::set<std::string> get_medoid_members(const std::string medoid) const;
  std::string get_cluster_assignment(const std::string &instance_name) const;
  double compute_membership_probability(const std::string &instance,
                                        const std::string &medoid) const;
  std::string find_closest_medoid(const std::string &instance_name);
  double get_distance(const std::string &s, const std::string &t);

  // public mutators
  void train();

private:
  // private inspectors
  double cost();
  bool is_medoid(const std::string &id) const;

  // private mutators
  void swap_medoid(const std::string &non_medoid, const std::string &medoid);
  void update_cluster_assignments();
  void assign_to_medoid(const std::string &instance, const std::string medoid);

  // private instance variables
  std::set<std::string> medoids;
  DistanceMatrix distance_matrix;
  vector<string> instance_ids;
  unordered_map<string, string> cluster_assignments;
};

#endif
