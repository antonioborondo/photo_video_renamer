#pragma once

#include "percentage_calculator.h"
#include "printer.h"

#include <filesystem>
#include <vector>

class PhotoVideoRenamer
{
    Printer printer_;

    PercentageCalculator percentage_calculator_;

public:
    PhotoVideoRenamer(const Printer& printer);

    bool DirectoryExists(const std::filesystem::path& directory);

    bool FilenameIsPhotoOrVideo(const std::filesystem::path& filename);

    std::vector<std::filesystem::path> GetFilenamesFromDirectory(const std::filesystem::path& directory);

    std::vector<std::filesystem::path> GenerateNewFilenames(const std::vector<std::filesystem::path>& filenames, const std::string& prefix = "");

    bool CheckIfNewFilenamesAlreadyExist(const std::vector<std::filesystem::path>& filenames, const std::vector<std::filesystem::path>& new_filenames);

    bool RenameFilenames(const std::vector<std::filesystem::path>& filenames, const std::vector<std::filesystem::path>& new_filenames);

    bool RenamePhotosAndVideosFromDirectory(const std::filesystem::path& directory);
};
