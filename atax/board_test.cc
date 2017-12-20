#include "board.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using atax::Board;

TEST(BoardTest, Create) {
  Board b = Board::Create();
  cout << "====(Initial)====" << endl << b << endl;
  //  EXPECT_EQ(2UL, b.num_unattacked());
}

TEST(BoardTest, Free) {
  Board b = Board::Create();
  b.Move(1, 7, 7);  // Rb1-h8
  b.Move(2, 4, 1);  // Nc1-b5
  b.Move(3, 6, 2);  // Nd1-c7
  b.Move(4, 2, 2);  // Be1-c3
  b.Move(5, 7, 4);  // Bf1-e8
  b.Move(6, 4, 4);  // Qg1-e5
  b.Move(7, 2, 6);  // Kh1-g3
  b.Move(8, 1, 2);  // Pa2-c2
  cout << "====(Moved)====" << endl << b << endl;
}
/*TEST(BoardTest, Copying) {
  Board b1 = Board::Create();
  Board b2 = b1;

  b1.Permute(0, 2);

  }*/
