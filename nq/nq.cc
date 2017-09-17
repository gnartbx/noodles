#include <atomic>
#include <chrono>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <thread>

#include <gflags/gflags.h>
#include <math.h>
#include <signal.h>
#include <time.h>

#include "queens.h"

DEFINE_int32(board_size, 8,
             "Number of rows/columns in the chess boards.");
DEFINE_int32(num_threads, 32,
             "Number of threads to try to solve with.");
DEFINE_int64(num_attempts, 1024,
             "Total number of attempts.");
DEFINE_int64(max_tries, 24,
             "Number of random tries within an annealing iteration.");
DEFINE_int32(annealing_steps, 200,
	     "Maximum number of annealing steps run in each solution attempt");
DEFINE_int32(stats_interval_seconds, 10,
	     "Interval between reporting stats, in seconds. No reporting if <= 0");

static const double T_max = 1.0;
static const double T_min = 0.00001;

class Stats {
 public:
  Stats():
    start_(clock()),
    num_accepted_(0),
    num_rejected_(0),
    min_cost_(std::numeric_limits<short>::max()) {}

  void UpdateAccepted(size_t delta_accepted) {
    std::atomic_fetch_add(&num_accepted_, delta_accepted);
  }

  void UpdateRejected(size_t delta_rejected) {
    std::atomic_fetch_add(&num_rejected_, delta_rejected);
  }

  void UpdateMinCost(size_t min_cost) {
    if (min_cost < min_cost_) {
      std::lock_guard<std::mutex> lock(mutex_);
      min_cost_ = std::min(min_cost, min_cost_);
    }
  }
  
  size_t GetAccepted() const { return num_accepted_; }
  size_t GetRejected() const { return num_rejected_; }
  float GetElapsedSeconds() const {
    return  (float)(clock() - start_) / CLOCKS_PER_SEC;
  }

  std::ostream& Dump(std::ostream &os) {
    return os << "Elapsed time:     " << GetElapsedSeconds() << " (s)" << std::endl
	      << "Rejected configs: " << GetRejected() << std::endl
              << "Accepted configs: " << GetAccepted() << std::endl
              << "Min cost:         " << min_cost_ << std::endl;
  }

 private:
  clock_t start_;
  std::mutex mutex_;
  std::atomic<size_t> num_accepted_;
  std::atomic<size_t> num_rejected_;
  size_t min_cost_;
};

volatile bool solved = false;
volatile bool interrupted = false;

static void handle_signal(int signum) {
  solved = interrupted = true;
}

static void solve(const nq::Queens &start, const double alpha, const int64_t max_tries, Stats *stats, volatile bool *found) {
  nq::Queens q = start;
  q.Randomize();

  std::mt19937 rng(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
  std::uniform_int_distribution<int> method_dist(0, 1);
  std::uniform_int_distribution<size_t> row_dist(0, q.num_rows() - 1);
  std::uniform_real_distribution<> accept_dist(0, 1);

  nq::Queens old_q = q;
  float old_cost = old_q.num_attacks();
  float min_cost = old_cost;
  size_t num_steps = 0;

  size_t accepted = 0;
  size_t rejected = 0;
  for (float T = T_max; T > T_min && !*found; T = T * alpha) {
    for (int iteration = 0; iteration < max_tries && !*found; iteration++) {
      num_steps++;
      size_t first_row = row_dist(rng);
      size_t second_row = row_dist(rng);
      if (method_dist(rng) > 0) {
	q.Permute(first_row, second_row);
      } else {
	q.Swap(first_row, second_row);
      }

      float new_cost = q.num_attacks();
      if (new_cost == 0 && !*found) {
	*found = true;
	std::cout << "Solved at step #" << num_steps << ", temperature " << T << std::endl
		  << "Board:" << std::endl
		  << q << std::endl;
      }

      if (old_cost >= new_cost || exp((old_cost - new_cost) / T) > accept_dist(rng)) {
	accepted++;
	old_q = q;
	old_cost = new_cost;
	min_cost = std::min(min_cost, old_cost);
      } else {
	rejected++;
	q = old_q;
      }
    }
    stats->UpdateAccepted(accepted);
    stats->UpdateRejected(rejected);
    stats->UpdateMinCost(min_cost);
    accepted = rejected = 0;
  }
  stats->UpdateAccepted(accepted);
  stats->UpdateRejected(rejected);
  stats->UpdateMinCost(min_cost);
}

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  float alpha = exp(log(T_min / T_max) / FLAGS_annealing_steps);
  
  nq::Queens q = nq::Queens::Create(FLAGS_board_size);

  int64_t remaining_tries = FLAGS_num_attempts;

  signal(SIGINT, &handle_signal);
  Stats stats;

  if (FLAGS_stats_interval_seconds > 0) {
    std::thread([&stats]() {
	while (true) {
	  std::this_thread::sleep_for(std::chrono::seconds(FLAGS_stats_interval_seconds));
	  if (solved) {
	    break;
	  }
	  stats.Dump(std::cout) << std::endl;
	}
      }).detach();
  }

  while (remaining_tries > 0 && !solved) {
    int32_t num_threads = (FLAGS_num_threads <= remaining_tries) ? FLAGS_num_threads : remaining_tries;
    std::cout << "Remaining tries: " << remaining_tries << " ... Starting " << num_threads << " attempts." << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
      threads.emplace_back(solve, q, alpha, FLAGS_max_tries, &stats, &solved);
    }

    for (auto& thread: threads) {
      thread.join();
    }
    remaining_tries -= num_threads;
  }

  stats.Dump(std::cout);

  return solved ? 0 : 1;
}
