#pragma once

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>

class FilesystemWrapper
{
protected:
    std::filesystem::path path_;

    FilesystemWrapper() = default;

public:
    ~FilesystemWrapper()
    {
        std::filesystem::remove_all(path_);
    }

    const std::filesystem::path path() const
    {
        return path_;
    }
};

class DirectoryWrapper: public FilesystemWrapper
{
public:
    DirectoryWrapper()
    {
        path_ = std::tmpnam(nullptr);
        std::filesystem::create_directory(path_);
    }
};

class FileWrapper: public FilesystemWrapper
{
    bool FilenameIsPhoto(const std::filesystem::path& filename)
    {
        const std::regex photo_extensions{".jpg", std::regex_constants::icase};

        const auto filename_extension{filename.extension().string()};

        return std::regex_match(filename_extension, photo_extensions);
    }

public:
    FileWrapper(const DirectoryWrapper& parent_directory, const std::filesystem::path& filename)
    {
        path_ = parent_directory.path();
        path_ /= filename;
        std::ofstream file{path_};

        if(FilenameIsPhoto(filename))
        {
            std::vector<std::byte> photo_header = {
                std::byte{0xFF}, // Start of marker
                std::byte{0xD8}, // SOI (Start Of Image) marker
                std::byte{0xFF}, // Start of marker
                std::byte{0xD9}, // EOI (End Of Image) marker
            };

            file.write(reinterpret_cast<const char*>(photo_header.data()), photo_header.size());
        }
    }
};
