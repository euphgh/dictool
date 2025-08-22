#include "dictool/MultiDict.h"
#include <gtest/gtest.h>

using namespace dictool;
// Test fixture for MultiMap tests
class MultiMapTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Set up test data
    mm_int_int.emplace(1, 10);
    mm_int_int.emplace(1, 20);
    mm_int_int.emplace(2, 30);

    mm_str_str.emplace("key1", "value1");
    mm_str_str.emplace("key1", "value2");
    mm_str_str.emplace("key2", "value3");
  }

  void TearDown() override {
    // Clean up if needed
  }

  MultiMap<int, int> mm_int_int;
  MultiMap<std::string, std::string> mm_str_str;
  MultiMap<int, std::string> empty_mm;
};