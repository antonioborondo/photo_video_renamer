#pragma once

#include <filesystem>
#include <vector>

bool directory_exists(const std::filesystem::path& directory);

bool filename_is_photo_or_video(const std::filesystem::path& filename);

std::vector<std::filesystem::path> get_filenames_from_directory(const std::filesystem::path& directory);

std::vector<std::filesystem::path> generate_new_filenames(const std::vector<std::filesystem::path>& filenames, const std::string& prefix = "");

bool check_if_new_filenames_already_exist(const std::vector<std::filesystem::path>& filenames, const std::vector<std::filesystem::path>& new_filenames);

bool rename_photos_and_videos_from_directory(const std::filesystem::path& directory);
