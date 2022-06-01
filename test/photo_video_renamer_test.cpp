#include "photo_video_renamer.h"

#include "filesystem_wrappers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

TEST(DirectoryExists, NonExistingDirectoryDoesNotExists)
{
    const fs::path non_existing_directory{std::tmpnam(nullptr)};

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
    ASSERT_TRUE(filename_is_photo_or_video("file.avi"));
    ASSERT_TRUE(filename_is_photo_or_video("file.AVI"));
    ASSERT_TRUE(filename_is_photo_or_video("file.bmp"));
    ASSERT_TRUE(filename_is_photo_or_video("file.BMP"));
    ASSERT_TRUE(filename_is_photo_or_video("file.heic"));
    ASSERT_TRUE(filename_is_photo_or_video("file.HEIC"));
    ASSERT_TRUE(filename_is_photo_or_video("file.jpeg"));
    ASSERT_TRUE(filename_is_photo_or_video("file.JPEG"));
    ASSERT_TRUE(filename_is_photo_or_video("file.jpg"));
    ASSERT_TRUE(filename_is_photo_or_video("file.JPG"));
    ASSERT_TRUE(filename_is_photo_or_video("file.mov"));
    ASSERT_TRUE(filename_is_photo_or_video("file.MOV"));
    ASSERT_TRUE(filename_is_photo_or_video("file.mp4"));
    ASSERT_TRUE(filename_is_photo_or_video("file.MP4"));
    ASSERT_TRUE(filename_is_photo_or_video("file.png"));
    ASSERT_TRUE(filename_is_photo_or_video("file.PNG"));
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
    File_wrapper file_01{parent_directory, "file_01.avi"};
    File_wrapper file_02{parent_directory, "file_02.bmp"};
    File_wrapper file_03{parent_directory, "file_03.heic"};
    File_wrapper file_04{parent_directory, "file_04.jpeg"};
    File_wrapper file_05{parent_directory, "file_05.jpg"};
    File_wrapper file_06{parent_directory, "file_06.mov"};
    File_wrapper file_07{parent_directory, "file_07.mp4"};
    File_wrapper file_08{parent_directory, "file_08.png"};
    File_wrapper file_09{parent_directory, "file_09.txt"};
    File_wrapper file_10{parent_directory, "file_10.pdf"};
    File_wrapper file_11{parent_directory, "file_11.doc"};

    const auto filenames{get_filenames_from_directory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::UnorderedElementsAre(file_01.path(), file_02.path(), file_03.path(), file_04.path(), file_05.path(), file_06.path(), file_07.path(), file_08.path()));
}

TEST(GetFilenamesFromDirectory, FilenamesReturnedAreSorted)
{
    Directory_wrapper parent_directory;
    File_wrapper file_1{parent_directory, "file_1.avi"};
    File_wrapper file_2{parent_directory, "file_2.bmp"};
    File_wrapper file_3{parent_directory, "file_3.heic"};
    File_wrapper file_4{parent_directory, "file_4.jpeg"};
    File_wrapper file_5{parent_directory, "file_5.jpg"};
    File_wrapper file_6{parent_directory, "file_6.mov"};
    File_wrapper file_7{parent_directory, "file_7.mp4"};
    File_wrapper file_8{parent_directory, "file_8.png"};

    const auto filenames{get_filenames_from_directory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::ElementsAre(file_1.path(), file_2.path(), file_3.path(), file_4.path(), file_5.path(), file_6.path(), file_7.path(), file_8.path()));
}

TEST(GenerateNewFilenames, NumberOfInputFilenamesIsEqualToNumberOfGeneratedFilenames)
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

TEST(GenerateNewFilenames, ExtensionIsLowerCase)
{
    ASSERT_THAT(generate_new_filenames({fs::path{"file.TXT"}}), testing::ElementsAre(fs::path{"1.txt"}));
}

TEST(GenerateNewFilenames, ParentDirectoryIsKept)
{
    const std::vector<fs::path> filenames{fs::path{"C:/1/2/3/4/5/file.txt"}};
    const auto new_filenames{generate_new_filenames(filenames)};

    ASSERT_EQ(filenames.at(0).parent_path(), new_filenames.at(0).parent_path());
}

TEST(GenerateNewFilenames, PrefixIsAddedToTheFilename)
{
    constexpr auto prefix{"test_"};

    ASSERT_THAT(generate_new_filenames(std::vector<fs::path>(1), prefix), testing::ElementsAre(fs::path{"test_1"}));
}

TEST(CheckIfNewFilenamesAlreadyExist, OneFilenameAlreadyExists)
{
    const std::vector<fs::path> filenames{fs::path{"1"}, fs::path{"2"}, fs::path{"3"}};
    const std::vector<fs::path> new_filenames{fs::path{"a"}, fs::path{"2"}, fs::path{"c"}};

    ASSERT_TRUE(check_if_new_filenames_already_exist(filenames, new_filenames));
}

TEST(CheckIfNewFilenamesAlreadyExist, OneFilenameWithDifferentCaseAlreadyExists)
{
    const std::vector<fs::path> filenames{fs::path{"a"}, fs::path{"b"}, fs::path{"c"}};
    const std::vector<fs::path> new_filenames{fs::path{"1"}, fs::path{"B"}, fs::path{"3"}};

    ASSERT_TRUE(check_if_new_filenames_already_exist(filenames, new_filenames));
}

TEST(CheckIfNewFilenamesAlreadyExist, NoFilenameExists)
{
    const std::vector<fs::path> filenames{fs::path{"1"}, fs::path{"2"}, fs::path{"3"}};
    const std::vector<fs::path> new_filenames{fs::path{"a"}, fs::path{"b"}, fs::path{"c"}};

    ASSERT_FALSE(check_if_new_filenames_already_exist(filenames, new_filenames));
}

TEST(RenameFilenames, FilenameDoesNotExist)
{
    Directory_wrapper parent_directory;
    fs::path non_existing_filename{parent_directory.path() / "file.jpg"};
    fs::path new_filename{parent_directory.path() / "1.jpg"};

    ASSERT_FALSE(rename_filenames({non_existing_filename}, {new_filename}));
}

TEST(RenameFilenames, FilenameIsRenamed)
{
    Directory_wrapper parent_directory;
    File_wrapper file{parent_directory, "file.jpg"};
    fs::path new_filename{parent_directory.path() / "1.jpg"};

    ASSERT_TRUE(rename_filenames({file.path()}, {new_filename}));

    auto filenames = get_filenames_from_directory(parent_directory.path());

    ASSERT_THAT(filenames, testing::Contains(new_filename));
}

TEST(RenamePhotosAndVideosFromDirectory, PhotosAndVideosAreRenamed)
{
    Directory_wrapper parent_directory;
    File_wrapper file_1{parent_directory, "a.jpg"};
    File_wrapper file_2{parent_directory, "2.jpg"};
    File_wrapper file_3{parent_directory, "c.jpg"};
    File_wrapper file_4{parent_directory, "4.jpg"};
    File_wrapper file_5{parent_directory, "e.jpg"};

    ASSERT_TRUE(rename_photos_and_videos_from_directory(parent_directory.path()));

    ASSERT_THAT(get_filenames_from_directory(parent_directory.path()), testing::ElementsAre(fs::path{parent_directory.path() / "1.jpg"}, fs::path{parent_directory.path() / "2.jpg"}, fs::path{parent_directory.path() / "3.jpg"}, fs::path{parent_directory.path() / "4.jpg"}, fs::path{parent_directory.path() / "5.jpg"}));
}
