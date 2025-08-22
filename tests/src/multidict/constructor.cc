#include "fixture.h"

// Test default constructor
TEST_F(MultiMapTest, DefaultConstructor) {
  MultiMap<int, int> mm;
  EXPECT_TRUE(mm.empty());
  EXPECT_EQ(mm.size(), 0);
  EXPECT_EQ(mm.key_count(), 0);
}

// Test initializer list constructor
TEST_F(MultiMapTest, InitializerListConstructor) {
  MultiMap<int, int> mm = {
      {1, 10},
      {1, 20},
      {2, 30},
  };

  EXPECT_FALSE(mm.empty());
  EXPECT_EQ(mm.size(), 3);
  EXPECT_EQ(mm.key_count(), 2);
  EXPECT_TRUE(mm.contains(1));
  EXPECT_TRUE(mm.contains(2));
}