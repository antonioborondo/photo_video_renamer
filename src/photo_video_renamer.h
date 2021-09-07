#pragma once

#include <filesystem>

bool directory_exists(const std::filesystem::path& directory);

bool filename_is_photo_or_video(const std::filesystem::path& filename);

std::vector<std::filesystem::path> get_filenames_from_directory(const std::filesystem::path& directory);

bool rename_photos_and_videos_from_directory(const std::filesystem::path& directory);
