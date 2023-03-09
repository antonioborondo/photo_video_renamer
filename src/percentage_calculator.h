#pragma once

#include <string>

enum class PercentageCalculatorMode
{
    kDefault,
    kTempRenaming,
    kFinalRenaming,
};

class PercentageCalculator
{
    PercentageCalculatorMode mode_;

public:
    PercentageCalculator();

    void SetMode(PercentageCalculatorMode mode);

    std::string CalculatePercentage(size_t renamed, size_t total);
};
