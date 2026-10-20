#include <gtest/gtest.h>
#include "hello_space/gap_checker.hpp"

using hello_space::is_gap_too_long;

TEST(GapChecker, NormalOneSecondGapIsFine)
{
  EXPECT_FALSE(is_gap_too_long(10.0, 11.0, 1.5));
}

TEST(GapChecker, HiccupGapIsFlagged)
{
  EXPECT_TRUE(is_gap_too_long(10.0, 11.9, 1.5));
}

TEST(GapChecker, ExactlyAtLimitIsNotFlagged)
{
  EXPECT_FALSE(is_gap_too_long(10.0, 11.5, 1.5));
}
