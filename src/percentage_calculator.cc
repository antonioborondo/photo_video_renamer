#include "percentage_calculator.h"

#include <fmt/format.h>

PercentageCalculator::PercentageCalculator():
    mode_{PercentageCalculatorMode::kDefault}
{
}

void PercentageCalculator::SetMode(PercentageCalculatorMode mode)
{
    mode_ = mode;
}

std::string PercentageCalculator::CalculatePercentage(size_t renamed, size_t total)
{
    size_t real_renamed{};
    size_t real_total{};

    switch(mode_)
    {
        case PercentageCalculatorMode::kTempRenaming:
        {
            real_renamed = renamed;
            real_total = total * 2;
            break;
        }
        case PercentageCalculatorMode::kFinalRenaming:
        {
            real_renamed = total + renamed;
            real_total = total * 2;
            break;
        }
        default:
        {
            real_renamed = renamed;
            real_total = total;
            break;
        }
    }

    const auto percentage{(real_renamed * 100) / real_total};
    return fmt::format("{:<4}", fmt::format("{}%", percentage));
}
