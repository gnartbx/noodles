#ifndef NQ_QUEENS_H_
#define NQ_QUEENS_H_

#include <ostream>
#include <vector>

#include <stdlib.h>

namespace nq {

class Queens {
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
  size_t num_rows_;
  std::vector<size_t> col_by_row_;
};

}  // namespace nq

std::ostream &operator<<(std::ostream &os, nq::Queens const &m);

#endif
