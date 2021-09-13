#include "genome.hpp"

#include <iostream>
#include <cmath>

using namespace NEAT;

double Genome::evaluate() {
  double fitness = 0;

  Network n (*this);

  double output;

  double input[2] = {0.0,0.0};

  n.evaluate(input,2);
  output = n.get_output(0);
  output = output/(1+abs(output));
  fitness += output*output;

  input[1] = 1.0;
  n.evaluate(input,2);
  output = n.get_output(0);
  output = output/(1+abs(output));
  fitness += (1-output)*(1-output);

  input[0] = 1.0;
  input[1] = 0.0;
  n.evaluate(input,2);
  output = n.get_output(0);
  output = output/(1+abs(output));
  fitness += (1-output)*(1-output);

  input[0] = 0.0;
  input[1] = 1.0;
  n.evaluate(input,2);
  output = n.get_output(0);
  output = output/(1+abs(output));
  fitness += output*output;

  return 4 - fitness;
}
