// renamer.cpp : Defines the entry point for the application.
//

#include "renamer.h"

#include <natural_sort/natural_sort.hpp>

#include <algorithm>
#include <filesystem>
#include <regex>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> get_filenames_from_directory(const fs::path& directory)
{
    std::vector<fs::path> filenames;

    if (fs::exists(directory) && fs::is_directory(directory))
    {
        for (fs::directory_iterator directory_iterator{ directory }; fs::directory_iterator{} != directory_iterator; directory_iterator++)
        {
            if (fs::is_regular_file(directory_iterator->status()))
            {
                filenames.push_back(directory_iterator->path());
            }
        }
    }

    std::sort(filenames.begin(), filenames.end(), [](const fs::path& filename_1, const fs::path& filename_2)
        {
            return SI::natural::compare<std::wstring>(filename_1.filename().wstring(), filename_2.filename().wstring());
        });

    return filenames;
}

void remove_non_pics_or_movie_filenames(std::vector<fs::path>& filenames)
{

    filenames.erase(std::remove_if(filenames.begin(), filenames.end(), [](const fs::path& filename)
        {
            const std::regex pics_and_movie_file_extensions{ ".jpg|.jpeg|.heic|.mov" };

            auto filename_extension{ filename.extension().string() };

            std::transform(filename_extension.begin(), filename_extension.end(), filename_extension.begin(), ::tolower);

            return !std::regex_match(filename_extension, pics_and_movie_file_extensions);

        }), filenames.end());
}

int main()
{
    const auto directory{std::filesystem::current_path()};

    std::cout << "Directory: " << directory << std::endl;

    auto filenames{ get_filenames_from_directory(directory)};

    std::cout << "Filenames:" << std::endl;

    for (const auto& filename : filenames)
    {
        std::cout << filename << std::endl;
    }

    remove_non_pics_or_movie_filenames(filenames);

    std::cout << "Pics and movies:" << std::endl;

    for (const auto& filename : filenames)
    {
        std::cout << filename << std::endl;
    }
	
	return 0;
}
