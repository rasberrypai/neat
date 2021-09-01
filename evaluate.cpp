#include "genome.hpp"

#include <iostream>

using namespace NEAT;

double Genome::evaluate() {
  std::cout << "INPUT:" << std::endl;
  for (int i = 0; i < links.size(); i++) {
    std::cout << links[i].weight << std::endl;
  }

  Network n (*this);

  double input[2] = {1.4,2.6};
  n.evaluate(input,2);
  
  std::cout << "OUTPUT: " << n.get_output(0) << std::endl;

  return 0;
}
