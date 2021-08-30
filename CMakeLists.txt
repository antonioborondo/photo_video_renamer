cmake_minimum_required(VERSION 3.8)

project("photo_video_renamer")

set(CMAKE_CXX_STANDARD 20)

add_executable(photo_video_renamer
    src/main.cpp
)

target_link_libraries(photo_video_renamer
    PRIVATE
    natural_sort
)

add_subdirectory(lib)
