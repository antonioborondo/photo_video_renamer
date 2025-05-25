#pragma once

#include <cstdio>
#include <filesystem>
#include <fstream>

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
public:
    FileWrapper(const DirectoryWrapper& parent_directory, const std::filesystem::path& filename)
    {
        path_ = parent_directory.path();
        path_ /= filename;
        std::ofstream file{path_};

        unsigned char minimalJpeg[] = {
            0xFF,
            0xD8, // SOI marker
            0xFF,
            0xD9 // EOI marker
        };
        file.write(reinterpret_cast<char*>(minimalJpeg), sizeof(minimalJpeg));
    }
};
