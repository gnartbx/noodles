#ifndef NQ_QUEENS_H_
#define NQ_QUEENS_H_

#include <bitset>
#include <ostream>
#include <utility>
#include <vector>

#include <stdlib.h>

namespace nq {
  
class Queens {
  static constexpr size_t BITSET_SIZE = 64;
  using Board = std::vector<std::bitset<BITSET_SIZE>>;

 public:
  static Queens Create(size_t num_rows);

  size_t num_rows() const { return num_rows_; }
  size_t num_attacks() const;
  std::vector<std::pair<size_t, size_t>> OccupiedRowCols() const;

  void Swap(size_t row1, size_t row2);
  void Permute(size_t start_row, size_t end_row);

  void Randomize();

 protected:
  Queens(size_t num_rows);

 private:
  inline size_t to_index(size_t row, size_t col) const {
    return row * num_rows_ + col;
  }
  inline void set_occupied(Board& board, size_t row, size_t col, bool occupied = true) {
    div_t board_div = div(to_index(row, col), BITSET_SIZE);
    board[board_div.quot][board_div.rem] = occupied;
  }
  inline bool get_occupied(Board& board, size_t row, size_t col) {
    div_t board_div = div(to_index(row, col), BITSET_SIZE);
    return board[board_div.quot][board_div.rem];
  }
  const std::vector<Board>* InitializeAttacks();

  size_t num_rows_;
  std::vector<size_t> col_by_row_;
  Board occupied_;
  const std::vector<Board>* attacks_;
};

}  // namespace nq

std::ostream &operator<<(std::ostream &os, nq::Queens const &m);

#endif
