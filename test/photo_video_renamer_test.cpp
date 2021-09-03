#include "photo_video_renamer.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

class Filesystem_wrapper
{
protected:
    std::filesystem::path m_path;

    Filesystem_wrapper()
    {
    }

public:
    ~Filesystem_wrapper()
    {
        std::filesystem::remove_all(m_path);
    }

    const std::filesystem::path path() const
    {
        return m_path;
    }
};

class Directory_wrapper : public Filesystem_wrapper
{
public:
    Directory_wrapper()
    : Filesystem_wrapper{}
    {
        m_path = std::tmpnam(nullptr);
        std::filesystem::create_directory(m_path);
    }
};

class File_wrapper : public Filesystem_wrapper
{
public:
    File_wrapper(const Directory_wrapper& directory, const std::filesystem::path& filename)
    : Filesystem_wrapper{}
    {
        m_path = directory.path();
        m_path /= filename;
        std::ofstream file{m_path};
    }
};

TEST(DirectoryExists, NonExistingDirectoryDoesNotExists)
{
    const std::filesystem::path non_existing_directory{std::tmpnam(nullptr)};
    ASSERT_FALSE(directory_exists(non_existing_directory));
}

TEST(DirectoryExists, ExistingFileIsNotDirectory)
{
    File_wrapper file{Directory_wrapper{}, "test.txt"};
    ASSERT_FALSE(directory_exists(file.path()));
}

TEST(DirectoryExists, ExistingDirectoryExists)
{
    Directory_wrapper directory;
    ASSERT_TRUE(directory_exists(directory.path()));
}
