#include <fmt/format.h>
#include <natural_sort/natural_sort.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> get_filenames_from_directory(const fs::path& directory)
{
    std::vector<fs::path> filenames;

    if(fs::exists(directory) && fs::is_directory(directory))
    {
        for(fs::directory_iterator directory_iterator{directory}; fs::directory_iterator{} != directory_iterator; directory_iterator++)
        {
            if(fs::is_regular_file(directory_iterator->status()))
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

void remove_non_photo_or_video_filenames(std::vector<fs::path>& filenames)
{
    filenames.erase(std::remove_if(filenames.begin(), filenames.end(), [](const fs::path& filename)
    {
        const std::regex pics_and_movie_file_extensions{ ".heic|.jpeg|.jpg|.mov|.mp4|.png" };

        auto filename_extension{ filename.extension().string() };

        std::transform(filename_extension.begin(), filename_extension.end(), filename_extension.begin(), ::tolower);

        return !std::regex_match(filename_extension, pics_and_movie_file_extensions);

    }), filenames.end());
}

std::vector<fs::path> get_new_filenames(const std::vector<fs::path>& filenames, const std::string& prefix = "")
{
    std::vector<fs::path> new_filenames;

    const auto number_digits_filename{std::to_string(std::to_string(filenames.size()).length())};

    for (auto i = 0; i < filenames.size(); i++)
    {
        const auto filename = filenames.at(i);

        const auto new_filename_number{fmt::format("{:0" + number_digits_filename + "d}", i + 1)};

        auto new_filename_extension{ filename.extension().string() };

        std::transform(new_filename_extension.begin(), new_filename_extension.end(), new_filename_extension.begin(), ::tolower);

        auto new_filename = filename.parent_path() / fs::path{ prefix + new_filename_number + new_filename_extension };

        new_filenames.push_back(new_filename);
    }

    return new_filenames;
}

bool check_if_new_filenames_already_exist(const std::vector<fs::path>& filenames, const std::vector<fs::path>& new_filenames)
{
    for (const fs::path& filename : filenames)
    {       
        for (const fs::path& new_filename : new_filenames)
        {
            if (filename.stem() == new_filename.stem())
                return true;
        }
    }

    return false;
}

bool rename_filenames(const std::vector<fs::path>& filenames, const std::vector<fs::path>& new_filenames)
{
    for (int i{}; i < filenames.size(); i++)
    {
        try
        {
            fs::rename(filenames.at(i), new_filenames.at(i));
        }
        catch (const fs::filesystem_error&)
        {
            return false;
        }
    }

    return true;
}

bool directory_exists(const fs::path directory)
{
    return (fs::exists(directory) && fs::is_directory(directory));
}

int main(int argc, const char** argv)
{
    if(2 < argc)
    {
        std::cout << "Usage: photo_video_renamer.exe [<directory>]" << std::endl;

        return 1;
    }

    fs::path directory;
    if(2 == argc)
    {
        directory = argv[1];
    }
    else
    {
        directory = fs::current_path();
    }

    if (!directory_exists(directory))
    {
        std::cout << "Error: Directory '" << argv[1] << "' does not exist" << std::endl;

        return 1;
    }

    auto filenames{get_filenames_from_directory(directory)};

    remove_non_photo_or_video_filenames(filenames);

    auto new_filenames{get_new_filenames(filenames)};

    const auto new_filenames_already_exist{check_if_new_filenames_already_exist(filenames, new_filenames)};
    if(new_filenames_already_exist)
    {
        int i{};
        std::vector<fs::path> temp_filenames;
        bool temp_filenames_already_exist{};
        do
        {
            temp_filenames = get_new_filenames(filenames, "temp" + std::to_string(i++));

            temp_filenames_already_exist = check_if_new_filenames_already_exist(filenames, temp_filenames);

        } while (temp_filenames_already_exist);
        
        rename_filenames(filenames, temp_filenames);

        filenames = get_filenames_from_directory(directory);
    }

    if (!rename_filenames(filenames, new_filenames))
        return 1;
	
	return 0;
}
