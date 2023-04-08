#include "filesystem_wrappers.h"
#include "photo_video_renamer.h"
#include "printer.h"
#include "progress_tracker.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

class PhotoVideoRenamerTest: public testing::Test
{
    std::ostringstream sink_;

    Printer printer_;

    ProgressTracker progress_tracker_;

protected:
    PhotoVideoRenamer photo_video_renamer_;

    PhotoVideoRenamerTest():
        printer_{sink_},
        progress_tracker_{printer_},
        photo_video_renamer_{progress_tracker_}
    {
    }
};

TEST_F(PhotoVideoRenamerTest, NonExistingDirectoryDoesNotExists)
{
    const fs::path non_existing_directory{std::tmpnam(nullptr)};

    ASSERT_FALSE(photo_video_renamer_.DirectoryExists(non_existing_directory));
}

TEST_F(PhotoVideoRenamerTest, ExistingFileIsNotDirectory)
{
    DirectoryWrapper parent_directory;
    FileWrapper file{parent_directory, "file.txt"};

    ASSERT_FALSE(photo_video_renamer_.DirectoryExists(file.path()));
}

TEST_F(PhotoVideoRenamerTest, ExistingDirectoryExists)
{
    DirectoryWrapper directory;

    ASSERT_TRUE(photo_video_renamer_.DirectoryExists(directory.path()));
}

TEST_F(PhotoVideoRenamerTest, FilenameIsPhotoOrVideoInLowerAndUpperCase)
{
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.avi"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.AVI"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.bmp"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.BMP"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.heic"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.HEIC"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.jpeg"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.JPEG"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.jpg"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.JPG"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.mov"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.MOV"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.mp4"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.MP4"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.png"));
    ASSERT_TRUE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.PNG"));
}

TEST_F(PhotoVideoRenamerTest, FilenameIsNotPhotoOrVideoInLowerAndUpperCase)
{
    ASSERT_FALSE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.TXT"));
    ASSERT_FALSE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.pdf"));
    ASSERT_FALSE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.PDF"));
    ASSERT_FALSE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.doc"));
    ASSERT_FALSE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.txt"));
    ASSERT_FALSE(photo_video_renamer_.FilenameIsPhotoOrVideo("file.DOC"));
}

TEST_F(PhotoVideoRenamerTest, GetFilenamesFromDirectoryDoesNotReturnFilenamesIfDirectoryIsEmpty)
{
    DirectoryWrapper empty_directory;
    const auto filenames{photo_video_renamer_.GetFilenamesFromDirectory(empty_directory.path())};

    ASSERT_THAT(filenames, testing::IsEmpty());
}

TEST_F(PhotoVideoRenamerTest, GetFilenamesFromDirectoryOnlyReturnsPhotosAndVideos)
{
    DirectoryWrapper parent_directory;
    FileWrapper file_01{parent_directory, "file_01.avi"};
    FileWrapper file_02{parent_directory, "file_02.bmp"};
    FileWrapper file_03{parent_directory, "file_03.heic"};
    FileWrapper file_04{parent_directory, "file_04.jpeg"};
    FileWrapper file_05{parent_directory, "file_05.jpg"};
    FileWrapper file_06{parent_directory, "file_06.mov"};
    FileWrapper file_07{parent_directory, "file_07.mp4"};
    FileWrapper file_08{parent_directory, "file_08.png"};
    FileWrapper file_09{parent_directory, "file_09.txt"};
    FileWrapper file_10{parent_directory, "file_10.pdf"};
    FileWrapper file_11{parent_directory, "file_11.doc"};

    const auto filenames{photo_video_renamer_.GetFilenamesFromDirectory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::UnorderedElementsAre(file_01.path(), file_02.path(), file_03.path(), file_04.path(), file_05.path(), file_06.path(), file_07.path(), file_08.path()));
}

TEST_F(PhotoVideoRenamerTest, GetFilenamesFromDirectoryReturnsFilenamesSorted)
{
    DirectoryWrapper parent_directory;
    FileWrapper file_1{parent_directory, "file_1.avi"};
    FileWrapper file_2{parent_directory, "file_2.bmp"};
    FileWrapper file_3{parent_directory, "file_3.heic"};
    FileWrapper file_4{parent_directory, "file_4.jpeg"};
    FileWrapper file_5{parent_directory, "file_5.jpg"};
    FileWrapper file_6{parent_directory, "file_6.mov"};
    FileWrapper file_7{parent_directory, "file_7.mp4"};
    FileWrapper file_8{parent_directory, "file_8.png"};

    const auto filenames{photo_video_renamer_.GetFilenamesFromDirectory(parent_directory.path())};

    ASSERT_THAT(filenames, testing::ElementsAre(file_1.path(), file_2.path(), file_3.path(), file_4.path(), file_5.path(), file_6.path(), file_7.path(), file_8.path()));
}

TEST_F(PhotoVideoRenamerTest, GenerateNewFilenamesReturnsSameNumberOfFilesAsReceived)
{
    for(int i{}; i < 10; ++i)
    {
        ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames(std::vector<fs::path>(i)), testing::SizeIs(i));
    }
}

