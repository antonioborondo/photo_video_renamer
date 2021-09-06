#pragma once

#include <filesystem>

bool directory_exists(const std::filesystem::path& directory);

bool filename_is_photo_or_video(const std::filesystem::path& filename);

bool rename_photos_and_videos_from_directory(const std::filesystem::path& directory);
