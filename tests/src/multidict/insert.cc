#include "fixture.h"
#include <gmock/gmock.h>

using namespace dictool;

// Test emplace method
TEST_F(MultiMapTest, Emplace) {
  MultiMap<int, std::string> mm_int_str;
  mm_int_str.emplace(1, "apple");
  mm_int_str.emplace(1, "banana");
  mm_int_str.emplace(2, "cherry");

  EXPECT_EQ(mm_int_str.size(), 3);
  EXPECT_EQ(mm_int_str.value_count(1), 2);
  EXPECT_EQ(mm_int_str.value_count(2), 1);

  MultiMap<int, int> mm_int_int;
  mm_int_int.emplace(1, 42);
  mm_int_int.emplace(1, 99);

  EXPECT_EQ(mm_int_int.size(), 2);
  EXPECT_EQ(mm_int_int.count(1), 2);
  EXPECT_EQ(mm_int_int.total_value_count(), 2);
}

TEST_F(MultiMapTest, InsertAndFind) {
  MultiMap<int, std::string> mm;
  mm.insert({1, "a"});
  mm.insert({1, "b"});
  mm.insert({2, "c"});

  EXPECT_EQ(mm.size(), 3);
  EXPECT_TRUE(mm.contains(1));
  EXPECT_EQ(mm.count(1), 2);

  auto it = mm.find(1);
  ASSERT_NE(it, mm.end());
  EXPECT_EQ(it.key(), 1);
}

TEST_F(MultiMapTest, EmptyAndClear) {
  MultiMap<int, int> mm;
  EXPECT_TRUE(mm.empty());
  mm.insert({1, 10});
  EXPECT_FALSE(mm.empty());
  mm.clear();
  EXPECT_TRUE(mm.empty());
}

// Test insert method
TEST_F(MultiMapTest, Insert) {
  MultiMap<int, std::string> mm;
  mm.insert({1, "apple"});
  mm.insert(std::make_pair(1, "banana"));
  mm.insert(std::pair<int, std::string>(2, "cherry"));

  EXPECT_EQ(mm.size(), 3);
  EXPECT_EQ(mm.value_count(1), 2);
  EXPECT_EQ(mm.value_count(2), 1);
}
