#include "dictool/Aglorithm.h"
#include "gmock/gmock.h"
#include <gtest/gtest.h>

using namespace dictool;

TEST(ReverseMapTest, reverse) {
  std::map<std::string, size_t> in = {
      {"hello", 1},
      {"world", 1},
      {"!", 2},
  };
  auto res = dictool::reverse(in);
  EXPECT_THAT(res[1], ::testing::UnorderedElementsAre("hello", "world"));
  EXPECT_THAT(res[2], ::testing::ElementsAre("!"));
}

// 基础测试：整数map
TEST(ReverseMapTest, IntToInt) {
  std::map<int, int> input = {{1, 10}, {2, 20}, {3, 10}};
  auto output = reverse(input);

  // 10 -> {1,3}, 20 -> {2}
  ASSERT_EQ(output.size(), 2);
  ASSERT_EQ(output[10].size(), 2);
  EXPECT_EQ(output[10][0], 1);
  EXPECT_EQ(output[10][1], 3);
  ASSERT_EQ(output[20].size(), 1);
  EXPECT_EQ(output[20][0], 2);
}

// 测试：string key, int value
TEST(ReverseMapTest, StringToInt) {
  std::map<std::string, int> input = {{"apple", 1}, {"banana", 2}, {"pear", 1}};
  auto output = reverse(input);

  ASSERT_EQ(output.size(), 2);
  ASSERT_EQ(output[1].size(), 2);
  EXPECT_EQ(output[1][0], "apple");
  EXPECT_EQ(output[1][1], "pear");
  ASSERT_EQ(output[2].size(), 1);
  EXPECT_EQ(output[2][0], "banana");
}

// 测试：unordered_map
TEST(ReverseMapTest, UnorderedMap) {
  std::unordered_map<int, char> input = {{1, 'a'}, {2, 'b'}, {3, 'a'}};
  auto output = reverse(input);

  ASSERT_EQ(output.size(), 2);
  ASSERT_EQ(output['a'].size(), 2);
  ASSERT_EQ(output['b'].size(), 1);
  EXPECT_TRUE((output['a'][0] == 1 && output['a'][1] == 3) ||
              (output['a'][0] == 3 && output['a'][1] == 1)); // 无序
  EXPECT_EQ(output['b'][0], 2);
}

// 边界情况：空map
TEST(ReverseMapTest, EmptyInput) {
  std::map<int, int> input;
  auto output = reverse(input);
  EXPECT_TRUE(output.empty());
}
