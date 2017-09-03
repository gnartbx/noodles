#include "queens.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using nq::Queens;

TEST(QueensTest, Two) {
  Queens<2UL> q;
  cout << "====(Initial)====" << endl << q << endl;
  EXPECT_EQ(2UL, q.num_attacks());
  q.SwapQueens(0, 1);
  cout << "====Swap(0,1)====" << endl << q << endl;
  EXPECT_EQ(2UL, q.num_attacks());
}

TEST(QueensTest, Three) {
  Queens<3UL> q;
  cout << "====(Initial)====" << endl << q << endl;
  EXPECT_EQ(6UL, q.num_attacks());

  q.SwapQueens(0, 1);
  cout << "====Swap(0,1)====" << endl << q << endl;
  EXPECT_EQ(2UL, q.num_attacks());
}

TEST(QueensTest, Four) {
  Queens<4UL> q;
  cout << "====(Initial)====" << endl << q << endl;
  
  EXPECT_EQ(12UL, q.num_attacks());
  q.SwapQueens(0, 3);
  cout << "====Swap(0,3)====" << endl << q << endl;
  EXPECT_EQ(4UL, q.num_attacks());

  q.SwapQueens(0, 1);
  cout << "====Swap(0,1)====" << endl << q << endl;

  q.SwapQueens(2, 3);
  cout << "====Swap(2,3)====" << endl << q << endl;
  EXPECT_EQ(0UL, q.num_attacks());
}
