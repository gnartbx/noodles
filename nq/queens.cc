#include "queens.h"

#include <algorithm>
#include <unordered_map>

namespace nq {

static size_t BoardSize(size_t num_rows, size_t bitmap_size) {
  // Need at least num_rows**2 bits in each Board, a vector whose
  // elements have bitmap_size bits.
  size_t num_squares = num_rows * num_rows;
  div_t bitmaps_per_board = div(num_squares, bitmap_size);
  return bitmaps_per_board.quot + (bitmaps_per_board.rem > 0 ? 1 : 0);
}


/* static */
Queens Queens::Create(size_t num_rows) {
  return Queens(num_rows);
}

Queens::Queens(size_t num_rows):
  num_rows_(num_rows),
  col_by_row_(num_rows),
  occupied_(BoardSize(num_rows, Queens::BITSET_SIZE)),
  attacks_(InitializeAttacks()) {
  for (size_t row = 0; row < num_rows_; row++) {
    col_by_row_[row] = row;
    set_occupied(occupied_, row, row);
  }
}

size_t Queens::num_attacks() const {
  size_t result = 0;

  for (size_t row = 0; row < num_rows_; row++) {
    size_t index = to_index(row, col_by_row_[row]);
    const Board& attack = (*attacks_)[index];
    for (size_t i = 0; i < attack.size(); ++i) {
      result += (attack[i] & occupied_[i]).count();
    }
  }

  return result;
}

void Queens::Swap(size_t row1, size_t row2) {
  size_t col1 = col_by_row_[row1];
  size_t col2 = col_by_row_[row2];
  set_occupied(occupied_, row1, col1, false);
  set_occupied(occupied_, row2, col2, false);

  col_by_row_[row1] = col2;
  col_by_row_[row2] = col1;
  set_occupied(occupied_, row1, col2, true);
  set_occupied(occupied_, row2, col1, true);
}

void Queens::Permute(size_t start_row, size_t end_row) {
  size_t min_row = std::min(start_row, end_row);
  size_t max_row = std::max(start_row, end_row);
  size_t first_col = col_by_row_[min_row];

  for (size_t row = min_row; row <= max_row; ++row) {
    size_t next_col = (row < max_row) ? col_by_row_[row + 1] : first_col;
    size_t col = col_by_row_[row];

    set_occupied(occupied_, row, col, false);
    col_by_row_[row] = next_col;
    set_occupied(occupied_, row, next_col, true);
  }
}

std::vector<std::pair<size_t, size_t>> Queens::OccupiedRowCols() const {
  std::vector<std::pair<size_t, size_t>> result;;
  for (size_t row = 0; row < num_rows_; row++) {
    result.emplace_back(row, col_by_row_[row]);
  }
  return result;
}

const std::vector<Queens::Board>* Queens::InitializeAttacks() {
  const size_t bitmaps_per_board = occupied_.size();
  // Memoization
  static std::unordered_map<size_t, const std::vector<Queens::Board>*> num_rows_to_attacks;
  auto it = num_rows_to_attacks.find(num_rows_);
  if (it != num_rows_to_attacks.end()) {
    return it->second;
  }

  auto attacks = new std::vector<Queens::Board>();
  
  for (size_t row = 0; row < num_rows_; ++row) {
    for (size_t col = 0; col < num_rows_; ++col) {
      attacks->emplace_back(bitmaps_per_board);
      Board& current = attacks->back();

      for(size_t i = row + 1; i < num_rows_; ) {
	set_occupied(current, i++, col);
      }

      for (size_t i = row; i > 0; ) {
	set_occupied(current, --i, col);
      }

      for(size_t j = col + 1; j < num_rows_; ) {
	set_occupied(current, row, j++);
      }

      for (size_t j = col; j > 0; ) {
	set_occupied(current, row, --j);
      }

      for(size_t i = row + 1, j = col + 1; i < num_rows_ && j < num_rows_; ) {
	set_occupied(current, i++, j++);
      }

      for (size_t i = row, j = col; i > 0 && j > 0; ) {
	set_occupied(current, --i, --j);
      }

      for(size_t i = row + 1, j = col; i < num_rows_ && j > 0; ) {
	set_occupied(current, i++, --j);
      }

      for (size_t i = row, j = col + 1; i > 0 && j < num_rows_; ) {
	set_occupied(current, --i, j++);
      }      
    }
  }
  num_rows_to_attacks[num_rows_] = attacks;
  return attacks;
}

void Queens::Randomize() {
  std::random_shuffle(col_by_row_.begin(), col_by_row_.end());
  for (size_t row = 0; row < num_rows_; row++) {
    size_t occupied_col = col_by_row_[row];
    for (size_t col = 0; col < num_rows_; col++) {
      set_occupied(occupied_, row, col, col == occupied_col);
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
