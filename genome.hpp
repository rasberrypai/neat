#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <unordered_set>

#include "innovation.hpp"
#include "random.hpp"

//HACK
namespace NEAT { //forward declaration to deal with codependency
  class Genome;
}

#include "gnn.hpp"

namespace NEAT {
  class Genome {
    public:
      //Constructors
      //default
      Genome();
      //creates empty genome that expects inputs and outputs
      Genome(int _inputs,int _outputs);
      //create basic network where all inputs + bias are connected to all outputs
      Genome(int _inputs,int _outputs, InnovationTable& it);
      //

      //fitness
      double get_fitness();
      //

      //breeding
      friend void crossover(Genome& a, Genome& b, Genome& result);
      //

      //compatability
      double get_compatability_score(Genome& other);
      //

      //mutation
      void mutate(InnovationTable& it);
      //

      //visual representation of network
      void draw();
      //
      
      //Destructor
      ~Genome();
      //
      
      void set_evaluated(bool e);

      bool operator < (const Genome& other) const;
      bool operator > (const Genome& other) const;

    private:
      //structure
      enum gene_type {INPUT,HIDDEN,OUTPUT,BIAS};
      //represents an edge in the genome graph
      struct Link {
        unsigned int id;
        unsigned int from;
        unsigned int to;
        double weight;
        bool enabled;
        Link(unsigned int _id, unsigned int _from, unsigned int _to, double _weight, bool _enabled): 
          id(_id), from(_from), to(_to), weight(_weight), enabled(_enabled) {}
        bool operator < (const Link& other) const {
          return id < other.id;
        }
        bool operator > (const Link& other) const {
          return id > other.id;
        }
        bool operator == (const Link& other) const {
          return id == other.id;
        }
      };
      //represents a vertex in the genome graph
      struct Gene {
        unsigned int id;
        gene_type type;
        float x;
        float y;
        Gene(unsigned int _id, gene_type _type, float _x, float _y): 
          id(_id), type(_type), x(_x), y(_y) {}
      };
      
      std::vector<Link> links; //all edges/links in the genome
      std::vector<Gene> genes; //all vertexes/genes in the genome
      std::unordered_map<unsigned int,int> gene_table;
      std::unordered_set<unsigned int> link_table;
      int inputs;
      int outputs;
      //
      
      //private Constructor
      //create off of vector input
      //Genome(const int _inputs, const int _outputs, std::vector<Gene> _genes, std::vector<Link> _links);
      //

      //fitness
      double fitness;
      bool evaluated;
      friend class Network;
      double evaluate();
      //
      
      //mutation
      void mutate_all_links();
      void mutate_weight(Link& l, Random& r);
      void mutate_link_activation(Link& l, Random& r);
      void mutate_add_link(InnovationTable& it, Random& r);
      void mutate_add_gene(InnovationTable& it, Random& r);
      //

      //add to structure
      int add_gene(int from, int to, gene_type type, float x, float y, InnovationTable& it);
      int add_gene(Gene& g);
      int add_link(int from, int to, double weight, bool enabled, InnovationTable& it);
      int add_link(Link& l);
      //
  };
  void crossover(Genome& a, Genome& b);
}

#endif
