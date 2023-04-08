#include "printer.h"
#include "progress_tracker.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <sstream>
#include <thread>

class ProgressTrackerTest: public testing::Test
{
    Printer printer_;

protected:
    std::ostringstream sink_;

    ProgressTracker progress_tracker_;

    ProgressTrackerTest():
        printer_{sink_},
        progress_tracker_{printer_}
    {
    }

    void SetUp() override
    {
        sink_.str("");
        sink_.clear();
    }
};

TEST_F(ProgressTrackerTest, PrintProgressNotInitialised)
{
    progress_tracker_.PrintProgress();

    ASSERT_EQ("Renaming 0%", sink_.str());
}

TEST_F(ProgressTrackerTest, PrintProgress)
{
    progress_tracker_.IncrementTotal(100);

    progress_tracker_.IncrementRenamed(50);

    progress_tracker_.PrintProgress();

    ASSERT_EQ("Renaming 50%", sink_.str());
}

TEST_F(ProgressTrackerTest, PrintMultipleProgress)
{
    progress_tracker_.IncrementTotal(100);

    progress_tracker_.IncrementRenamed(50);

    progress_tracker_.PrintProgress();

    ASSERT_EQ("Renaming 50%", sink_.str());

    progress_tracker_.IncrementRenamed(50);

    progress_tracker_.PrintProgress();
    ASSERT_EQ("Renaming 50%\b\b\b\b\b\b\b\b\b\b\b\bRenaming 100%", sink_.str());
}

TEST_F(ProgressTrackerTest, PrintProgressEveryHalfSecond)
{
    std::thread progress_tracker_worker{std::ref(progress_tracker_)};

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    progress_tracker_.Stop();
    progress_tracker_worker.join();

    ASSERT_THAT(sink_.str(), testing::HasSubstr("Renaming 0%\b\b\b\b\b\b\b\b\b\b\bRenaming 0%\b\b\b\b\b\b\b\b\b\b\bRenaming 0%"));
}
