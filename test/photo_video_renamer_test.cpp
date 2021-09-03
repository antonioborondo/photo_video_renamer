#include "photo_video_renamer.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

class Directory_wrapper
{
    std::filesystem::path m_path;

public:
    Directory_wrapper()
    {
        m_path = std::tmpnam(nullptr);
        std::filesystem::create_directory(m_path);
    }

    ~Directory_wrapper()
    {
        std::filesystem::remove_all(m_path);
    }

    const std::filesystem::path path() const
    {
        return m_path;
    }
};

class File_wrapper
{
    std::filesystem::path m_path;

public:
    File_wrapper()
    {
        m_path = std::tmpnam(nullptr);
        std::ofstream file{m_path};
    }

    ~File_wrapper()
    {
        std::filesystem::remove(m_path);
    }

    const std::filesystem::path path() const
    {
        return m_path;
    }
};

TEST(Directory_exists_test, NonExistingDirectoryDoesNotExists)
{
    const std::filesystem::path non_existing_directory{std::tmpnam(nullptr)};
    ASSERT_FALSE(directory_exists(non_existing_directory));
}

TEST(Directory_exists_test, ExistingFileIsNotDirectory)
{
    File_wrapper file;
    ASSERT_FALSE(directory_exists(file.path()));
}

TEST(Directory_exists_test, ExistingDirectoryExists)
{
    Directory_wrapper directory;
    ASSERT_TRUE(directory_exists(directory.path()));
}
