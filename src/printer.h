#pragma once

#include <ostream>

class Printer
{
    std::ostream& sink_;

    size_t last_number_printed_characters_;

public:
    Printer(std::ostream& sink);

    void Print(const std::string& message);
};
