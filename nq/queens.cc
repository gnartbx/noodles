#include "queens.h"

#include <algorithm>

namespace nq {

/* static */
Queens Queens::Create(size_t num_rows) {
  return Queens(num_rows);
}

Queens::Queens(size_t num_rows):
  num_rows_(num_rows),
  col_by_row_(num_rows) {
  for (size_t row = 0; row < num_rows_; row++) {
    col_by_row_[row] = row;
  }
}

size_t Queens::num_attacks() const {
  size_t result = 0;

  for (size_t row = 0; row < num_rows_; row++) {
    size_t col = col_by_row_[row];
    for (size_t next_row = row + 1; next_row < num_rows_; next_row++) {
      size_t next_col = col_by_row_[next_row];
      if (next_col + row == col + next_row ||
	  next_col + next_row == col + row) {
	result += 2;
      }
    }
  }

  return result;
}

void Queens::Swap(size_t row1, size_t row2) {
  size_t col1 = col_by_row_[row1];
  size_t col2 = col_by_row_[row2];

  col_by_row_[row1] = col2;
  col_by_row_[row2] = col1;
}

void Queens::Permute(size_t start_row, size_t end_row) {
  size_t min_row = std::min(start_row, end_row);
  size_t max_row = std::max(start_row, end_row);
  size_t first_col = col_by_row_[min_row];

  for (size_t row = min_row; row <= max_row; ++row) {
    size_t next_col = (row < max_row) ? col_by_row_[row + 1] : first_col;

    col_by_row_[row] = next_col;
  }
}

std::vector<std::pair<size_t, size_t>> Queens::OccupiedRowCols() const {
  std::vector<std::pair<size_t, size_t>> result;;
  for (size_t row = 0; row < num_rows_; row++) {
    result.emplace_back(row, col_by_row_[row]);
  }
  return result;
}
void Queens::Randomize() {
  std::random_shuffle(col_by_row_.begin(), col_by_row_.end());
}

}  // namespace nq


std::ostream &operator<<(std::ostream &os, nq::Queens const &m) {
  os << "Queens[" << m.num_rows() << "] (attacks: " << m.num_attacks() << ")" << std::endl;
  for (auto &it: m.OccupiedRowCols()) {
    for (size_t col = 0; col < m.num_rows(); col++) {
      if (col == it.second) {
        os << "Q";
      } else {
        os << ((it.first + col) % 2 == 0 ? "." : " ");
      }
    }
    os << std::endl;
  }
  return os;	
}
