#include "photo_video_renamer.h"
#include "printer.h"
#include "progress.h"

#include <fmt/format.h>

#include <filesystem>
#include <thread>

int main(int argc, const char** argv)
{
    Printer printer;
    Progress progress{printer};
    PhotoVideoRenamer photo_video_renamer{progress};

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

    directory = "U:\\test";

    if(!photo_video_renamer.DirectoryExists(directory))
    {
        printer.PrintMessage(fmt::format("Error: Directory {0} does not exist\n", directory.string()));

        return 1;
    }

    std::thread progress_worker{std::ref(progress)};

    if(!photo_video_renamer.RenamePhotosAndVideosFromDirectory(directory))
    {
        printer.PrintMessage(fmt::format("Error: Cannot rename photos and videos from directory {0}\n", directory.string()));

        return 1;
    }

    progress.Stop();
    progress_worker.join();

    return 0;
}
