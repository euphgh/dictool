#include "fixture.h"

// -------- 使用 std::set 容器 --------
TEST(MultiMapSetTest, UniqueValues) {
  MultiMap<int, int, std::set> mm;
  mm.insert({1, 10});
  mm.insert({1, 10}); // set 不允许重复
  mm.insert({1, 20});

  EXPECT_EQ(mm.count(1), 2);
  auto container = mm.get(1);
  EXPECT_EQ(container.size(), 2);
  EXPECT_TRUE(container.find(10) != container.end());
  EXPECT_TRUE(container.find(20) != container.end());
}
