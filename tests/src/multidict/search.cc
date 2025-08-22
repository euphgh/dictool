#include "fixture.h"
#include <gmock/gmock.h>

// Test contains method
TEST_F(MultiMapTest, Contains) {
  EXPECT_TRUE(mm_int_int.contains(1));
  EXPECT_TRUE(mm_int_int.contains(2));
  EXPECT_FALSE(mm_int_int.contains(3));
}

// Test get method
TEST_F(MultiMapTest, Get) {
  auto container = mm_int_int.get(1);
  EXPECT_EQ(container.size(), 2);
  EXPECT_TRUE(std::find(container.begin(), container.end(), 10) !=
              container.end());
  EXPECT_TRUE(std::find(container.begin(), container.end(), 20) !=
              container.end());

  auto empty_container = mm_int_int.get(999);
  EXPECT_TRUE(empty_container.empty());
}

// Test operator[]
TEST_F(MultiMapTest, OperatorBracket) {
  auto &container = mm_int_int[1];
  EXPECT_EQ(container.size(), 2);

  // Test creating new key
  auto &new_container = mm_int_int[3];
  EXPECT_TRUE(new_container.empty());
  new_container.push_back(100);
  EXPECT_EQ(mm_int_int.value_count(3), 1);
}

// Test at method
TEST_F(MultiMapTest, At) {
  auto &container = mm_int_int.at(1);
  EXPECT_EQ(container.size(), 2);

  EXPECT_THROW(auto foo = mm_int_int.at(999), std::out_of_range);
}

// Test size and empty methods
TEST_F(MultiMapTest, SizeAndEmpty) {
  EXPECT_FALSE(mm_int_int.empty());
  EXPECT_EQ(mm_int_int.size(), 3);
  EXPECT_EQ(mm_int_int.key_count(), 2);
  EXPECT_EQ(mm_int_int.total_value_count(), 3);

  EXPECT_TRUE(empty_mm.empty());
  EXPECT_EQ(empty_mm.size(), 0);
}

// Test count method
TEST_F(MultiMapTest, Count) {
  EXPECT_EQ(mm_int_int.count(1), 2);
  EXPECT_EQ(mm_int_int.count(2), 1);
  EXPECT_EQ(mm_int_int.count(3), 0);
}

// Test find method
TEST_F(MultiMapTest, Find) {
  auto it = mm_int_int.find(1);
  EXPECT_NE(it, mm_int_int.end());
  EXPECT_EQ(it.key(), 1);

  it = mm_int_int.find(999);
  EXPECT_EQ(it, mm_int_int.end());
}

// Test equal_range method
TEST_F(MultiMapTest, EqualRange) {
  auto range = mm_int_int.equal_range(1);
  int count = 0;
  for (auto it = range.first; it != range.second; ++it) {
    ++count;
    EXPECT_EQ(it.key(), 1);
  }
  EXPECT_EQ(count, 2);

  auto empty_range = mm_int_int.equal_range(999);
  EXPECT_EQ(empty_range.first, empty_range.second);

  // use new instance to test
  MultiMap<int, char> mm;
  mm.insert({1, 'a'});
  mm.insert({1, 'b'});
  mm.insert({2, 'c'});

  auto [first, last] = mm.equal_range(1);
  std::vector<char> values;
  for (auto it = first; it != last; ++it) {
    values.push_back(it.value());
  }
  EXPECT_EQ(values.size(), 2);
  EXPECT_THAT(values, ::testing::ElementsAre('a', 'b'));
}