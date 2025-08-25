#include "dictool/Aglorithm.h"
#include <gtest/gtest.h>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace dictool;

// ---------------- transform (新字典) ----------------
TEST(DictUtilsTest, TransformNew) {
  std::map<int, int> input = {{1, 10}, {2, 20}, {3, 30}};
  auto output = transform<int, int, std::string>(
      input, [](int k, int v) { return std::to_string(k + v); });

  ASSERT_EQ(output.size(), 3);
  EXPECT_EQ(output[1], "11");
  EXPECT_EQ(output[2], "22");
  EXPECT_EQ(output[3], "33");
}

// ---------------- transform (原地修改) ----------------
TEST(DictUtilsTest, TransformInPlace) {
  std::map<int, int> input = {{1, 10}, {2, 20}};
  transform<int, int>(input, [](int k, int v) { return v * 2 + k; });

  EXPECT_EQ(input[1], 21); // (10*2 + 1)
  EXPECT_EQ(input[2], 42); // (20*2 + 2)
}

// ---------------- new_by_set ----------------
TEST(DictUtilsTest, NewBySet) {
  std::set<std::string> input = {"apple", "banana"};
  auto output = new_by_set<std::string, int>(
      input, [](const std::string &s) { return (int)s.size(); });

  ASSERT_EQ(output.size(), 2);
  EXPECT_EQ(output["apple"], 5);
  EXPECT_EQ(output["banana"], 6);
}

// ---------------- values_of ----------------
TEST(DictUtilsTest, ValuesOf) {
  std::map<int, std::string> input = {{1, "a"}, {2, "b"}, {3, "c"}};
  auto values = values_of<int, std::string>(input);

  ASSERT_EQ(values.size(), 3);
  EXPECT_NE(std::find(values.begin(), values.end(), "a"), values.end());
  EXPECT_NE(std::find(values.begin(), values.end(), "b"), values.end());
  EXPECT_NE(std::find(values.begin(), values.end(), "c"), values.end());
}

// ---------------- update: 插入新元素 ----------------
TEST(DictUtilsTest, UpdateInsert) {
  std::map<int, int> dict;
  auto it = update<int, int>(
      dict, 1, 100, [](int, int old_v, int new_v) { return old_v + new_v; });

  EXPECT_EQ(dict.size(), 1);
  EXPECT_EQ(dict[1], 100);
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, 100);
}

// ---------------- update: 更新已有元素 ----------------
TEST(DictUtilsTest, UpdateExisting) {
  std::map<int, int> dict = {{1, 50}};
  auto it = update<int, int>(
      dict, 1, 25, [](int, int old_v, int new_v) { return old_v + new_v; });

  EXPECT_EQ(dict.size(), 1);
  EXPECT_EQ(dict[1], 75);
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, 75);
}
