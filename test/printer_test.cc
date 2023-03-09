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
    printer_.PrintMessage("a");
    ASSERT_EQ("a", sink_.str());
}

TEST_F(PrinterTest, PrintMultipleMessage)
{
    printer_.PrintMessage("a");
    ASSERT_EQ("a", sink_.str());

    printer_.PrintMessage("ab");
    ASSERT_EQ("aab", sink_.str());

    printer_.PrintMessage("abc");
    ASSERT_EQ("aababc", sink_.str());
}

TEST_F(PrinterTest, PrintReplaceableMessage)
{
    printer_.PrintReplaceableMessage("a");
    ASSERT_EQ("a", sink_.str());
}

TEST_F(PrinterTest, PrintMultipleReplaceableMessages)
{
    printer_.PrintReplaceableMessage("a");
    ASSERT_EQ("a", sink_.str());

    printer_.PrintReplaceableMessage("ab");
    ASSERT_EQ("a\bab", sink_.str());

    printer_.PrintReplaceableMessage("abc");
    ASSERT_EQ("a\bab\b\babc", sink_.str());
}
