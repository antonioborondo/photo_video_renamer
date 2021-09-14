#include "photo_video_renamer.h"

#include "filesystem_wrappers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

TEST(DirectoryExists, NonExistingDirectoryDoesNotExists)
{
    const std::filesystem::path non_existing_directory{std::tmpnam(nullptr)};

    ASSERT_FALSE(directory_exists(non_existing_directory));
}

TEST(DirectoryExists, ExistingFileIsNotDirectory)
{
    Directory_wrapper parent_directory;
    File_wrapper file{parent_directory, "file.txt"};

    ASSERT_FALSE(directory_exists(file.path()));
}

TEST(DirectoryExists, ExistingDirectoryExists)
{
    Directory_wrapper directory;

    ASSERT_TRUE(directory_exists(directory.path()));
}

TEST(FilenameIsPhotoOrVideo, PhotoOrVideoFilenamesInLowerAndUpperCase)
{
    ASSERT_TRUE(filename_is_photo_or_video("file.jpg"));
    ASSERT_TRUE(filename_is_photo_or_video("file.JPG"));
    ASSERT_TRUE(filename_is_photo_or_video("file.jpeg"));
    ASSERT_TRUE(filename_is_photo_or_video("file.JPEG"));
    ASSERT_TRUE(filename_is_photo_or_video("file.heic"));
    ASSERT_TRUE(filename_is_photo_or_video("file.HEIC"));
    ASSERT_TRUE(filename_is_photo_or_video("file.mov"));
    ASSERT_TRUE(filename_is_photo_or_video("file.MOV"));
    ASSERT_TRUE(filename_is_photo_or_video("file.mp4"));
    ASSERT_TRUE(filename_is_photo_or_video("file.MP4"));
}

TEST(FilenameIsPhotoOrVideo, NonPhotoOrVideoFilenamesInLowerAndUpperCase)
{
    ASSERT_FALSE(filename_is_photo_or_video("file.txt"));
    ASSERT_FALSE(filename_is_photo_or_video("file.TXT"));
    ASSERT_FALSE(filename_is_photo_or_video("file.pdf"));
    ASSERT_FALSE(filename_is_photo_or_video("file.PDF"));
    ASSERT_FALSE(filename_is_photo_or_video("file.doc"));
    ASSERT_FALSE(filename_is_photo_or_video("file.DOC"));
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
    File_wrapper file_1{parent_directory, "file_1.jpg"};
    File_wrapper file_2{parent_directory, "file_2.jpeg"};
    File_wrapper file_3{parent_directory, "file_3.heic"};
    File_wrapper file_4{parent_directory, "file_4.mov"};
    File_wrapper file_5{parent_directory, "file_5.mp4"};
    File_wrapper file_6{parent_directory, "file_6.txt"};
    File_wrapper file_7{parent_directory, "file_7.pdf"};
    File_wrapper file_8{parent_directory, "file_8.doc"};

    const auto filenames{get_filenames_from_directory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::UnorderedElementsAre(file_1.path(), file_2.path(), file_3.path(), file_4.path(), file_5.path()));
}

TEST(GetFilenamesFromDirectory, FilenamesReturnedAreSorted)
{
    Directory_wrapper parent_directory;
    File_wrapper file_1{parent_directory, "file_1.jpg"};
    File_wrapper file_2{parent_directory, "file_2.jpeg"};
    File_wrapper file_3{parent_directory, "file_3.heic"};
    File_wrapper file_4{parent_directory, "file_4.mov"};
    File_wrapper file_5{parent_directory, "file_5.mp4"};

    const auto filenames{get_filenames_from_directory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::ElementsAre(file_1.path(), file_2.path(), file_3.path(), file_4.path(), file_5.path()));
}

TEST(GenerateNewFilenames, NumberOfInputFilenamesEqualToNumberOfGeneratedFilenames)
{
    for(int i{}; i < 10; ++i)
    {
        ASSERT_THAT(generate_new_filenames(std::vector<fs::path>(i)), testing::SizeIs(i));
    }
}

TEST(GenerateNewFilenames, NumberOfLeadingZerosIsCorrect)
{
    ASSERT_THAT(generate_new_filenames(std::vector<fs::path>(9)), testing::IsSupersetOf({fs::path{"1"}, fs::path{"9"}}));
    ASSERT_THAT(generate_new_filenames(std::vector<fs::path>(99)), testing::IsSupersetOf({fs::path{"01"}, fs::path{"99"}}));
    ASSERT_THAT(generate_new_filenames(std::vector<fs::path>(999)), testing::IsSupersetOf({fs::path{"001"}, fs::path{"010"}, fs::path{"999"}}));
    ASSERT_THAT(generate_new_filenames(std::vector<fs::path>(9999)), testing::IsSupersetOf({fs::path{"0001"}, fs::path{"0010"}, fs::path{"0100"}, fs::path{"9999"}}));
}
