#ifndef NQ_QUEENS_H_
#define NQ_QUEENS_H_

#include <bitset>
#include <ostream>
#include <utility>
#include <vector>

namespace nq {

template <size_t N>
class Queens {
  using Board = std::bitset<N * N>;

 public:
   Queens():
      attacks_(N * N) {
    for (size_t row = 0; row < N; row++) {
      col_by_row_[row] = row;
      occupied_[row * (N + 1)] = 1;
    }

    InitializeAttacks();
  }

  size_t num_attacks() const {
    size_t result = 0;

    for (size_t row = 0; row < N; row++) {
      size_t index = to_index(row, col_by_row_[row]);
      result += (attacks_[index] & occupied_).count();
    }

    return result;
  }

  void SwapQueens(size_t row1, size_t row2) {
    size_t col1 = col_by_row_[row1];
    size_t col2 = col_by_row_[row2];
    occupied_[to_index(row1, col1)] = 0;
    occupied_[to_index(row2, col2)] = 0;

    col_by_row_[row1] = col2;
    col_by_row_[row2] = col1;
    occupied_[to_index(row1, col2)] = 1;
    occupied_[to_index(row2, col1)] = 1;
  }

  std::vector<std::pair<size_t, size_t>> OccupiedRowCols() const {
    std::vector<std::pair<size_t, size_t>> result;;
    for (size_t row = 0; row < N; row++) {
      result.emplace_back(row, col_by_row_[row]);
    }
    return result;
  }

 private:
  inline size_t to_index(size_t row, size_t col) const {
    return row * N + col;
  }
  
  void InitializeAttacks() {
    for (size_t row = 0; row < N; ++row) {
      for (size_t col = 0; col < N; ++col) {
        Board &attack = attacks_[to_index(row, col)];
        for(size_t i = row + 1; i < N; ) {
          attack[to_index(i++, col)] = 1;
        }

        for (size_t i = row; i > 0; ) {
          attack[to_index(--i, col)] = 1;
        }

        for(size_t j = col + 1; j < N; ) {
          attack[to_index(row, j++)] = 1;
        }

        for (size_t j = col; j > 0; ) {
          attack[to_index(row, --j)] = 1;
        }

        for(size_t i = row + 1, j = col + 1; i < N && j < N; ) {
          attack[to_index(i++, j++)] = 1;
        }

        for (size_t i = row, j = col; i > 0 && j > 0; ) {
          attack[to_index(--i, --j)] = 1;
        }

        for(size_t i = row + 1, j = col; i < N && j > 0; ) {
          attack[to_index(i++, --j)] = 1;
        }

        for (size_t i = row, j = col + 1; i > 0 && j < N; ) {
          attack[to_index(--i, j++)] = 1;
        }      
      }
    }
  }

  size_t col_by_row_[N];
  Board occupied_;
  std::vector<Board> attacks_;
};

}  // namespace nq


template <size_t N>
std::ostream &operator<<(std::ostream &os, nq::Queens<N> const &m) {
  os << "Queens[" << N << "] (attacks: " << m.num_attacks() << ")" << std::endl;
  for (auto &it: m.OccupiedRowCols()) {
    for (size_t col = 0; col < N; col++) {
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

#endif
