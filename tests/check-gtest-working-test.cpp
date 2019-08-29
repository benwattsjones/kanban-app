#include "gtest/gtest.h"

TEST(GTestTestSuite, CheckGTestIsWorking)
{
    int result = 3 + 4;
    ASSERT_EQ(7, result);
}

