#include "photo_video_renamer.h"

#include <gtest/gtest.h>

#include <filesystem>

class Directory_wrapper : public ::testing::Test
{
protected:
    std::filesystem::path m_directory;

    Directory_wrapper()
    {
        m_directory = std::tmpnam(nullptr);

        std::filesystem::create_directory(m_directory);
    }

    ~Directory_wrapper()
    {
        std::filesystem::remove_all(m_directory);
    }
};

class Directory_exists_test : public Directory_wrapper
{
};

TEST_F(Directory_exists_test, NonExistingDirectoryDoesNotExists)
{
    const std::filesystem::path non_existing_directory{std::tmpnam(nullptr)};
    ASSERT_FALSE(directory_exists(non_existing_directory));
}

TEST_F(Directory_exists_test, ExistingDirectoryExists)
{
    ASSERT_TRUE(directory_exists(m_directory));
}
