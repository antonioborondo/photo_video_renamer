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

TEST(FilenameIsPhotoOrVideo, PhotoOrVideoFilenamesInLowerAndUpperCase)
{
    ASSERT_TRUE(filename_is_photo_or_video("test.jpg"));
    ASSERT_TRUE(filename_is_photo_or_video("TEST.JPG"));
    ASSERT_TRUE(filename_is_photo_or_video("test.jpeg"));
    ASSERT_TRUE(filename_is_photo_or_video("TEST.JPEG"));
    ASSERT_TRUE(filename_is_photo_or_video("test.heic"));
    ASSERT_TRUE(filename_is_photo_or_video("TEST.HEIC"));
    ASSERT_TRUE(filename_is_photo_or_video("test.mov"));
    ASSERT_TRUE(filename_is_photo_or_video("TEST.MOV"));
    ASSERT_TRUE(filename_is_photo_or_video("test.mp4"));
    ASSERT_TRUE(filename_is_photo_or_video("TEST.MP4"));
}

TEST(FilenameIsPhotoOrVideo, NonPhotoOrVideoFilenamesInLowerAndUpperCase)
{
    ASSERT_FALSE(filename_is_photo_or_video("test.txt"));
    ASSERT_FALSE(filename_is_photo_or_video("TEST.TXT"));
    ASSERT_FALSE(filename_is_photo_or_video("test.pdf"));
    ASSERT_FALSE(filename_is_photo_or_video("TEST.PDF"));
    ASSERT_FALSE(filename_is_photo_or_video("test.doc"));
    ASSERT_FALSE(filename_is_photo_or_video("TEST.DOC"));
}
