#include "printer.h"

Printer::Printer(std::ostream& sink):
    sink_{sink},
    last_number_replaceable_characters_{}
{
}

void Printer::PrintMessage(const std::string& message)
{
    sink_ << message;

    last_number_replaceable_characters_ = 0;
}

void Printer::PrintReplaceableMessage(const std::string& message)
{
    sink_ << std::string(last_number_replaceable_characters_, '\b');

    sink_ << message;

    last_number_replaceable_characters_ = message.length();
}
