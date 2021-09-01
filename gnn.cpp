#include "gnn.hpp"

#include <algorithm>
#include <iostream>

using namespace NEAT;

Network::Network(Genome& g) {
  vertexes.reserve(g.genes.size()); //capacity
  
  //add all genes to our vector
  for (int i = 0; i < g.genes.size(); i++) {
    Genome::gene_type type = g.genes[i].type;
    int depth = g.genes[i].y;
    Vertex* v = new Vertex(depth,type);
    vertexes.push_back(v);
    if(type == Genome::INPUT) {
      inputs.push_back(v);
    } else if(type == Genome::OUTPUT) {
      outputs.push_back(v);
    }
  }
  
  //add links using index table (index table should correspond to our
  for (int i = 0; i < g.links.size(); i++) {
    if (!g.links[i].enabled) {
      continue;
    }
    int from = g.gene_table.find(g.links[i].from)->second;
    int to = g.gene_table.find(g.links[i].to)->second;
    Edge e(*vertexes[to],g.links[i].weight);
    vertexes[from]->add_edge(e); 
  }

  auto comparator = [](Vertex* a, Vertex* b) {
    return *a < *b;
  };

  std::sort(vertexes.begin(),vertexes.end(),comparator);
}

//Feed inputs through network
//NOTE: will succeed even if inputs (param) != # of input nodes in network
//<param> args: array of inputs to be added to the network
//<param> size: size of args
void Network::evaluate(double args[], int size) {
  int i;
  //reset all
  for (i = 0; i < vertexes.size(); i++) {
    vertexes[i]->flush();
  }
  
  //set inputs
  for (i = 0; i < size && i < inputs.size(); i++) {
    inputs[i]->set_value(args[i]);
  }

  //evaluate all
  for (i = 0; i < vertexes.size(); i++) {
    vertexes[i]->evaluate();
  }
}

double Network::get_output(unsigned int index) {
  if (index >= outputs.size()) {
    return 0; //HACK: maybe throw error
  }
  return outputs[index]->get_value();
}

Network::~Network() {
  for (int i = 0; i < vertexes.size(); i++) {
    delete vertexes[i];
  }
}

//Vertex implementation
//NOTE: typeof(_type) == Genome::gene_type
Network::Vertex::Vertex(int _depth,int _type):
  type(_type), depth(_depth)
{
  flush();
}

void Network::Vertex::flush() {
  value = type == Genome::BIAS ? 1 : 0;
  activated = type == Genome::HIDDEN ? false : true; //only hidden nodes need to be activated
}
 
void Network::Vertex::evaluate() {
  if (!activated) {
    value = activation_function(value);
    activated = true;
  }
  for (int i = 0; i < edges.size(); i++) {
    edges[i].get_destination().value += value * edges[i].get_weight();
  }
}

void Network::Vertex::add_edge(Edge& e) {
  edges.push_back(e);
}

bool Network::Vertex::operator < (const Vertex& other) const {
  return depth < other.depth;
}

void Network::Vertex::set_value(double x) {
  value = x;
}

double Network::Vertex::get_value() {
  return value;
}

double Network::Vertex::activation_function(double x) {
  return x;
}

int Network::Vertex::get_depth() {
  return depth;
}

//

//Edge implementation
Network::Edge::Edge(Vertex& _dest, double _weight):
  dest(_dest), weight(_weight) {}

Network::Edge& Network::Edge::operator = (const Edge& other) {
  dest = other.dest;
  weight = other.weight;
  return *this;
}

Network::Vertex& Network::Edge::get_destination() {
  return dest;
}

double Network::Edge::get_weight() {
  return weight;
}
//
