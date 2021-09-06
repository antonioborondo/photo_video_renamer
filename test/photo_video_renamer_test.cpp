#include "photo_video_renamer.h"

#include "filesystem_wrappers.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>

TEST(DirectoryExists, NonExistingDirectoryDoesNotExists)
{
    const std::filesystem::path non_existing_directory{std::tmpnam(nullptr)};
    ASSERT_FALSE(directory_exists(non_existing_directory));
}

TEST(DirectoryExists, ExistingFileIsNotDirectory)
{
    Directory_wrapper parent_directory;
    File_wrapper file{parent_directory, "test.txt"};
    ASSERT_FALSE(directory_exists(file.path()));
}

TEST(DirectoryExists, ExistingDirectoryExists)
{
    Directory_wrapper directory;
    ASSERT_TRUE(directory_exists(directory.path()));
}
