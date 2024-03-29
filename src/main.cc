﻿#include "photo_video_renamer.h"
#include "printer.h"
#include "progress_tracker.h"

#include <fmt/format.h>

#include <filesystem>
#include <thread>

int main(int argc, const char** argv)
{
    Printer printer;
    ProgressTracker progress_tracker{printer};
    PhotoVideoRenamer photo_video_renamer{progress_tracker};

    if(2 < argc)
    {
        printer.PrintMessage("Usage: photo_video_renamer.exe [<directory>]\n");

        return 1;
    }

    auto directory{std::filesystem::current_path()};
    if(2 == argc)
    {
        directory = argv[1];
    }

    if(!photo_video_renamer.DirectoryExists(directory))
    {
        printer.PrintMessage(fmt::format("Error: Directory {0} does not exist\n", directory.string()));

        return 1;
    }

    std::thread progress_tracker_worker{std::ref(progress_tracker)};

    const auto renaming_result{photo_video_renamer.RenamePhotosAndVideosFromDirectory(directory)};

    progress_tracker.Stop();
    progress_tracker_worker.join();

    printer.PrintMessage("\n");

    if(!renaming_result)
    {
        printer.PrintMessage(fmt::format("Error: Cannot rename photos and videos from directory {0}\n", directory.string()));

        return 1;
    }

    return 0;
}
