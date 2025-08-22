#include "fixture.h"

// Test keys method
TEST_F(MultiMapTest, Keys) {
  auto keys = mm_int_int.keys();
  EXPECT_EQ(keys.size(), 2);
  EXPECT_TRUE(std::find(keys.begin(), keys.end(), 1) != keys.end());
  EXPECT_TRUE(std::find(keys.begin(), keys.end(), 2) != keys.end());
}

// Test clear method
TEST_F(MultiMapTest, Clear) {
  EXPECT_FALSE(mm_int_int.empty());
  mm_int_int.clear();
  EXPECT_TRUE(mm_int_int.empty());
  EXPECT_EQ(mm_int_int.size(), 0);
}

// Test merge method
TEST_F(MultiMapTest, Merge) {
  MultiMap<int, int> mm1;
  mm1.emplace(1, 10);
  mm1.emplace(2, 20);

  MultiMap<int, int> mm2;
  mm2.emplace(1, 30);
  mm2.emplace(3, 40);

  mm1.merge(mm2);

  EXPECT_EQ(mm1.size(), 4);
  EXPECT_EQ(mm1.value_count(1), 2);
  EXPECT_EQ(mm1.value_count(2), 1);
  EXPECT_EQ(mm1.value_count(3), 1);
}

// Test copy semantics
TEST_F(MultiMapTest, CopySemantics) {
  MultiMap<int, int> copy = mm_int_int;
  EXPECT_EQ(copy.size(), mm_int_int.size());
  EXPECT_EQ(copy.key_count(), mm_int_int.key_count());

  // Modify original, copy should not change
  mm_int_int.emplace(3, 40);
  EXPECT_EQ(copy.size(), 3);
  EXPECT_EQ(mm_int_int.size(), 4);
}

// Test move semantics
TEST_F(MultiMapTest, MoveSemantics) {
  MultiMap<int, int> moved = std::move(mm_int_int);
  EXPECT_EQ(moved.size(), 3);
  EXPECT_TRUE(mm_int_int.empty());
}

// Test string keys and values
TEST_F(MultiMapTest, StringTypes) {
  EXPECT_EQ(mm_str_str.size(), 3);
  EXPECT_EQ(mm_str_str.value_count("key1"), 2);
  EXPECT_EQ(mm_str_str.value_count("key2"), 1);

  auto values = mm_str_str.get("key1");
  EXPECT_TRUE(std::find(values.begin(), values.end(), "value1") !=
              values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), "value2") !=
              values.end());
}

// Test edge cases
TEST_F(MultiMapTest, EdgeCases) {
  MultiMap<int, int> mm;

  // Empty multimap operations
  EXPECT_TRUE(mm.empty());
  EXPECT_EQ(mm.find(1), mm.end());
  EXPECT_EQ(mm.count(1), 0);
  EXPECT_TRUE(mm.get(1).empty());

  // Erase from empty multimap
  EXPECT_EQ(mm.erase(1), 0);

  // Clear empty multimap
  EXPECT_NO_THROW(mm.clear());
}

// Test value_count for non-existent key
TEST_F(MultiMapTest, ValueCountNonExistentKey) {
  EXPECT_EQ(mm_int_int.value_count(999), 0);
}

// Test print methods (mostly for coverage)
TEST_F(MultiMapTest, PrintMethods) {
  testing::internal::CaptureStdout();
  mm_int_int.print_stats();
  std::string stats_output = testing::internal::GetCapturedStdout();
  EXPECT_FALSE(stats_output.empty());

  testing::internal::CaptureStdout();
  mm_int_int.print_all();
  std::string all_output = testing::internal::GetCapturedStdout();
  EXPECT_FALSE(all_output.empty());
}
