#include "random.hpp"

using namespace NEAT;

Random::Random() {
  gen = std::mt19937(std::random_device()());
}

int Random::get_int(int low, int high) {
  if (low == high) {
    return low;
  }
  std::uniform_int_distribution<> dis(low,high);
  return dis(gen);
}

float Random::get_float(float low, float high) {
  if (low == high) {
    return low;
  }
  std::uniform_real_distribution<> dis(low, high);
  return dis(gen);
}

double Random::get_double(double low, double high) {
  if (low == high) {
    return low;
  }
  std::uniform_real_distribution<> dis(low, high);
  return dis(gen);
}

