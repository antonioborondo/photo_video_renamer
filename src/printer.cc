#include "printer.h"

Printer::Printer(std::ostream& sink):
    sink_{sink},
    last_number_printed_characters_{}
{
}

void Printer::Print(const std::string& message)
{
    sink_ << std::string(last_number_printed_characters_, '\b');

    sink_ << message;

    last_number_printed_characters_ = message.length();
}