TEST_F(PhotoVideoRenamerTest, GenerateNewFilenamesReturnsCorrectNumberOfLeadingZeros)
{
    ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames(std::vector<fs::path>(9)), testing::IsSupersetOf({fs::path{"1"}, fs::path{"9"}}));
    ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames(std::vector<fs::path>(99)), testing::IsSupersetOf({fs::path{"01"}, fs::path{"99"}}));
    ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames(std::vector<fs::path>(999)), testing::IsSupersetOf({fs::path{"001"}, fs::path{"010"}, fs::path{"999"}}));
    ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames(std::vector<fs::path>(9999)), testing::IsSupersetOf({fs::path{"0001"}, fs::path{"0010"}, fs::path{"0100"}, fs::path{"9999"}}));
}

TEST_F(PhotoVideoRenamerTest, GenerateNewFilenamesReturnsExtensionInLowerCase)
{
    ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames({fs::path{"file.TXT"}}), testing::ElementsAre(fs::path{"1.txt"}));
}

TEST_F(PhotoVideoRenamerTest, GenerateNewFilenamesKeepParentDirectory)
{
    const std::vector<fs::path> filenames{fs::path{"C:/1/2/3/4/5/file.txt"}};
    const auto new_filenames{photo_video_renamer_.GenerateNewFilenames(filenames)};

    ASSERT_EQ(filenames.at(0).parent_path(), new_filenames.at(0).parent_path());
}

TEST_F(PhotoVideoRenamerTest, GenerateNewFilenamesAddsPrefixToFilename)
{
    constexpr auto prefix{"test_"};

    ASSERT_THAT(photo_video_renamer_.GenerateNewFilenames(std::vector<fs::path>(1), prefix), testing::ElementsAre(fs::path{"test_1"}));
}

TEST_F(PhotoVideoRenamerTest, CheckIfNewFilenamesAlreadyExistsFindsOneFilename)
{
    const std::vector<fs::path> filenames{fs::path{"1"}, fs::path{"2"}, fs::path{"3"}};
    const std::vector<fs::path> new_filenames{fs::path{"a"}, fs::path{"2"}, fs::path{"c"}};

    ASSERT_TRUE(photo_video_renamer_.CheckIfNewFilenamesAlreadyExist(filenames, new_filenames));
}

TEST_F(PhotoVideoRenamerTest, CheckIfNewFilenamesAlreadyExistsFindsOneFilenameButWithDifferentCase)
{
    const std::vector<fs::path> filenames{fs::path{"a"}, fs::path{"b"}, fs::path{"c"}};
    const std::vector<fs::path> new_filenames{fs::path{"1"}, fs::path{"B"}, fs::path{"3"}};

    ASSERT_TRUE(photo_video_renamer_.CheckIfNewFilenamesAlreadyExist(filenames, new_filenames));
}

TEST_F(PhotoVideoRenamerTest, CheckIfNewFilenamesAlreadyExistsDoesNotFindExistingFilenames)
{
    const std::vector<fs::path> filenames{fs::path{"1"}, fs::path{"2"}, fs::path{"3"}};
    const std::vector<fs::path> new_filenames{fs::path{"a"}, fs::path{"b"}, fs::path{"c"}};

    ASSERT_FALSE(photo_video_renamer_.CheckIfNewFilenamesAlreadyExist(filenames, new_filenames));
}

TEST_F(PhotoVideoRenamerTest, RenameFilenamesDoesNotRenameIfFilenameDoesNotExist)
{
    DirectoryWrapper parent_directory;
    fs::path non_existing_filename{parent_directory.path() / "file.jpg"};
    fs::path new_filename{parent_directory.path() / "1.jpg"};

    ASSERT_FALSE(photo_video_renamer_.RenameFilenames({non_existing_filename}, {new_filename}));
}

TEST_F(PhotoVideoRenamerTest, RenameFilenamesRenamesFilenames)
{
    DirectoryWrapper parent_directory;
    FileWrapper file{parent_directory, "file.jpg"};
    fs::path new_filename{parent_directory.path() / "1.jpg"};

    ASSERT_TRUE(photo_video_renamer_.RenameFilenames({file.path()}, {new_filename}));

    auto filenames = photo_video_renamer_.GetFilenamesFromDirectory(parent_directory.path());

    ASSERT_THAT(filenames, testing::Contains(new_filename));
}

TEST_F(PhotoVideoRenamerTest, RenamePhotosAndVideosFromDirectoryRenamesPhotosAndVideosFromDirectory)
{
    DirectoryWrapper parent_directory;
    FileWrapper file_1{parent_directory, "a.jpg"};
    FileWrapper file_2{parent_directory, "2.jpg"};
    FileWrapper file_3{parent_directory, "c.jpg"};
    FileWrapper file_4{parent_directory, "4.jpg"};
    FileWrapper file_5{parent_directory, "e.jpg"};

    ASSERT_TRUE(photo_video_renamer_.RenamePhotosAndVideosFromDirectory(parent_directory.path()));

    ASSERT_THAT(photo_video_renamer_.GetFilenamesFromDirectory(parent_directory.path()), testing::ElementsAre(fs::path{parent_directory.path() / "1.jpg"}, fs::path{parent_directory.path() / "2.jpg"}, fs::path{parent_directory.path() / "3.jpg"}, fs::path{parent_directory.path() / "4.jpg"}, fs::path{parent_directory.path() / "5.jpg"}));
}
