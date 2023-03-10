#include "progress.h"

#include "percentage_calculator.h"

#include <fmt/format.h>

#include <chrono>

Progress::Progress(const Printer& printer):
    total_{},
    renamed_{},
    stop_{},
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
}

void Progress::Stop()
{
    stop_ = true;
}

void Progress::operator()()
{
    do
    {
        printer_.PrintReplaceableMessage(CalculatePercentage(renamed_, total_));

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
    }
    while(!stop_);
    printer_.PrintReplaceableMessage(CalculatePercentage(renamed_, total_));
}
