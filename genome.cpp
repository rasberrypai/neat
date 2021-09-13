#include "genome.hpp"

#include <iostream>
#include <GLUT/glut.h>
#include <cmath>
#include <algorithm>

#include "draw.hpp"
#include "parameters.hpp"

#define MAX_Y 64

using namespace NEAT;

//useless
Genome::Genome() {

}

Genome::Genome(int _inputs, int _outputs):
  inputs(_inputs), outputs(_outputs), evaluated(false) {}

Genome::Genome(int _inputs, int _outputs, InnovationTable& it): 
  inputs(_inputs), outputs(_outputs), evaluated(false) {
  int i;
  int o;
  unsigned int id;

  //add bias to genome
  add_gene(0,0,BIAS,0,0,it);

  id = 1;

  //add inputs to genome
  for (i = 0; i < inputs; i++, id++) {
    add_gene(id,id,INPUT,i+1,0,it);
  }
  
  //add outputs to genome
  for (i = 0; i < outputs; i++, id++) {
    add_gene(id,id,OUTPUT,i,MAX_Y,it);
  }

  //connect inputs + bias to outputs
  for (i = 0; i < inputs + 1; i++) {
    for (o = 0; o < outputs; o++) {
      add_link(i,o+inputs+1,1,true,it);
    }
  }
  mutate_all_links();
}

//Genome::Genome(const int _inputs, const int _outputs, std::vector<Gene> _genes, std::vector<Link> _links):
//  inputs(_inputs), outputs(_outputs), genes(_genes), links(_links) {}

double Genome::get_fitness() {
  if (!evaluated) {
    fitness = evaluate();
    evaluated = 1;
  }
  return fitness;
}

double Genome::get_compatability_score(Genome& other) {
  return 0;
}

void Genome::mutate(InnovationTable& it) {
  mutate_all_links();
  Random r;
  mutate_add_link(it,r);
  mutate_add_gene(it,r);
}

void Genome::draw() {
  int i;
  float x_scale;
  float y_scale;
  
  //for links
  std::unordered_map<unsigned int,int>::const_iterator to;
  std::unordered_map<unsigned int,int>::const_iterator from;

  //calculations
  float x_offset = 5;
  float y_offset = 5;
  
  if (inputs+1 > outputs) {
    x_scale = (100.0-2*x_offset) / (inputs+2);
  } else {
    x_scale = (100.0-2*x_offset) / (outputs+1);
  }

  y_scale = (100.0-2*y_offset) / 65.0;

  //actual draw
  for (i = 0; i < genes.size(); i++) {
    drawCircle((genes[i].x+1) * x_scale + x_offset, (genes[i].y+1) * y_scale + y_offset, 0.5, 25);
  }
  for (i = 0; i < links.size(); i++) {
    if (links[i].enabled) {
      to = gene_table.find(links[i].to);
      from = gene_table.find(links[i].from);
      drawLine((genes[from->second].x+1) * x_scale + x_offset, (genes[from->second].y+1) * y_scale + y_offset,(genes[to->second].x+1) * x_scale + x_offset, (genes[to->second].y+1) * y_scale + y_offset);
    }
  }
}

Genome::~Genome() {
  
}

//Attempts to mutate all links
//OPTIMIZE: multi-thread
void Genome::mutate_all_links() {
  Random r; //if multi-threaded initialize within thread function
  for (int i = 0; i < links.size(); i++) {
    mutate_weight(links[i],r);
  }
}

//Attempts to change the weight of link l 
//<param> l: link to mutate
//<param> r: source of randomness
void Genome::mutate_weight(Link& l, Random& r) {
  if (r.get_float(0,1) >= MUTATE_WEIGHT / 100.0) { //change that weight will even mutate
    return; //mutation failed
  }
  if (r.get_float(0,1) < MUTATE_WEIGHT_RANDOM / 100.0) { //random change
    l.weight = r.get_float(0,1) * WEIGHT_RAND_STRENGTH;
  } else {
    l.weight += r.get_float(0,1) * WEIGHT_SHIFT_STRENGTH; //uniform pertrube
  }
}

//Attempts to toggle (disable/enable) link l
//<param> l: link to toggle
//<param> r: source of randomness
void Genome::mutate_link_activation(Link& l, Random& r) {
  if (r.get_float(0,1) >= MUTATE_LINK_TOGGLE / 100.0) {
    return; //mutation failed
  }
  l.enabled = !l.enabled; 
}

//Attempts to add Edge/Link between two unconnected Vertexes/Genes/Neurons
//Nothing will happen if the function fails to find two random unconnected Genes after FIND_LINK_TRIES times
//<param> it: innovation table for the population
//<param> r: source of seeded randomness
void Genome::mutate_add_link(InnovationTable& it, Random& r) {
  if (r.get_float(0,1) >= MUTATE_LINK / 100.0) {
    return; //mutation failed
  }

  if (genes.size() <= 1) {
    return;
  }
  for (int i = 0; i < FIND_LINK_TRIES; i++) {
    int lower = r.get_int(0,genes.size()-1);
    int higher = r.get_int(0,genes.size()-1);
    if (genes[higher].y < genes[lower].y) {
      int swp = lower;
      lower = higher;
      higher = swp;
    }
    if (genes[lower].y < genes[higher].y && add_link(genes[lower].id,genes[higher].id,1,1,it)) { 
      return;
    }
  }
}

