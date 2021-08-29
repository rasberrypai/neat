#include <vector>

#include "innovation.hpp"

namespace NEAT {
  class Genome {
    public:
      //Constructor - basic network where all inputs + bias are connected to all outputs
      Genome(const int _inputs, const int _outputs, InnovationTable& it);
      //

      //breeding
      friend crossover(Genome& a, Genome& b);
      //

      //compatability
      get_compatability_score(Genome& other);
      //

      //mutation
      mutate(InnovationTable& it);
      //

      //visual representation of network
      void draw();
      //
      
      //Destructor
      ~Genome();
      //
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
        bool operator < (const Link& other) {
          return id < other.id;
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
      const int inputs;
      const int outputs;
      //

      //fitness
      
      //
      
      //mutation

      //
  };
}
