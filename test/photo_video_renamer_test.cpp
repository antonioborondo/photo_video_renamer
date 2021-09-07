#include "photo_video_renamer.h"

#include "filesystem_wrappers.h"

#include <gmock/gmock.h>
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

TEST(GetFilenamesFromDirectory, NoFilenamesAreReturnedIfDirectoryIsEmpty)
{
    Directory_wrapper empty_directory;
    const auto filenames{get_filenames_from_directory(empty_directory.path())};

    ASSERT_THAT(filenames, testing::IsEmpty());
}

TEST(GetFilenamesFromDirectory, OnlyPhotosAndVideosAreReturned)
{
    Directory_wrapper parent_directory;
    File_wrapper file_1{parent_directory, "1.jpg"};
    File_wrapper file_2{parent_directory, "2.jpeg"};
    File_wrapper file_3{parent_directory, "3.heic"};
    File_wrapper file_4{parent_directory, "4.mov"};
    File_wrapper file_5{parent_directory, "5.mp4"};
    File_wrapper file_6{parent_directory, "6.txt"};
    File_wrapper file_7{parent_directory, "7.pdf"};
    File_wrapper file_8{parent_directory, "8.doc"};

    const auto filenames{get_filenames_from_directory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::UnorderedElementsAre(file_1.path(), file_2.path(), file_3.path(), file_4.path(), file_5.path()));
}

TEST(GetFilenamesFromDirectory, FilenamesReturnedAreSorted)
{
    Directory_wrapper parent_directory;
    File_wrapper file_1{parent_directory, "1.jpg"};
    File_wrapper file_2{parent_directory, "2.jpeg"};
    File_wrapper file_3{parent_directory, "3.heic"};
    File_wrapper file_4{parent_directory, "4.mov"};
    File_wrapper file_5{parent_directory, "5.mp4"};

    const auto filenames{get_filenames_from_directory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::ElementsAre(file_1.path(), file_2.path(), file_3.path(), file_4.path(), file_5.path()));
}
