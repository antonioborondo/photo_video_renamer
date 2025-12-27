#include "photo_video_renamer.h"

#include <boost/algorithm/string.hpp>
#include <exiv2/exiv2.hpp>
#include <fmt/format.h>
#include <MediaInfo/MediaInfo.h>
#include <natural_sort/natural_sort.hpp>

#include <algorithm>
#include <codecvt>
#include <locale>
#include <regex>

namespace fs = std::filesystem;

PhotoVideoRenamer::PhotoVideoRenamer(ProgressTracker& progress_tracker):
    progress_tracker_{progress_tracker}
{
}

bool PhotoVideoRenamer::DirectoryExists(const fs::path& directory)
{
    return (fs::exists(directory) && fs::is_directory(directory));
}

PhotoVideoRenamer::FileType PhotoVideoRenamer::GetFileType(const std::filesystem::path& filename)
{
    const std::regex photo_extensions{".bmp|.heic|.jpeg|.jpg|.png", std::regex_constants::icase};
    const std::regex video_extensions{".avi|.mov|.mp4", std::regex_constants::icase};

    const auto filename_extension{filename.extension().string()};

    if(std::regex_match(filename_extension, photo_extensions))
    {
        return FileType::Photo;
    }
    else if(std::regex_match(filename_extension, video_extensions))
    {
        return FileType::Video;
    }
    else
    {
        return FileType::Other;
    }
}

bool PhotoVideoRenamer::FilenameIsPhotoOrVideo(const fs::path& filename)
{
    const auto file_type{GetFileType(filename)};

    return (file_type == FileType::Photo || file_type == FileType::Video);
}

std::vector<fs::path> PhotoVideoRenamer::GetFilenamesFromDirectory(const fs::path& directory, PhotoVideoRenamer::Sort sort)
{
    std::vector<fs::path> filenames;

    for(fs::directory_iterator directory_iterator{directory}; fs::directory_iterator{} != directory_iterator; directory_iterator++)
    {
        if(fs::is_regular_file(directory_iterator->status()) && FilenameIsPhotoOrVideo(directory_iterator->path()))
        {
            filenames.push_back(directory_iterator->path());
        }
    }

    std::sort(filenames.begin(), filenames.end(), [&](const fs::path& filename_1, const fs::path& filename_2)
        {
            if(sort == PhotoVideoRenamer::Sort::Natural)
            {
                return SI::natural::compare<std::wstring>(filename_1.filename().wstring(), filename_2.filename().wstring());
            }
            else if(sort == PhotoVideoRenamer::Sort::DateTaken)
            {
                return GetDateTaken(filename_1.string()) < GetDateTaken(filename_2.wstring());
            }
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

std::vector<fs::path> PhotoVideoRenamer::GenerateNewFilenamesByDateTaken(const std::vector<fs::path>& filenames, const std::string& prefix)
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

            progress_tracker_.IncrementRenamed(1);
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
    auto filenames{GetFilenamesFromDirectory(directory, Sort::DateTaken)};

    progress_tracker_.IncrementTotal(filenames.size());

    auto new_filenames{GenerateNewFilenames(filenames)};

    const auto new_filenames_already_exist{CheckIfNewFilenamesAlreadyExist(filenames, new_filenames)};
    if(new_filenames_already_exist)
    {
        progress_tracker_.IncrementTotal(filenames.size());

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

        filenames = GetFilenamesFromDirectory(directory, Sort::DateTaken);
    }

    return RenameFilenames(filenames, new_filenames);
}

std::string PhotoVideoRenamer::GetDateTaken(const fs::path& filename)
{
    if(GetFileType(filename) == FileType::Photo)
    {
        try
        {
            auto image = Exiv2::ImageFactory::open(filename.string());
            image->readMetadata();
            Exiv2::ExifData& exifData = image->exifData();
            auto it = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
            if(it != exifData.end())
            {
                return it->toString(); // Usually in "YYYY:MM:DD HH:MM:SS"
            }
        }
        catch(Exiv2::Error& e)
        {
            std::cerr << "Error reading " << filename << ": " << e.what() << "\n";
        }
    }
    else if(GetFileType(filename) == FileType::Video)
    {
        MediaInfoLib::MediaInfo MI;
        MediaInfoLib::String file;
        MI.Open(filename.wstring());

        //const auto file_size = MI.Get(MediaInfoLib::Stream_General, 0, __T("FileSize"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name).c_str();

        // Example: retrieve general creation date
        //const auto recorded_date{MI.Get(MediaInfoLib::Stream_General, 0, __T("Recorded_Date"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name)};
        const auto encoded_date = MI.Get(MediaInfoLib::Stream_General, 0, __T("Encoded_Date"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name);
        //auto tagged_date = MI.Get(MediaInfoLib::Stream_General, 0, __T("Tagged_Date"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name);
        MI.Close();

        std::wstring wstr{encoded_date};
        // create a converter for UTF-8 ⇔ UTF-16/32 (depending on sizeof(wchar_t))
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

        // convert to UTF-8 std::string
        std::string utf8 = conv.to_bytes(wstr);

        return utf8;

        // …and back again, if you need:
        //std::wstring back = conv.from_bytes(utf8);
    }
    return ""; // fallback if date not found
}
