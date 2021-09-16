#include "genome.hpp"

#include <iostream>
#include <cmath>

using namespace NEAT;

double Genome::evaluate() {

  double fitness = 0;
  

  //5-10 microseconds
  Network n (*this);

  double output;

  double input[4][3] = {
                        {0.0,0.0,0.0},
                        {0.0,1.0,1.0},
                        {1.0,0.0,1.0},
                        {1.0,1.0,0.0}
                       };

  //0-1 microsecond
  for (int i = 0; i < 4; i++) {
    n.evaluate(input[i],2);
    output = n.get_output(0);
    output = 0.5*(output*4.9)/(1+abs(output*4.9))+0.5;
    fitness += abs(input[i][2] - output);
  }

  return (4 - fitness)*(4-fitness);
}
