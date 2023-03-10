#pragma once

#include "printer.h"

class Progress
{
    size_t total_;

    size_t renamed_;

    Printer printer_;

public:
    Progress(const Printer& printer);

    void IncrementTotal(size_t amount);

    void IncrementRenamed(size_t amount);
};
