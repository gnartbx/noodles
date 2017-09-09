#include <iostream>
#include <gflags/gflags.h>

#include "queens.h"

DEFINE_int32(board_size, 8,
             "Number of rows/columns in the chess boards.");


int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  nq::Queens q = nq::Queens::Create(FLAGS_board_size);
  std::cout << "Board state: " << q << std::endl;
  return 0;
}
