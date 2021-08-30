#include "random.hpp"

using namespace NEAT;

Random::Random() {
  gen = std::mt19937(std::random_device()());
}

int Random::get_int(int low, int high) {
  std::uniform_int_distribution<> dis(low,high);
  return dis(gen);
}

float Random::get_float(float low, float high) {
  std::uniform_real_distribution<> dis(low, high);
  return dis(gen);
}

double Random::get_double(double low, double high) {
  std::uniform_real_distribution<> dis(low, high);
  return dis(gen);
}

