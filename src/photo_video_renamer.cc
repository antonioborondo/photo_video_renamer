#include "photo_video_renamer.h"

#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <natural_sort/natural_sort.hpp>

#include <algorithm>
#include <regex>

namespace fs = std::filesystem;

PhotoVideoRenamer::PhotoVideoRenamer(const Progress& progress):
    progress_{progress}
{
}

bool PhotoVideoRenamer::DirectoryExists(const fs::path& directory)
{
    return (fs::exists(directory) && fs::is_directory(directory));
}

bool PhotoVideoRenamer::FilenameIsPhotoOrVideo(const fs::path& filename)
{
    const std::regex photo_and_video_extensions{".avi|.bmp|.heic|.jpeg|.jpg|.mov|.mp4|.png", std::regex_constants::icase};

    const auto filename_extension{filename.extension().string()};

    return std::regex_match(filename_extension, photo_and_video_extensions);
}

std::vector<fs::path> PhotoVideoRenamer::GetFilenamesFromDirectory(const fs::path& directory)
{
    std::vector<fs::path> filenames;

    for(fs::directory_iterator directory_iterator{directory}; fs::directory_iterator{} != directory_iterator; directory_iterator++)
    {
        if(fs::is_regular_file(directory_iterator->status()) && FilenameIsPhotoOrVideo(directory_iterator->path()))
        {
            filenames.push_back(directory_iterator->path());
        }
    }

    std::sort(filenames.begin(), filenames.end(), [](const fs::path& filename_1, const fs::path& filename_2)
        {
            return SI::natural::compare<std::wstring>(filename_1.filename().wstring(), filename_2.filename().wstring());
        });

    return filenames;
}

std::vector<fs::path> PhotoVideoRenamer::GenerateNewFilenames(const std::vector<fs::path>& filenames, const std::string& prefix)
{
    const auto number_digits_filename{std::to_string(filenames.size()).length()};

    std::vector<fs::path> new_filenames;

    for(size_t i{}; i < filenames.size(); ++i)
    {
        const auto filename{filenames.at(i)};

        const auto new_filename_number{fmt::format("{:0{}}", i + 1, number_digits_filename)};

        auto new_filename_extension{filename.extension().string()};

        std::transform(new_filename_extension.begin(), new_filename_extension.end(), new_filename_extension.begin(), ::tolower);

        const auto new_filename{filename.parent_path() / fs::path{prefix + new_filename_number + new_filename_extension}};

        new_filenames.push_back(new_filename);
    }

    return new_filenames;
}

bool PhotoVideoRenamer::CheckIfNewFilenamesAlreadyExist(const std::vector<fs::path>& filenames, const std::vector<fs::path>& new_filenames)
{
    return std::any_of(filenames.begin(), filenames.end(), [&](const fs::path& filename)
        {
            return (std::find_if(new_filenames.begin(), new_filenames.end(), [&](const fs::path& new_filename)
                        {
                            return boost::iequals(filename.filename().wstring(), new_filename.filename().wstring());
                        }) != new_filenames.end());
        });
}

bool PhotoVideoRenamer::RenameFilenames(const std::vector<fs::path>& filenames, const std::vector<fs::path>& new_filenames)
{
    for(size_t i{}; i < filenames.size(); ++i)
    {
        try
        {
            fs::rename(filenames.at(i), new_filenames.at(i));

            progress_.IncrementRenamed(1);
        }
        catch(const fs::filesystem_error&)
        {
            return false;
        }
    }

    return true;
}

bool PhotoVideoRenamer::RenamePhotosAndVideosFromDirectory(const fs::path& directory)
{
    auto filenames{GetFilenamesFromDirectory(directory)};

    progress_.IncrementTotal(filenames.size());

    auto new_filenames{GenerateNewFilenames(filenames)};

    const auto new_filenames_already_exist{CheckIfNewFilenamesAlreadyExist(filenames, new_filenames)};
    if(new_filenames_already_exist)
    {
        progress_.IncrementTotal(filenames.size());

        int i{};
        std::vector<fs::path> temp_filenames;
        bool temp_filenames_already_exist{};
        do
        {
            temp_filenames = GenerateNewFilenames(filenames, "temp" + std::to_string(i++));

            temp_filenames_already_exist = CheckIfNewFilenamesAlreadyExist(filenames, temp_filenames);
        }
        while(temp_filenames_already_exist);

        RenameFilenames(filenames, temp_filenames);

        filenames = GetFilenamesFromDirectory(directory);
    }

    return RenameFilenames(filenames, new_filenames);
}
