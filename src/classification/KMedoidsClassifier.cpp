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
#include <string>
#include <vector>

// local includes
#include "Dataset.hpp"
#include "KMedoids.hpp"
#include "KMedoidsClassifier.hpp"
#include "DistanceMatrix.hpp"

// bring these names into the local namespace
using std::string;
using std::vector;
using std::set;

/*****************************************************************************
 *                         STATIC HELPER FUNCTIONS                           *
 *****************************************************************************/

/**
 * convert a dataset that gives the distance
 * Instances in the intial dataset have  N + 2 attributes, where N of these
 * are distances to the other N instances in the dataset and the extra two are
 * the name of the instance and the class. The names must match the attributes.
 * A new dataset is constructed where each instance is described only by the
 * attributes named in medoids.
 */
static void
build_new_dataset(const Dataset &exist_ds, Dataset &new_ds,
                  const set<string> &medoids,
                  const set<size_t> &ignore_inst_ids,
                  const string &inst_name_att_name) {
  new_ds = Dataset(exist_ds, ignore_inst_ids);

  vector<string> att_names;
  for (auto it = new_ds.begin_attributes(); it != new_ds.end_attributes(); ++it) {
    Attribute* att_ptr = (*it);
    att_names.push_back(att_ptr->get_name());
  }

  for (auto name : att_names) {
    if (std::find(medoids.begin(), medoids.end(), name) == medoids.end())
      new_ds.delete_attribute(name);
  }
}


/*****************************************************************************
 *                       Classifiers::KMedoids CLASS                         *
 *****************************************************************************/

string
Classifiers::KMedoids::to_string() const {
  std::stringstream ss;
  ss << "KMedoids classifier constructed using " << this->name_att
     << " as names; learned medoids as " << join(this->medoid_names, ", ")
     << "and nb classifier as " << this->nb_classifier.to_string();
  return ss.str();
}

void
Classifiers::KMedoids::learn(const Dataset &train_instances,
                             const string &class_label,
                             const std::set<size_t> &ignore_inst_ids) {
  // convert the dataset into a distance matrix
  std::cerr << "building distance matrix " << std::endl;
  DistanceMatrix m;
  size_t skipped = 0;
  std::set<string> inst_ids_set;
  for (auto it = train_instances.begin(); it != train_instances.end(); ++it) {
    if (ignore_inst_ids.find(it->get_instance_id()) != ignore_inst_ids.end()) {
      skipped += 1;
      continue;
    }
    string name_att_val = it->get_att_occurrence(this->name_att)->get_attribute_name();
    for (auto ait = it->begin(); ait != it->end(); ++ait) {
      AttributeOccurrence* aoc_ptr = (*ait);
      m[std::make_pair(aoc_ptr->get_attribute_name(), name_att_val)] =\
        ((*aoc_ptr) * 1.0);
      inst_ids_set.insert(aoc_ptr->get_attribute_name());
    }
  }

  // perform k-medoids clustering on the distance matrix
  std::cerr << "perform clustering " << std::endl;
  vector<string> inst_ids;
  std::copy(inst_ids_set.begin(), inst_ids_set.end(), std::back_inserter(inst_ids));
  KMedoidsClusterer clstr(2, m, inst_ids);
  clstr.train();
  this->medoid_names = clstr.get_medoids();

  // build new dataset characterized by distance to the medoids from
  // the clustering
  std::cerr << "transofrm dataset " << std::endl;
  Dataset nds;
  build_new_dataset(train_instances, nds, this->medoid_names,
                    ignore_inst_ids, this->name_att);

  // build a NB classifier from the new instances
  std::cerr << "build nb classifier " << std::endl;
  this->nb_classifier.learn(nds, class_label);
}


/**
 * \brief compute the probability that a given Instance belongs to a given
 *        class using this classifier.
 */
double
Classifiers::KMedoids::class_probability(const Instance &test_instance,
                                         const string &class_label) const {
  Instance t_cp(test_instance); // !!!!!!!!!! make sure copy constructor is set up for instance!!!
  vector<string> att_names;
  for (auto it = t_cp.begin(); it != t_cp.end(); ++it) {
    AttributeOccurrence *att_oc_ptr = (*it);
    att_names.push_back(att_oc_ptr->get_attribute_name());
  }
  for (auto it = att_names.begin(); it != att_names.end(); ++it) {
    if (this->medoid_names.find(*it) == this->medoid_names.end())
      t_cp.delete_attribute_occurrence(*it);
  }

  return this->nb_classifier.class_probability(t_cp, class_label);
}
