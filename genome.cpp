#include "genome.hpp"

#include <iostream>
#include <GLUT/glut.h>
#include <cmath>
#include <iostream>

#include "draw.hpp"

#define MAX_Y 64

using namespace NEAT;

Genome::Genome(const int _inputs, const int _outputs, InnovationTable& it) : inputs(_inputs), outputs(_outputs) {
  int i;
  int o;
  unsigned int id;

  //add bias to genome
  genes.push_back(Gene(it.get_innovation(0,0,true),BIAS,0,0));

  id = 1;

  //add inputs to genome
  for (i = 0; i < inputs; i++, id++) {
    genes.push_back(Gene(it.get_innovation(id,id,true),INPUT,i+1,0));
  }
  //add outputs to genome
  for (i = 0; i < outputs; i++, id++) {
    genes.push_back(Gene(it.get_innovation(id,id,true),OUTPUT,i,MAX_Y));
  }

  //connect inputs + bias to outputs
  for (i = 0; i < inputs + 1; i++) {
    for (o = 0; o < outputs; o++) {
      links.push_back(Link(it.get_innovation(i,o+inputs+1,false),i,o+inputs+1,1,true));
    }
  }
}

Genome crossover(Genome& a, Genome& b) {
    
}

double Genome::get_compatability_score(Genome& other) {

}

void Genome::mutate() {

}

void Genome::draw() {
  int i;
  float x_scale;
  float y_scale;
  
  //for links
  std::unordered_map<int,int> index_table;
  std::unordered_map<int,int>::const_iterator to;
  std::unordered_map<int,int>::const_iterator from;

  //calculations
  float x_offset = 5;
  float y_offset = 5;
  
  if (inputs+1 > outputs) {
    x_scale = (100.0-2*x_offset) / (inputs+2);
  } else {
    x_scale = (100.0-2*x_offset) / (outputs+1);
  }

  y_scale = (100.0-2*y_offset) / 65.0;

  //population index_table for links
  for (i = 0; i < genes.size(); i++) {
    index_table.insert(std::make_pair(genes[i].id,i));
  }

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
