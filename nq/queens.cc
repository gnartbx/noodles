#include "queens.h"

#include <algorithm>

namespace nq {

/* static */
Queens Queens::Create(size_t num_rows) {
  return Queens(num_rows);
}

Queens::Queens(size_t num_rows):
  num_rows_(num_rows),
  col_by_row_(num_rows),
  occupied_(num_rows * num_rows),
  attacks_(num_rows * num_rows) {
  for (size_t row = 0; row < num_rows_; row++) {
    col_by_row_[row] = row;
    occupied_[row * (num_rows_ + 1)] = true;
  }

  InitializeAttacks();
}

size_t Queens::num_attacks() const {
  size_t result = 0;

  for (size_t row = 0; row < num_rows_; row++) {
    size_t index = to_index(row, col_by_row_[row]);
    /* Do bit-by-bit && of attacks_[index], occupied_ */
    const std::vector<bool> attacks = attacks_[index];
    for (auto attacks_it = attacks.begin(), occupied_it = occupied_.begin();
	 attacks_it != attacks.end() && occupied_it != occupied_.end();
         ++attacks_it, ++occupied_it) {
      if (*attacks_it && *occupied_it) {
	result++;
      }
    }
  }

  return result;
}

void Queens::Swap(size_t row1, size_t row2) {
  size_t col1 = col_by_row_[row1];
  size_t col2 = col_by_row_[row2];
  occupied_[to_index(row1, col1)] = 0;
  occupied_[to_index(row2, col2)] = 0;

  col_by_row_[row1] = col2;
  col_by_row_[row2] = col1;
  occupied_[to_index(row1, col2)] = 1;
  occupied_[to_index(row2, col1)] = 1;
}

void Queens::Permute(size_t start_row, size_t end_row) {
  size_t min_row = std::min(start_row, end_row);
  size_t max_row = std::max(start_row, end_row);
  size_t first_col = col_by_row_[min_row];

  for (size_t row = min_row; row <= max_row; ++row) {
    size_t next_col = (row < max_row) ? col_by_row_[row + 1] : first_col;
    size_t col = col_by_row_[row];

    occupied_[to_index(row, col)] = 0;
    col_by_row_[row] = next_col;
    occupied_[to_index(row, next_col)] = 1;
  }
}

std::vector<std::pair<size_t, size_t>> Queens::OccupiedRowCols() const {
  std::vector<std::pair<size_t, size_t>> result;;
  for (size_t row = 0; row < num_rows_; row++) {
    result.emplace_back(row, col_by_row_[row]);
  }
  return result;
}


void Queens::InitializeAttacks() {
  for (size_t row = 0; row < num_rows_; ++row) {
    for (size_t col = 0; col < num_rows_; ++col) {
      attacks_[to_index(row, col)] = std::vector<bool>(num_rows_ * num_rows_);
      std::vector<bool> &attack = attacks_[to_index(row, col)];
      for(size_t i = row + 1; i < num_rows_; ) {
	attack[to_index(i++, col)] = 1;
      }

      for (size_t i = row; i > 0; ) {
	attack[to_index(--i, col)] = 1;
      }

      for(size_t j = col + 1; j < num_rows_; ) {
	attack[to_index(row, j++)] = 1;
      }

      for (size_t j = col; j > 0; ) {
	attack[to_index(row, --j)] = 1;
      }

      for(size_t i = row + 1, j = col + 1; i < num_rows_ && j < num_rows_; ) {
	attack[to_index(i++, j++)] = 1;
      }

      for (size_t i = row, j = col; i > 0 && j > 0; ) {
	attack[to_index(--i, --j)] = 1;
      }

      for(size_t i = row + 1, j = col; i < num_rows_ && j > 0; ) {
	attack[to_index(i++, --j)] = 1;
      }

      for (size_t i = row, j = col + 1; i > 0 && j < num_rows_; ) {
	attack[to_index(--i, j++)] = 1;
      }      
    }
  }
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
