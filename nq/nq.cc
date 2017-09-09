#include <iostream>
#include <random>
#include <thread>

#include <gflags/gflags.h>
#include <time.h>

#include "queens.h"

DEFINE_int32(board_size, 8,
             "Number of rows/columns in the chess boards.");
DEFINE_int32(num_threads, 32,
             "Number of threads to try to solve with.");
DEFINE_int64(max_tries, 1024,
             "Number of random tries per thread for a solution.");

static void solve(const nq::Queens &start, const int64_t max_tries, volatile bool *found) {
  nq::Queens q = start;

  std::mt19937 rng(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
  std::uniform_int_distribution<int> method_dist(0, 1);
  std::uniform_int_distribution<size_t> row_dist(0, q.num_rows() - 1);

  for (int iteration = 0; iteration < max_tries && !*found; ++iteration) {
    size_t first_row = row_dist(rng);
    size_t second_row = row_dist(rng);
    if (method_dist(rng) > 0) {
      q.Permute(first_row, second_row);
    } else {
      q.Swap(first_row, second_row);
    }
    if (q.num_attacks() == 0 && !*found) {
      *found = true;
      std::cout << "Solved at iteration #" << (iteration + 1) << std::endl
	   << "Board:" << std::endl
	   << q << std::endl;
    }
  }
}  

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  nq::Queens q = nq::Queens::Create(FLAGS_board_size);
  std::cout << "Starting board state: " << q << std::endl;

  volatile bool solved = false;
  std::vector<std::thread> threads;
  for (int i = 0; i < FLAGS_num_threads; i++) {
    threads.emplace_back(solve, q, FLAGS_max_tries, &solved);
  }

  for (auto& thread: threads) {
    thread.join();
  }

  return solved ? 0 : 1;
}
