#include <filesystem>

bool directory_exists(const std::filesystem::path directory);

bool rename_photos_and_videos_from_directory(const std::filesystem::path& directory);
