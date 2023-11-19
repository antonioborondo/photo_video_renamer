cmake_minimum_required(VERSION 3.27.7)

project(photo_video_renamer VERSION 1.6.0)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_BINARY_DIR})

add_library(photo_video_renamer_common STATIC
    src/photo_video_renamer.cc
    src/printer.cc
    src/progress_tracker.cc
)

target_include_directories(photo_video_renamer_common
    PUBLIC
    src
)

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(photo_video_renamer_common
    PRIVATE
    Boost::filesystem
    fmt::fmt
    natural_sort
)

add_executable(photo_video_renamer
    src/main.cc
)

target_link_libraries(photo_video_renamer
    PRIVATE
    fmt::fmt
    photo_video_renamer_common
)

add_executable(photo_video_renamer_test
    test/photo_video_renamer_test.cc
    test/printer_test.cc
    test/progress_tracker_test.cc
)

find_package(GTest REQUIRED)

target_link_libraries(photo_video_renamer_test
    PRIVATE
    GTest::gmock
    GTest::gtest_main
    photo_video_renamer_common
)

enable_testing()

add_test(NAME photo_video_renamer_test COMMAND photo_video_renamer_test)

add_subdirectory(third_party)

include(pack)
