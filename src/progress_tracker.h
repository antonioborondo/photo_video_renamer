#pragma once

#include "printer.h"

class ProgressTracker
{
    size_t total_;

    size_t renamed_;

    bool stop_;

    Printer& printer_;

public:
    ProgressTracker(Printer& printer);

    void IncrementTotal(size_t amount);

    void IncrementRenamed(size_t amount);

    void PrintProgress();

    void Stop();

    void operator()();
};
