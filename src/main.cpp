#include <natural_sort/natural_sort.hpp>

#include <algorithm>
#include <filesystem>
#include <format>
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

bool rename_filenames(const std::vector<fs::path>& filenames)
{
    const auto number_digits_filename = std::to_string(std::to_string(filenames.size()).length());

    for (auto i = 0; i < filenames.size(); i++)
    {
        const auto filename = filenames.at(i);

        const auto new_filename_number = std::format("{:0" + number_digits_filename + "d}", i + 1);

        auto new_filename_extension{ filename.extension().string() };

        std::transform(new_filename_extension.begin(), new_filename_extension.end(), new_filename_extension.begin(), ::tolower);

        auto new_filename = filename.parent_path() / fs::path{ new_filename_number + new_filename_extension };

        try
        {
            fs::rename(filename, new_filename);
        }
        catch (const fs::filesystem_error&)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    const auto directory{std::filesystem::current_path()};

    auto filenames{ get_filenames_from_directory(directory)};

    remove_non_pics_or_movie_filenames(filenames);

    if (!rename_filenames(filenames))
        return 1;
	
	return 0;
}
