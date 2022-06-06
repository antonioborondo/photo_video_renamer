#include "photo_video_renamer.h"

#include <filesystem>
#include <iostream>

int main(int argc, const char** argv)
{
    if(2 < argc)
    {
        std::cout << "Usage: photo_video_renamer.exe [<directory>]" << std::endl;

        return 1;
    }

    auto directory{std::filesystem::current_path()};
    if(2 == argc)
    {
        directory = argv[1];
    }

    if(!directory_exists(directory))
    {
        std::cout << "Error: Directory " << directory << " does not exist" << std::endl;

        return 1;
    }

    if(!rename_photos_and_videos_from_directory(directory))
    {
        std::cout << "Error: Cannot rename photos and videos from directory " << directory << std::endl;

        return 1;
    }

    return 0;
}
