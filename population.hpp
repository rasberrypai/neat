#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#include "genome.hpp"

namespace NEAT {
  class Population {
    public:
      Population(int pop_size, int _inputs, int _outputs);
      void evaluate();
      void next_generation();
      int get_generation();
      double get_total_fitness();
      double get_max_fitness();

      Genome& get_best();
    private:
      class Species {
        public:
          Species(Genome& g);
          void sort();
          void update_staleness();
          bool add(Genome& g, double tolerance);
          bool is_dead();
          void get_child(Genome& result, Random r = Random());
          double get_avg_fitness();
          double get_max_fitness();
          double get_total_fitness();
          Genome& get_best();
          void cull_weak();
          void evaluate();
        private:
          std::vector<Genome> genomes;
          double total_fitness;
          double max_fitness;
          double best_fitness;
          int staleness;
      };
      InnovationTable innov_table;
      std::vector<Species> species;
      int generation;
      int inputs;
      int outputs;
      int size;

      double compat_tolerance;

      double total_fitness;
      double max_fitness;
 
      //speciation
      void addToSpecies(Genome& g);
  };
}

#endif
