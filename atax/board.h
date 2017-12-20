#ifndef ATAX_BOARD_H_
#define ATAX_BOARD_H_

#include <bitset>
#include <ostream>
#include <tuple>
#include <vector>

#include <stdlib.h>

namespace atax {

class Board {
 public:
  enum class Piece { None, Pawn, Knight, Bishop, Rook, Queen, King };

  static constexpr size_t kBoardSize = 8;
  static constexpr size_t kNumPieces = 9;

  using BoardFreeSquares = std::bitset<kBoardSize * kBoardSize>;

  static Board Create();

  size_t num_unattacked() const;

  bool Move(size_t piece_index, size_t row, size_t col);
  void Permute(size_t start_piece, size_t end_piece);
  std::vector<std::tuple<size_t, size_t, Board::Piece>> OccupiedRowCols() const;
  Piece GetPiece(size_t row, size_t col) const;

  void Randomize();

 protected:
  Board();

 private:
  bool IsValidMove(size_t piece_index, size_t row, size_t col) const;
  void KnightMoves(BoardFreeSquares &free, size_t row, size_t col) const;
  void PawnMoves(BoardFreeSquares &free, size_t row, size_t col) const;
  void KingMoves(BoardFreeSquares &free, size_t row, size_t col) const;
  void BishopMoves(BoardFreeSquares &free, size_t row, size_t col) const;
  void RookMoves(BoardFreeSquares &free, size_t row, size_t col) const;
  void FixBoard();

  Piece by_square_[kBoardSize * kBoardSize];
  size_t by_piece_[kNumPieces];
};

}  // namespace atax

std::ostream &operator<<(std::ostream &os, atax::Board const &m);

#endif  // ATAX_BOARD_H_
