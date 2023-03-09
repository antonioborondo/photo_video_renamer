#include "printer.h"

#include <gtest/gtest.h>

#include <sstream>

class PrinterTest: public testing::Test
{
protected:
    std::ostringstream sink_;

    Printer printer_;

    PrinterTest():
        printer_{sink_}
    {
    }

    void SetUp() override
    {
        sink_.str("");
        sink_.clear();
    }
};

TEST_F(PrinterTest, PrintMessage)
{
    printer_.Print("a");
    ASSERT_EQ("a", sink_.str());
}

TEST_F(PrinterTest, PrintMultipleMessages)
{
    printer_.Print("a");
    ASSERT_EQ("a", sink_.str());

    printer_.Print("ab");
    ASSERT_EQ("a\bab", sink_.str());

    printer_.Print("abc");
    ASSERT_EQ("a\bab\b\babc", sink_.str());
}
