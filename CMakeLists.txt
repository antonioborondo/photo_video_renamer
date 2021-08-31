cmake_minimum_required(VERSION 3.8)

project("photo_video_renamer")

set(CMAKE_CXX_STANDARD 17)

set(CONAN ${CMAKE_BINARY_DIR}/conan.cmake)

if(NOT EXISTS ${CONAN})
    message(STATUS "Downloading Conan from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" ${CONAN})
endif()

include(${CONAN})

conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
)

add_executable(photo_video_renamer
    src/main.cpp
)

target_link_libraries(photo_video_renamer
    PRIVATE
    CONAN_PKG::fmt
    natural_sort
)

add_subdirectory(lib)
