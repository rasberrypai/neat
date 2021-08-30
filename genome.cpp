#include "genome.hpp"

#include <iostream>
#include <GLUT/glut.h>
#include <cmath>

#include "draw.hpp"
#include "random.hpp"

#define MAX_Y 64

using namespace NEAT;

Genome::Genome(const int _inputs, const int _outputs, InnovationTable& it): 
  inputs(_inputs), outputs(_outputs) {
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
}

//Genome::Genome(const int _inputs, const int _outputs, std::vector<Gene> _genes, std::vector<Link> _links):
//  inputs(_inputs), outputs(_outputs), genes(_genes), links(_links) {}

double Genome::get_compatability_score(Genome& other) {
  return 0;
}

void Genome::mutate(InnovationTable& it) {
  mutate_add_gene(it);
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
      to = index_table.find(links[i].to);
      from = index_table.find(links[i].from);
      drawLine((genes[from->second].x+1) * x_scale + x_offset, (genes[from->second].y+1) * y_scale + y_offset,(genes[to->second].x+1) * x_scale + x_offset, (genes[to->second].y+1) * y_scale + y_offset);
    }
  }
}

Genome::~Genome() {
  
}

void Genome::mutate_weight_shift(Link& l) {
  
}
void Genome::mutate_weight_random(Link& l) {
  
}
void Genome::mutate_link_activation(Link& l) {
  
}
void Genome::mutate_add_link(InnovationTable& it) {
  
}

//Splits existing Edge/Link with a new Vertex/Gene/Neuron
//<param> it: Innovation Table for the population
void Genome::mutate_add_gene(InnovationTable& it) {
  Random r;

  if (links.size() == 0) { //no links 
    return;
  }
  //find link to split
  int chosen = r.get_int(0,links.size()-1);

  //disabled so skip
  if (!links[chosen].enabled) {
    return;
  }
  
  links[chosen].enabled = 0;


  //we know that the genes exist so we don't need to check index table
  int from = index_table.find(links[chosen].from)->second;
  int to = index_table.find(links[chosen].to)->second;
  
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
void Genome::add_gene(int from, int to, gene_type type, float x, float y, InnovationTable& it) {
  unsigned int id = it.get_innovation(from,to,true);
  if (index_table.count(id)) { //already exists in genome 
    return; 
  }
  index_table.insert(std::make_pair(id,genes.size()));
  genes.push_back(Gene(id,type,x,y));
}

void Genome::add_link(int from, int to, double weight, bool enabled, InnovationTable& it) {
  unsigned int id = it.get_innovation(from,to,false);
  links.push_back(Link(id,from,to,weight,enabled));
}

Genome NEAT::crossover(Genome& a, Genome& b) {
  
}
