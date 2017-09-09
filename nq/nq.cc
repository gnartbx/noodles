#include <iostream>

#include "queens.h"

int main(int argc, char** argv) {
  nq::Queens q = nq::Queens::Create(8);
  std::cout << "num_attacks(8) = " << q.num_attacks() << std::endl;
  std::cout << "Board state: " << q << std::endl;
  return 0;
}
