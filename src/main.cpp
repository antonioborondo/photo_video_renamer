#include "photo_video_renamer.h"
#include "printer.h"
#include "progress.h"

#include <fmt/format.h>

#include <filesystem>

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

    if(!photo_video_renamer.DirectoryExists(directory))
    {
        printer.PrintMessage(fmt::format("Error: Directory {0} does not exist\n", directory.string()));

        return 1;
    }

    if(!photo_video_renamer.RenamePhotosAndVideosFromDirectory(directory))
    {
        printer.PrintMessage(fmt::format("Error: Cannot rename photos and videos from directory {0}\n", directory.string()));

        return 1;
    }

    return 0;
}
