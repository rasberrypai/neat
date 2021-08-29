#include "innovation.hpp"

using namespace NEAT;

unsigned int InnovationTable::get_innovation(unsigned int in, unsigned int out, bool is_gene) {
  unsigned int result;
  Innovation i = Innovation(in,out);
  std::unordered_map<Innovation,unsigned int,Innovation::hash>::const_iterator got;
  if (is_gene) {
    got = gene_table.find(i);
    if (got == gene_table.end()) {
      result = gene_table.size();
      gene_table.insert(std::make_pair(i,result));
      return result;
    } else {
      return got->second;
    }
  } else {
    got = link_table.find(i);
    if (got == link_table.end()) {
      result = link_table.size();
      link_table.insert(std::make_pair(i,result));
      return result;
    } else {
      return got->second;
    }
  }
}
