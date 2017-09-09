#include "queens.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using nq::Queens;

TEST(QueensTest, Two) {
  Queens q = Queens::Create(2);
  cout << "====(Initial)====" << endl << q << endl;
  EXPECT_EQ(2UL, q.num_attacks());
  q.Swap(0, 1);
  cout << "====Swap(0,1)====" << endl << q << endl;
  EXPECT_EQ(2UL, q.num_attacks());
}

TEST(QueensTest, Three) {
  Queens q = Queens::Create(3);
  cout << "====(Initial)====" << endl << q << endl;
  EXPECT_EQ(6UL, q.num_attacks());

  q.Swap(0, 1);
  cout << "====Swap(0,1)====" << endl << q << endl;
  EXPECT_EQ(2UL, q.num_attacks());
}

TEST(QueensTest, Four) {
  Queens q = Queens::Create(4);
  cout << "====(Initial)====" << endl << q << endl;
  
  EXPECT_EQ(12UL, q.num_attacks());
  q.Swap(0, 3);
  cout << "====Swap(0,3)====" << endl << q << endl;
  EXPECT_EQ(4UL, q.num_attacks());

  q.Swap(0, 1);
  cout << "====Swap(0,1)====" << endl << q << endl;

  q.Swap(2, 3);
  cout << "====Swap(2,3)====" << endl << q << endl;
  EXPECT_EQ(0UL, q.num_attacks());
}
