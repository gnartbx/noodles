#ifndef NQ_QUEENS_H_
#define NQ_QUEENS_H_

#include <bitset>
#include <ostream>
#include <utility>
#include <vector>

namespace nq {

class Queens {

 public:
  static Queens Create(size_t num_rows);

  size_t num_rows() const { return num_rows_; }
  size_t num_attacks() const;
  std::vector<std::pair<size_t, size_t>> OccupiedRowCols() const;

  void Swap(size_t row1, size_t row2);

 protected:
  Queens(size_t num_rows);

 private:
  inline size_t to_index(size_t row, size_t col) const {
    return row * num_rows_ + col;
  }
  void InitializeAttacks();

  size_t num_rows_;
  std::vector<size_t> col_by_row_;
  std::vector<bool> occupied_;
  std::vector<std::vector<bool>> attacks_;
};

}  // namespace nq

std::ostream &operator<<(std::ostream &os, nq::Queens const &m);

#endif
