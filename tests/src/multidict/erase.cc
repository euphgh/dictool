
#include "fixture.h"

// Test erase by key
TEST_F(MultiMapTest, EraseByKey) {
  EXPECT_EQ(mm_int_int.erase(1), 2);
  EXPECT_EQ(mm_int_int.size(), 1);
  EXPECT_FALSE(mm_int_int.contains(1));
  EXPECT_TRUE(mm_int_int.contains(2));

  EXPECT_EQ(mm_int_int.erase(999), 0); // Non-existent key
}

// Test erase by iterator
TEST_F(MultiMapTest, EraseByIterator) {
  auto it = mm_int_int.find(1);
  ASSERT_NE(it, mm_int_int.end());

  it = mm_int_int.erase(it);
  EXPECT_EQ(mm_int_int.size(), 2);
  EXPECT_EQ(mm_int_int.value_count(1), 1);
}

// Test erase by key-value pair
TEST_F(MultiMapTest, EraseByKeyValue) {
  EXPECT_TRUE(mm_int_int.erase(1, 10));
  EXPECT_EQ(mm_int_int.size(), 2);
  EXPECT_EQ(mm_int_int.value_count(1), 1);

  EXPECT_FALSE(mm_int_int.erase(1, 999));  // Non-existent value
  EXPECT_FALSE(mm_int_int.erase(999, 10)); // Non-existent key
}
