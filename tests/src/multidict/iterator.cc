#include "fixture.h"
#include <gmock/gmock.h>

// -------- 迭代器 --------
TEST_F(MultiMapTest, Iteration) {
  MultiMap<int, char> mm;
  mm.insert({1, 'a'});
  mm.insert({2, 'b'});
  mm.insert({3, 'c'});

  std::vector<char> values;
  for (auto it = mm.begin(); it != mm.end(); ++it) {
    values.push_back(it.value());
  }
  EXPECT_THAT(values, ::testing::UnorderedElementsAre('a', 'b', 'c'));
}

// Test iterator functionality
TEST_F(MultiMapTest, FixIteration) {
  int count = 0;
  for (auto it = mm_int_int.begin(); it != mm_int_int.end(); ++it) {
    ++count;
    EXPECT_TRUE(it.key() == 1 || it.key() == 2);
    EXPECT_TRUE(it.value() == 10 || it.value() == 20 || it.value() == 30);
  }
  EXPECT_EQ(count, 3);
}

// Test range-based for loop
TEST_F(MultiMapTest, RangeBasedForLoop) {
  int count = 0;
  for (const auto &[key, value] : mm_int_int) {
    ++count;
    EXPECT_TRUE(key == 1 || key == 2);
    EXPECT_TRUE(value == 10 || value == 20 || value == 30);
  }
  EXPECT_EQ(count, 3);
}

// Test iterator equality
TEST_F(MultiMapTest, IteratorEquality) {
  auto it1 = mm_int_int.begin();
  auto it2 = mm_int_int.begin();
  EXPECT_EQ(it1, it2);

  ++it1;
  EXPECT_NE(it1, it2);

  auto end_it = mm_int_int.end();
  EXPECT_NE(it1, end_it);
}
