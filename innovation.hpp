#include <unordered_map>

namespace NEAT {
  class InnovationTable {
    public:
      unsigned int get_innovation(unsigned int in, unsigned int out, bool is_gene);
    private:
      struct Innovation {
        unsigned int in;
        unsigned int out;
        Innovation(unsigned int _in, unsigned int _out): 
          in(_in), out(_out) {}
        bool operator==(const Innovation& other) const { 
          return in == other.in && out == other.out;
        }
        struct hash {
          std::size_t operator()(const Innovation& other) const {
            std::size_t h1 = std::hash<unsigned int>()(other.in);
            std::size_t h2 = std::hash<unsigned int>()(other.out);
            return h1 ^ (h2 << 1);
          }
        };
      };
      std::unordered_map<Innovation,unsigned int,Innovation::hash> gene_table;
      std::unordered_map<Innovation,unsigned int,Innovation::hash> link_table;
  };
}

