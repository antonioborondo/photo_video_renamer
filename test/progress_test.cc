#include "printer.h"
#include "progress.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <sstream>
#include <thread>

class ProgressTest: public testing::Test
{
    Printer printer_;

protected:
    std::ostringstream sink_;

    Progress progress_;

    ProgressTest():
        printer_{sink_},
        progress_{printer_}
    {
    }

    void SetUp() override
    {
        sink_.str("");
        sink_.clear();
    }
};

TEST_F(ProgressTest, PrintProgressNotInitialised)
{
    progress_.PrintProgress();

    ASSERT_EQ("Renaming 0%", sink_.str());
}

TEST_F(ProgressTest, PrintProgress)
{
    progress_.IncrementTotal(100);

    progress_.IncrementRenamed(50);

    progress_.PrintProgress();

    ASSERT_EQ("Renaming 50%", sink_.str());
}

TEST_F(ProgressTest, PrintMultipleProgress)
{
    progress_.IncrementTotal(100);

    progress_.IncrementRenamed(50);

    progress_.PrintProgress();

    ASSERT_EQ("Renaming 50%", sink_.str());

    progress_.IncrementRenamed(50);

    progress_.PrintProgress();
    ASSERT_EQ("Renaming 50%\b\b\b\b\b\b\b\b\b\b\b\bRenaming 100%", sink_.str());
}

TEST_F(ProgressTest, PrintProgressEveryHalfSecond)
{
    std::thread progress_worker{std::ref(progress_)};

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    progress_.Stop();
    progress_worker.join();

    ASSERT_THAT(sink_.str(), testing::HasSubstr("Renaming 0%\b\b\b\b\b\b\b\b\b\b\bRenaming 0%\b\b\b\b\b\b\b\b\b\b\bRenaming 0%"));
}
