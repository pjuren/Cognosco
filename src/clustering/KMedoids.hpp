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


class KMedoidsClusterer {
public:
  // constructors
  KMedoidsClusterer(const size_t k, const DistanceMatrix &dist_m,
                    const vector<string> &instance_ids);

  // public inspectors
  const set<string>&  get_medoids() const;
  set<string> get_medoid_members(const string medoid) const;
  string get_cluster_assignment(const string &instance_name) const;
  double compute_membership_probability(const string &instance,
                                 const string &medoid) const;
  string find_closest_medoid(const string &instance_name);
  double get_distance(const string &s, const string &t);


  // public mutators
  void train();

private:
  // private inspectors
  double cost();
  bool is_medoid(const string &id) const;

  // private mutators
  void swap_medoid(const string &non_medoid, const string &medoid);
  void update_cluster_assignments();
  void assign_to_medoid(const string &instance, const string medoid);

  // private instance variables
  set<string> medoids;
  DistanceMatrix distance_matrix;
  vector<string> instance_ids;
  unordered_map<string, string> cluster_assignments;
};
