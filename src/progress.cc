#include "progress.h"

#include "percentage_calculator.h"

#include <fmt/format.h>

Progress::Progress(const Printer& printer):
    total_{},
    renamed_{},
    printer_{printer}
{
}

void Progress::IncrementTotal(size_t amount)
{
    total_ += amount;
}

void Progress::IncrementRenamed(size_t amount)
{
    renamed_ += amount;

    printer_.PrintReplaceableMessage(CalculatePercentage(renamed_, total_));
}
