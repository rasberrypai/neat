#include "population.hpp"
#include "parameters.hpp"

#include <iostream>

using namespace NEAT;

Population::Population(int pop_size, int _inputs, int _outputs):
  generation(0), inputs(_inputs), outputs(_outputs), size(pop_size) {
    for (int i = 0; i < pop_size; i++) {
      Genome g(inputs,outputs,innov_table);
      addToSpecies(g); 
    }
  }

void Population::evaluate() {
  total_fitness = 0;
  max_fitness = 0;
  for (int i = 0; i < species.size(); i++) {
    species[i].evaluate();
    total_fitness += species[i].get_total_fitness();
    double cmp = species[i].get_max_fitness();
    if (cmp > max_fitness) {
      max_fitness = cmp;
    }
  }
}

void Population::next_generation() {
  //sort species - front of species.genomes is representative until next generation
  for (int i = 0; i < species.size(); i++) {
    species[i].sort();
  }

  //update staleness
  for (int i = 0; i < species.size(); i++) {
    species[i].update_staleness();
  } 

  //get total avg fitness of all species
  double avg_fitness = 0;
  for (int i = 0; i < species.size(); i++) {
    avg_fitness += species[i].get_avg_fitness();
  }

  //erase all stale species as well as all weak species (won't spawn any children)
  species.erase(
    std::remove_if(species.begin(), species.end(),
      [=](Species& s) { 
        return (s.get_max_fitness() < max_fitness) && 
               (s.is_dead() || (s.get_avg_fitness() / avg_fitness * size < 1)); 
      }),
    species.end());

  std::vector<Genome> children;
  //breed only if species.avg_fitness / population.total_avg_fitness * population.size >= 1
  //add children - keep best genome of each species
  Random r;
  for (int s = 0; s < species.size(); s++) {
    int num_children = species[s].get_avg_fitness() / avg_fitness * size - 1;
    for (int c = 0; c < num_children && children.size() < size; c++) {
      Genome g(inputs,outputs);
      species[s].get_child(g,r);
      children.push_back(g);
    }
    species[s].cull_weak();
  }
  //if not enough new children
  //  pick random species - breed child and add to a species
  while(children.size() < size - species.size()) {
    int rand_species = r.get_int(0,species.size()-1);
    children.push_back(species[rand_species].get_best());
  }

  //add all children to species
  for (int i = 0; i < children.size(); i++) {
    children[i].set_evaluated(false);
    children[i].mutate(innov_table);
    addToSpecies(children[i]);
  }

  generation++;
}

int Population::get_generation() {
  return generation;
}

double Population::get_total_fitness() {
  return total_fitness;
}

double Population::get_max_fitness() {
  return max_fitness;
}

void Population::addToSpecies(Genome& g) {
  bool speciesFound = false;
  for (int i = 0; i < species.size(); i++) {
    if (species[i].add(g)) {
      speciesFound = true;
      break;
    }
  }
  if (!speciesFound) {
    Species s(g);
    species.push_back(s);
  }
}

//Species

Population::Species::Species(Genome& g):
  staleness(0) {
    genomes.push_back(g);
    best_fitness = g.get_fitness();
    total_fitness += best_fitness;
  }

void Population::Species::sort() {
  std::sort(genomes.rbegin(),genomes.rend());
}

//assume sorted
void Population::Species::update_staleness() {
  double fitness = genomes[0].get_fitness();
  if (fitness > best_fitness) {
    best_fitness = fitness;
    staleness = -1;
  }
  staleness++;
}

//assume sorted
bool Population::Species::add(Genome& g) {
  if (get_best().get_compatability_score(g) < COMPAT_TOLERANCE) {
    genomes.push_back(g);
    total_fitness += g.get_fitness();
    return true;
  }
  return false;
}

bool Population::Species::is_dead() {
  return staleness == STALE_THRESHOLD;
}

//assume sorted
void Population::Species::get_child(Genome& result, Random r) {
  if (genomes.size() == 0) {
    return;
  }
  //pick two random genomes
  int left = r.get_int(0,r.get_int(0,genomes.size()*(SURVIVAL_THRESHOLD / 100.0)));
  int right = r.get_int(0,r.get_int(0,genomes.size()*(SURVIVAL_THRESHOLD / 100.0)));
  if (r.get_float(0,1) < CROSSOVER_CHANCE / 100.0) {
    crossover(genomes[left],genomes[right],result);
  } else {
    result = genomes[left];
  }
}

double Population::Species::get_avg_fitness() {
  if (genomes.size() == 0) {
    return 0;
  }
  return total_fitness / genomes.size();
}

double Population::Species::get_max_fitness() {
  return max_fitness;
}

double Population::Species::get_total_fitness() {
  return total_fitness;
}

//assume sorted
Genome& Population::Species::get_best() {
  return genomes[0];
}

//assume species is sorted
//erase all but first element
void Population::Species::cull_weak() {
  genomes.erase(genomes.begin()+1,genomes.end());
}

void Population::Species::evaluate() {
  total_fitness = 0;
  max_fitness = 0;
  for (int i = 0; i < genomes.size(); i++) {
    double fitness = genomes[i].get_fitness();
    total_fitness += fitness;
    if (fitness > max_fitness) {
      max_fitness = fitness;
    }
  }
}

Genome& Population::get_best() {
  int max = 0;
  double max_value = species[0].get_max_fitness();
  for (int i = 1; i < species.size(); i++) {
    if (species[i].get_max_fitness() > max_value) {
      max_value = species[i].get_max_fitness();
      max = i;
    }
  }
  return species[max].get_best();
}
