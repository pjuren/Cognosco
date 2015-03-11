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

// bring these names into the local namespace
using std::string;
using std::vector;

static void
build_new_instance(const DistanceMatrix &m, Dataset &d,
                   const Instance &inst, const string &inst_name_att_name) {
  string inst_name = inst.get_att_occurrence(instance_name_att_name)->to_string();
  instance res;
  for (auto ait = d.attribtue_begin(); ait != d.->attribtue_end(); ++ait) {
    const Attribute* at_ptr = (*ait);
    string nm = at_ptr->get_attribute_name();
    double dist = d[nm][inst_name];
    res.add_attribute_occurrance(dist, at_ptr);
  }
  dataset.add_instance(instance);
}

static void
build_new_dataset(const DistanceMatrix &m, Dataset &new_ds,
                  const Dataset &exist_ds, const vector<string> &medoids,
                  const std::set<size_t> &ignore_inst_ids,
                  const string &inst_name_att_name) {
  d.clear();
  for (size_t i = 0; i < medoids.size(); ++i) {
    d.add_attribute(Attribute(medoids[i], NUMERIC));
  }
  size_t skipper = 0;
  for (auto it = exist_ds.begin(); it != exist_ds.end(); ++it) {
    if (ignore_inst_ids.find(it->get_instance_id()) != ignore_inst_ids.end()) {
      skipped += 1;
      continue;
    }
    build_new_instance(m, d, *it, inst_name_att_name);
  }
}

void
Classifiers::KMedoids::learn(const Dataset &train_instances,
                             const string &class_label,
                             const std::set<size_t> &ignore_inst_ids,
                             const string &inst_name_att_name) {
  // convert the dataset into a distance matrix
  DistanceMatrix m;
  size_t skipped = 0;
  for (auto it = train_instances.begin(); it != train_instances.end(); ++it) {
    if (ignore_inst_ids.find(it->get_instance_id()) != ignore_inst_ids.end()) {
      skipped += 1;
      continue;
    }
    name_att_val = it->get_attribute(this->name_att);
    for (auto ait = it->attribtue_begin(); ait != it->attribtue_end(); ++ait) {
      m[std::make_pair(ait->get_attribute_name(), name_att_val] =\
        ait->get_attribute_value();
    }
  }

  // perform k-medoids clustering on the distance matrix
  KMedoidsClusterer clstr(m, inst_ids);
  clstr.train();
  this->medoid_names = clstr.get_medoids();

  // build new dataset characterized by distance to the medoids from
  // the clustering
  Dataset nds;
  build_new_dataset(m, nds, train_instances, this->medoid_names,
                    ignore_inst_ids, inst_name_att_name);

  // build a NB classifier from the new instances
  this->nb_classifier.train(nds);
}


/**
 * \brief compute the probability that a given Instance belongs to a given
 *        class using this classifier.
 */
double
Classifiers::KMedoids::posterior_probability(const Instance &test_instance,
                                             const string &class_label) const {
  // TODO this is a pretty round-about way to do it...
  Dataset exst;
  exst.add_instance(test_instance);
  Dataset nds;
  build_new_dataset()

  double res = 1;
  for (Instance::const_iterator it = test_instance.begin();
       it != test_instance.end(); ++it) {
    if ((*it)->get_attribute_name() == this->learned_class) continue;
    res *= this->get_conditional_prob(*it, class_label);
  }
  return res * this->get_prior_prob(class_label);
}
