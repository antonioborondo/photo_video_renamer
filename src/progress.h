#pragma once

#include "printer.h"

#include <mutex>

class Progress
{
    size_t total_;

    size_t renamed_;

    bool stop_;

    Printer printer_;

public:
    Progress(const Printer& printer);

    void IncrementTotal(size_t amount);

    void IncrementRenamed(size_t amount);

    void Stop();

    void operator()();
};
