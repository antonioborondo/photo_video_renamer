#include "percentage_calculator.h"

#include <gtest/gtest.h>

TEST(PercentageCalculatorTest, CalculatePercentage)
{
    ASSERT_EQ("0%", CalculatePercentage(0, 100));
    ASSERT_EQ("50%", CalculatePercentage(50, 100));
    ASSERT_EQ("100%", CalculatePercentage(100, 100));
}
