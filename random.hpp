#include <random>

namespace NEAT {
  class Random {
    public:
	  //constructor
      Random();

      //return: random int in range [low,high]
      int get_int(int low, int high);

      //return: random float in range [low,high]
      float get_float(float low, float high);
      
      //return: random double in range [low,high]
      double get_double(double low, double high);

    private:
      std::mt19937 gen;
  };
}
