#include "progress.h"

#include <fmt/format.h>

#include <chrono>
#include <thread>

Progress::Progress(Printer& printer):
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

void Progress::PrintProgress()
{
    const auto percentage{0 < total_ ? (renamed_ * 100) / total_ : 0};
    const auto message{fmt::format("Renaming photos and videos {0}%", percentage)};
    printer_.PrintReplaceableMessage(message);
}

void Progress::Stop()
{
    stop_ = true;
}

void Progress::operator()()
{
    while(!stop_)
    {
        PrintProgress();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
    }

    PrintProgress();
}
