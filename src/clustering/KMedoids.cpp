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

// stl includes
#include <string>
#include <vector>

// Cognosco includes
#include "KMedoids.hpp"

// bring these into the name space...
using std::string;
using std::vector;


/*****************************************************************************
 *                              CONSTRUCTORS                                 *
 *****************************************************************************/

KMedoidsClusterer::KMedoidsClusterer(const size_t k,
                                     const DistanceMatrix &dist_m,
                                     const vector<string> &instance_ids) :
  distance_matrix(dist_m), instance_ids(instance_ids) {
  // randomly assign k of the instances as medoids
  while (this->medoids.size() < k) {
    int r = rand() % instance_ids.size();
    if (medoids.find(instance_ids[r]) == medoids.end())
      medoids.insert(instance_ids[r]);
  }

  // compute cluster assignments
  update_cluster_assignments();
 };


/*****************************************************************************
 *                                INSPECTORS                                 *
 *****************************************************************************/

const set<string>&
KMedoidsClusterer::get_medoids() const {
  return this->medoids;
}

set<string>
KMedoidsClusterer::get_medoid_members(const string medoid) const {
  set<string> res;
  typedef unordered_map<string, string>::const_iterator AssigIter;
  for (AssigIter it = this->cluster_assignments.begin();
       it != this->cluster_assignments.end(); ++it) {
    if (it->second == medoid)
      res.insert(it->first);
  }
  return res;
}

/**
 * Return the medoid this instance is assigned to currently.
 */
string
KMedoidsClusterer::get_cluster_assignment(const string &instance_name) const {
  unordered_map<string, string>::const_iterator it =\
    this->cluster_assignments.find(instance_name);
  if (it == this->cluster_assignments.end()) {
    throw SMITHLABException("no such instance in cluster assignments: "
                            + instance_name);
  }
  return it->second;
}


/**
 * compute the probability that the given instance belongs to the given
 * medoid
 */
double
KMedoidsClusterer::compute_membership_probability(const string &instance,
                                                  const string &medoid) const {
  double sum = 0;
  for (set<string>::const_iterator it = this->medoids.begin();
       it != this->medoids.end(); ++it) {
    sum += (1/get_distance(*it, instance));
  }
  return (1 / get_distance(instance, medoid)) / sum;
};


/**
 * Find the medoid this instance is closest to (not neccessarily the one it
 * is currently assigned to).
 */
string
KMedoidsClusterer::find_closest_medoid(const string &instance_name) const {
  string closest_medoid;
  double closest_medoid_distance;
  bool first = true;
  typedef set<string>::iterator Miter;
  for (Miter it = this->medoids.begin(); it != this->medoids.end(); ++it) {
    double d = this->get_distance(*it, instance_name);
    if (first || (d < closest_medoid_distance)) {
      closest_medoid = *it;
      closest_medoid_distance = d;
    }
    first = false;
  }
  return closest_medoid;
}


/**
 * Get the distance between two instances.
 */
double
KMedoidsClusterer::get_distance(const string &s, const string &t) const {
  if (s == t) return 0;
  DistanceMatrix::const_iterator dist_iter =\
    this->distance_matrix.find(std::make_pair(s, t));
  if (dist_iter == this->distance_matrix.end()) {
    throw SMITHLABException("no such distance pair: " + s + ", " + t);
  }
  return dist_iter->second;
}


/**
 * compute the cost of the current configuration
 */
double
KMedoidsClusterer::cost() {
  double res = 0;
  for (size_t i = 0; i < this->instance_ids.size(); ++i) {
    string instance_name(this->instance_ids[i]);
    string medoid = this->get_cluster_assignment(instance_name);
    res += this->get_distance(instance_name, medoid);
  }
  return res;
}

bool
KMedoidsClusterer::is_medoid(const string &id) const {
  return (this->medoids.find(id) != this->medoids.end());
}

/*****************************************************************************
 *                                 MUTATORS                                  *
 *****************************************************************************/

void
KMedoidsClusterer::train() {
  set<string> best_medoids(this->medoids);
  double best_cost = this->cost();
  vector<string> medoids_vec;
  std::copy(this->medoids.begin(), this->medoids.end(),
            std::back_inserter(medoids_vec));
  for (size_t i = 0; i < medoids_vec.size(); ++i) {
    string medoid_name(medoids_vec[i]);
    for (size_t j = 0; j < this->instance_ids.size(); ++j) {
      string instance_name(this->instance_ids[j]);
      if (this->is_medoid(instance_name)) continue;
      //cerr << "trying to swap medoid " << medoid_name << " with instance " << instance_name << endl;
      this->swap_medoid(instance_name, medoid_name);
      double new_cost = this->cost();
      //cerr << "gives new cost of " << new_cost << endl;
      if (new_cost < best_cost) {
        medoids_vec[i] = instance_name;
        medoid_name = medoids_vec[i];
        best_cost = new_cost;
        best_medoids = this->medoids;
      } else {
        //cerr << "no better than " << best_cost << endl;
        this->swap_medoid(medoid_name, instance_name);
      }
    }
  }
  this->medoids = best_medoids;
  update_cluster_assignments();
};

/**
 * swap a non-medoid with a medoid and update all of the cluster assignments
 */
void
KMedoidsClusterer::swap_medoid(const string &non_medoid, const string &medoid) {
  if (this->medoids.find(non_medoid) != this->medoids.end()) {
    throw SMITHLABException("cannot swap, " + non_medoid + " is a medoid!");
  }
  if (this->medoids.find(medoid) == this->medoids.end()) {
    throw SMITHLABException("cannot swap, " + medoid + " is not a medoid!");
  }

  set<string>::iterator miter = this->medoids.find(medoid);
  this->medoids.erase(miter);
  this->medoids.insert(non_medoid);
  this->update_cluster_assignments();
}

/**
 * update the assignment of instances to medoids such that each instance
 * is assigned to its closest medoid.
 */
void
KMedoidsClusterer::update_cluster_assignments() {
  for (size_t i = 0; i < this->instance_ids.size(); ++i) {
    string instance_name(this->instance_ids[i]);
    string closest_medoid = this->find_closest_medoid(instance_name);
    this->assign_to_medoid(instance_name, closest_medoid);
  }
}

/**
 * Assign this instance tot he given medoid
 */
void
KMedoidsClusterer::assign_to_medoid(const string &instance,
                                    const string &medoid) {
  set<string>::const_iterator it2 = this->medoids.find(medoid);
  if (it2 == this->medoids.end()) {
    throw SMITHLABException("no such medoid: " + medoid);
  }
  this->cluster_assignments[instance] = medoid;
}
