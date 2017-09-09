#include <iostream>
#include <random>

#include <gflags/gflags.h>

#include "queens.h"

DEFINE_int32(board_size, 8,
             "Number of rows/columns in the chess boards.");
DEFINE_int64(max_tries, 1024,
             "Number of random tries for a solution.");


int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  nq::Queens q = nq::Queens::Create(FLAGS_board_size);
  std::cout << "Starting board state: " << q << std::endl;

  std::mt19937 rng;
  rng.seed(2495);
  std::uniform_int_distribution<int> method_dist(0, 1);
  std::uniform_int_distribution<size_t> row_dist(0, q.num_rows() - 1);

  for (int iteration = 0; iteration < FLAGS_max_tries; ++iteration) {
    size_t first_row = row_dist(rng);
    size_t second_row = row_dist(rng);
    if (method_dist(rng) > 0) {
      q.Permute(first_row, second_row);
    } else {
      q.Swap(first_row, second_row);
    }
    if (q.num_attacks() == 0) {
      std::cout << "Solved at iteration #" << (iteration + 1) << std::endl
	   << "Board:" << std::endl
	   << q << std::endl;
      return 0;
    }
  }
  return 1;
}
