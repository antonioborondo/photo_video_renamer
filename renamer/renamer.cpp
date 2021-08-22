// renamer.cpp : Defines the entry point for the application.
//

#include "renamer.h"

#include <natural_sort/natural_sort.hpp>

#include <filesystem>
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


int main()
{
    const auto directory{std::filesystem::current_path()};

    std::cout << "Directory: " << directory << std::endl;

    const auto filenames{ get_filenames_from_directory(directory)};

    std::cout << "Filenames:" << std::endl;

    for (const auto& filename : filenames)
    {
        std::cout << filename << std::endl;
    }
	
	return 0;
}
