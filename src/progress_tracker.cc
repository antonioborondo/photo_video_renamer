#include "progress_tracker.h"

#include <fmt/format.h>

#include <chrono>
#include <thread>

ProgressTracker::ProgressTracker(Printer& printer):
    total_{},
    renamed_{},
    stop_{},
    printer_{printer}
{
}

void ProgressTracker::IncrementTotal(size_t amount)
{
    total_ += amount;
}

void ProgressTracker::IncrementRenamed(size_t amount)
{
    renamed_ += amount;
}

void ProgressTracker::PrintProgress()
{
    const auto percentage{0 < total_ ? (renamed_ * 100) / total_ : 0};
    const auto message{fmt::format("Renaming {0}%", percentage)};
    printer_.PrintReplaceableMessage(message);
}

void ProgressTracker::Stop()
{
    stop_ = true;
}

void ProgressTracker::operator()()
{
    while(!stop_)
    {
        PrintProgress();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
    }

    PrintProgress();
}
