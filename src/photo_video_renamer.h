#pragma once

#include "progress_tracker.h"

#include <filesystem>
#include <vector>

class PhotoVideoRenamer
{
    ProgressTracker& progress_tracker_;

public:
    PhotoVideoRenamer(ProgressTracker& progress);

    bool DirectoryExists(const std::filesystem::path& directory);

    bool FilenameIsPhotoOrVideo(const std::filesystem::path& filename);

    std::vector<std::filesystem::path> GetFilenamesFromDirectory(const std::filesystem::path& directory);

    std::vector<std::filesystem::path> GenerateNewFilenames(const std::vector<std::filesystem::path>& filenames, const std::string& prefix = "");

    bool CheckIfNewFilenamesAlreadyExist(const std::vector<std::filesystem::path>& filenames, const std::vector<std::filesystem::path>& new_filenames);

    bool RenameFilenames(const std::vector<std::filesystem::path>& filenames, const std::vector<std::filesystem::path>& new_filenames);

    bool RenamePhotosAndVideosFromDirectory(const std::filesystem::path& directory);
};
