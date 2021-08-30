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

install(TARGETS photo_video_renamer RUNTIME DESTINATION bin)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
include(CPack)
