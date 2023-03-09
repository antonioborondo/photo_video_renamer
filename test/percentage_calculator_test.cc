#include "percentage_calculator.h"

#include <gtest/gtest.h>

class PercentageCalculatorTest: public testing::Test
{
protected:
    PercentageCalculator percentage_calculator_;
};

TEST_F(PercentageCalculatorTest, CalculateDefaultPercentage)
{
    ASSERT_EQ("0%", percentage_calculator_.CalculatePercentage(0, 100));
    ASSERT_EQ("50%", percentage_calculator_.CalculatePercentage(50, 100));
    ASSERT_EQ("100%", percentage_calculator_.CalculatePercentage(100, 100));
}

TEST_F(PercentageCalculatorTest, CalculateTempRenamingPercentage)
{
    percentage_calculator_.SetMode(PercentageCalculatorMode::kTempRenaming);

    ASSERT_EQ("0%", percentage_calculator_.CalculatePercentage(0, 100));
    ASSERT_EQ("25%", percentage_calculator_.CalculatePercentage(50, 100));
    ASSERT_EQ("50%", percentage_calculator_.CalculatePercentage(100, 100));
}

TEST_F(PercentageCalculatorTest, CalculateFinalRenamingPercentage)
{
    percentage_calculator_.SetMode(PercentageCalculatorMode::kFinalRenaming);

    ASSERT_EQ("50%", percentage_calculator_.CalculatePercentage(0, 100));
    ASSERT_EQ("75%", percentage_calculator_.CalculatePercentage(50, 100));
    ASSERT_EQ("100%", percentage_calculator_.CalculatePercentage(100, 100));
}
