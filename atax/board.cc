#include "board.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include <string.h>

namespace atax {

/* static */
Board Board::Create() { return Board(); }

constexpr Board::Piece PIECES[Board::kNumPieces] = {
    Board::Piece::Rook,   Board::Piece::Rook,   Board::Piece::Knight,
    Board::Piece::Knight, Board::Piece::Bishop, Board::Piece::Bishop,
    Board::Piece::Queen,  Board::Piece::King,   Board::Piece::Pawn};

Board::Board() {
  for (size_t square_index = 0; square_index < kNumPieces; square_index++) {
    by_piece_[square_index] = square_index;
  }
  FixBoard();
}

void Board::FixBoard() {
  memset(by_square_, 0, sizeof(by_square_));
  for (size_t piece_index = 0; piece_index < kNumPieces; piece_index++) {
    by_square_[by_piece_[piece_index]] = PIECES[piece_index];
  }
}

void Update(const std::vector<std::pair<int, int>> &deltas,
            Board::BoardFreeSquares &free_squares, size_t row, size_t col) {
  for (const auto &delta : deltas) {
    int new_row = (int)row + delta.first;
    int new_col = (int)col + delta.second;
    if (new_row < (int)Board::kBoardSize && new_row >= 0 &&
        new_col < (int)Board::kBoardSize && new_col >= 0) {
      free_squares.set(new_row * Board::kBoardSize + new_col, 0);
    }
  }
}

void Board::KnightMoves(Board::BoardFreeSquares &free_squares, size_t row,
                        size_t col) const {
  static std::vector<std::pair<int, int>> kKnightDeltas = {
      {1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
  Update(kKnightDeltas, free_squares, row, col);
}

void Board::PawnMoves(Board::BoardFreeSquares &free_squares, size_t row,
                      size_t col) const {
  if (row + 1 < kBoardSize) {
    static std::vector<std::pair<int, int>> kPawnDeltas = {{1, -1}, {1, 1}};
    Update(kPawnDeltas, free_squares, row, col);
  }
}

void Board::KingMoves(Board::BoardFreeSquares &free_squares, size_t row,
                      size_t col) const {
  static std::vector<std::pair<int, int>> kKingDeltas = {
      {1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1}};
  Update(kKingDeltas, free_squares, row, col);
}

void Board::BishopMoves(Board::BoardFreeSquares &free_squares, size_t row,
                        size_t col) const {
  for (int cur_row = (int)row + 1, cur_col = (int)col + 1;
       cur_row < (int)kBoardSize && cur_col < (int)kBoardSize;
       cur_row++, cur_col++) {
    size_t square_index = cur_row * kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }

  for (int cur_row = (int)row - 1, cur_col = (int)col + 1;
       cur_row >= 0 && cur_col < (int)kBoardSize; cur_row--, cur_col++) {
    size_t square_index = cur_row * kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }

  for (int cur_row = (int)row + 1, cur_col = (int)col - 1;
       cur_row < (int)kBoardSize && cur_col >= 0; cur_row++, cur_col--) {
    size_t square_index = cur_row * kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }

  for (int cur_row = (int)row - 1, cur_col = (int)col - 1;
       cur_row >= 0 && cur_col >= 0; cur_row--, cur_col--) {
    size_t square_index = cur_row * kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }
}

void Board::RookMoves(Board::BoardFreeSquares &free_squares, size_t row,
                      size_t col) const {
  for (int cur_row = (int)row + 1, cur_col = col;
       cur_row < (int)Board::kBoardSize; cur_row++) {
    size_t square_index = cur_row * Board::kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }

  for (int cur_row = (int)row - 1, cur_col = col; cur_row >= 0; cur_row--) {
    size_t square_index = cur_row * Board::kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }

  for (int cur_row = row, cur_col = (int)col + 1;
       cur_col < (int)Board::kBoardSize; cur_col++) {
    size_t square_index = cur_row * Board::kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }

  for (int cur_row = row, cur_col = (int)col - 1; cur_col >= 0; cur_col--) {
    size_t square_index = cur_row * Board::kBoardSize + cur_col;
    free_squares.set(square_index, 0);
    if (by_square_[square_index] != Board::Piece::None) {
      break;
    }
  }
}

size_t Board::num_unattacked() const {
  auto final_free_squares = BoardFreeSquares().set();

  for (size_t index = 0; index < kBoardSize * kBoardSize; index++) {
    auto free_squares = BoardFreeSquares().set();
    size_t row = index / kBoardSize;
    size_t col = index % kBoardSize;
    switch (by_square_[index]) {
      case Piece::None:
        break;
      case Piece::Knight:
        KnightMoves(free_squares, row, col);
        break;
      case Piece::Bishop:
        BishopMoves(free_squares, row, col);
        break;
      case Piece::Rook:
        RookMoves(free_squares, row, col);
        break;
      case Piece::Queen:
        RookMoves(free_squares, row, col);
        BishopMoves(free_squares, row, col);
        break;
      case Piece::King:
        KingMoves(free_squares, row, col);
        break;
      case Piece::Pawn:
        PawnMoves(free_squares, row, col);
        break;
    }
    if (by_square_[index] != Piece::None) {
      final_free_squares &= free_squares;
      // std::cout << "... num_unattacked(row=" << row << ", col=" << col
      //           << "): " << free_squares.count()
      //           << ", running_total: " << final_free_squares.count()
      //           << std::endl;
    }
  }
  return final_free_squares.count();
}

bool Board::IsValidMove(size_t piece_index, size_t row, size_t col) const {
  switch (PIECES[piece_index]) {
    case Piece::Pawn:
      return (row > 0 && row + 2 < kBoardSize);
    case Piece::Bishop: {
      size_t source_row = by_piece_[piece_index] / kBoardSize;
      size_t source_col = by_piece_[piece_index] % kBoardSize;
      return ((source_row + source_col) % 2) == ((row + col) % 2);
    }
    default:
      break;
  }
  return true;
}

bool Board::Move(size_t piece_index, size_t row, size_t col) {
  if (!IsValidMove(piece_index, row, col)) {
    return false;
  }

  by_piece_[piece_index] = row * kBoardSize + col;
  FixBoard();
  return true;
}

void Board::Permute(size_t start_piece_index, size_t end_piece_index) {
  size_t min_piece_index = std::min(start_piece_index, end_piece_index);
  size_t max_piece_index = std::max(start_piece_index, end_piece_index);
  size_t first_piece_square = by_piece_[min_piece_index];

  for (size_t index = min_piece_index; index <= max_piece_index; ++index) {
    size_t next_square =
        (index < max_piece_index) ? by_piece_[index + 1] : first_piece_square;
    if (IsValidMove(index, next_square / kBoardSize,
                    next_square % kBoardSize)) {
      by_piece_[index] = next_square;
    }
  }
  FixBoard();
}

std::vector<std::tuple<size_t, size_t, Board::Piece>> Board::OccupiedRowCols()
    const {
  std::vector<std::tuple<size_t, size_t, Board::Piece>> result;

  for (size_t index = 0; index < kNumPieces; index++) {
    size_t square = by_piece_[index];
    result.emplace_back(square / kBoardSize, square % kBoardSize,
                        PIECES[index]);
  }
  return result;
}

Board::Piece Board::GetPiece(size_t row, size_t col) const {
  return by_square_[row * kBoardSize + col];
}

void Board::Randomize() {
  // Ideas? Generate random squares for each piece, check validity, move
  // them?
}

}  // namespace atax

std::ostream &operator<<(std::ostream &os, atax::Board const &b) {
  os << "Board (free: " << b.num_unattacked() << ")" << std::endl;
  for (size_t row = atax::Board::kBoardSize; row > 0; row--) {
    for (size_t col = 0; col < atax::Board::kBoardSize; col++) {
      switch (b.GetPiece(row - 1, col)) {
        case atax::Board::Piece::None:
          os << (((row + col) % 2) ? "." : " ");
          break;
        case atax::Board::Piece::King:
          os << "K";
          break;
        case atax::Board::Piece::Queen:
          os << "Q";
          break;
        case atax::Board::Piece::Rook:
          os << "R";
          break;
        case atax::Board::Piece::Bishop:
          os << "B";
          break;
        case atax::Board::Piece::Knight:
          os << "N";
          break;
        case atax::Board::Piece::Pawn:
          os << "P";
          break;
      }
    }
    os << std::endl;
  }
  return os;
}
