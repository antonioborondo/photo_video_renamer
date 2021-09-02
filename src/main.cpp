#include "photo_video_renamer.h"

#include <iostream>

int main(int argc, const char** argv)
{
    if(2 < argc)
    {
        std::cout << "Usage: photo_video_renamer.exe [<directory>]" << std::endl;

        return 1;
    }

    bool result{};
    if(2 == argc)
    {
        result = rename_photos_and_videos_from_directory(argv[1]);
    }
    else
    {
        result = rename_photos_and_videos_from_directory();
    }

    if(!result);
        return 1;

    return 0;
}