//Attempts to split existing Edge/Link with a new Vertex/Gene/Neuron
//<param> it: Innovation Table for the population
//<param> r: source of seeded randomness
void Genome::mutate_add_gene(InnovationTable& it, Random& r) {
  if (r.get_float(0,1) >= MUTATE_GENE / 100.0) {
    return;
  }

  if (links.size() == 0) { //no links 
    return;
  }
  //find link to split
  //OPTIMIZE?: prioritize older links 
  //int chosen = links.size() > 2 ? r.get_int(0,links.size() - 1 - (int) sqrt(links.size())) : 0
  int chosen = r.get_int(0,links.size()-1);

  //disabled so skip
  if (!links[chosen].enabled) {
    return;
  }
  
  links[chosen].enabled = 0;


  //we know that the genes exist so we don't need to check index table
  int from = gene_table.find(links[chosen].from)->second;
  int to = gene_table.find(links[chosen].to)->second;
  
  float x = (genes[to].x - genes[from].x) / 2.0 + genes[from].x;
  float y = (genes[to].y - genes[from].y) / 2.0 + genes[from].y;
  
  add_gene(links[chosen].from,links[chosen].to,HIDDEN,x,y,it);

  int mid_id = genes.back().id;

  //the incoming link/edge gets a weight of 1
  add_link(links[chosen].from,mid_id,1,1,it); 
  //the outgoing link/edge inherits weight of split link
  add_link(mid_id,links[chosen].to,links[chosen].weight,1,it);
  //hence both links combined will be equivalent to the split link
}

//Adds new Vertex/Gene/Neuron to Genome/Graph
//return 0 on failure (duplicate gene); 1 otherwise
int Genome::add_gene(int from, int to, gene_type type, float x, float y, InnovationTable& it) {
  unsigned int id = it.get_innovation(from,to,true);
  Gene g(id,type,x,y);
  return add_gene(g);
}

int Genome::add_gene(Gene& g) {
  if (gene_table.count(g.id)) { //already exists in genome 
    return 0; 
  }
  gene_table.insert(std::make_pair(g.id,genes.size()));
  genes.push_back(g);
  return 1;
  
}

//Adds new Edge/Link to Genome/Graph
//return 0 on failure (duplicate link); 1 otherwise
int Genome::add_link(int from, int to, double weight, bool enabled, InnovationTable& it) {
  unsigned int id = it.get_innovation(from,to,false);
  Link l(id,from,to,weight,enabled);
  return add_link(l);
}

int Genome::add_link(Link& l) {
  if (link_table.count(l.id)) {
    return 0;
  }
  link_table.insert(l.id);
  links.push_back(l);
  if (links.size() > 1 && links.back() < links[links.size()-2]) {
    std::sort(links.begin(),links.end());
  }
  return 1;
}

void NEAT::crossover(Genome& a, Genome& b, Genome& result) {
  //FIXME - allow random object to be passed in as parameter
  Random r;
  
  bool a_dom = false;
  //decide which genome is dominant
  //prioritize fitness and then gene size in case of ties
  if (a.get_fitness() == b.get_fitness()) {
    if (a.genes.size() == b.genes.size()) { //choose at random
      if (r.get_int(0,1)) {
        a_dom = true;
      }
    } else if (a.genes.size() < b.genes.size()){
      a_dom = true;
    }
  } else if (a.get_fitness() > b.get_fitness()) {
    a_dom = true;
  }

  int ai = 0;
  int bi = 0;

  //result links
  while (ai < a.links.size() && bi < b.links.size()) {
    if (a.links[ai] == b.links[bi]) { //choose randomly
      if (r.get_int(0,1)) {
        result.add_link(a.links[ai]); 
      } else {
        result.add_link(b.links[bi]);
      }
      ai++;
      bi++;
    } else if (a.links[ai] > b.links[bi]) { //b is disjoint
      if (!a_dom) {
        result.add_link(b.links[bi]);
      }
      bi++;
    } else { //a is disjoint
      if (a_dom) {
        result.add_link(a.links[ai]);
      }
      ai++;
    }
  }

  if (a_dom) {
    while (ai < a.links.size()) {
      result.add_link(a.links[ai]);
      ai++;
    }
    for (ai = 0; ai < a.genes.size(); ai++) {
      result.add_gene(a.genes[ai]);
    } 
  } else {
    while (bi < b.links.size()) {
      result.add_link(b.links[bi]);
      bi++;
    }
    for (bi = 0; bi < b.genes.size(); bi++) {
      result.add_gene(b.genes[bi]);
    }
  }
}  
