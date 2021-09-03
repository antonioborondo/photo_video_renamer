#include "photo_video_renamer.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, const char** argv)
{
    if(2 < argc)
    {
        std::cout << "Usage: photo_video_renamer.exe [<directory>]" << std::endl;

        return 1;
    }

    auto directory{fs::current_path()};
    if(2 == argc)
    {
        directory = argv[1];
    }

    if(!directory_exists(directory))
    {
        std::cout << "Error: Directory " << directory << " does not exist" << std::endl;
    }

    return rename_photos_and_videos_from_directory(directory) ? 0 : 1;
}
