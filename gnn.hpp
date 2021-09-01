#ifndef GNN_H
#define GNN_H

#include <vector>
#include <array>

#include "genome.hpp"

namespace NEAT {
  class Network {
    public:
      Network(Genome& g);
      void evaluate(double args[], int size);
      double get_output(unsigned int index);
      ~Network();
    private:
      //structure
      class Vertex;
      class Edge;

      class Vertex {
        public:
          //NOTE: typeof(_type) == Genome::gene_type
          Vertex(int depth,int _type);
          void flush();
          void evaluate();
          void add_edge(Edge& e);
          bool operator < (const Vertex& other) const;
          void set_value(double x);
          double get_value();
          int get_depth();
        private:
          std::vector<Edge> edges;
          double value;
          //NOTE: typeof(type) == Genome::gene_type
          int type;
          int depth;
          bool activated;
          //activation function
          double activation_function(double x);
      };
      class Edge {
        public:
          Edge(Vertex& _dest, double _weight);
          Edge& operator = (const Edge& other);
          Vertex& get_destination();
          double get_weight();
        private:
          Vertex& dest;
          double weight;
      };

      std::vector<Vertex*> inputs; //stores the indexes of each input vertex in vertexes
      std::vector<Vertex*> outputs; //stores the indexes of each output vertex in vertexes
      std::vector<Vertex*> vertexes;
      //

  };
}

#endif
