#pragma once

#include <iostream>
#include <ostream>

class Printer
{
    std::ostream& sink_;

    size_t last_number_replaceable_characters_;

public:
    Printer(std::ostream& sink = std::cout);

    void PrintMessage(const std::string& message);

    void PrintReplaceableMessage(const std::string& message);
};
