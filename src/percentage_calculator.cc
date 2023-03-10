#include "percentage_calculator.h"

#include <fmt/format.h>

std::string CalculatePercentage(size_t renamed, size_t total)
{
    const auto percentage{0 < total ? (renamed * 100) / total : 0};
    return fmt::format("{0}%", percentage);
}
