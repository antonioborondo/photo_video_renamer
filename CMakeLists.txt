cmake_minimum_required(VERSION 3.21)

project(photo_video_renamer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CONAN_CMAKE ${CMAKE_BINARY_DIR}/conan.cmake)

if(NOT EXISTS ${CONAN_CMAKE})
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake" ${CONAN_CMAKE})
endif()

include(${CONAN_CMAKE})

conan_cmake_run(
    REQUIRES
    fmt/8.0.1
    gtest/1.11.0
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

install(TARGETS photo_video_renamer DESTINATION bin)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
    "
    WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer' '' 'Rename photos and videos'
    WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer' 'Icon' '%SYSTEMROOT%\\\\System32\\\\shell32.dll,328'
    WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer\\\\command' '' '$INSTDIR\\\\bin\\\\photo_video_renamer.exe %1'
    WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer' '' 'Rename photos and videos'
    WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer' 'Icon' '%SYSTEMROOT%\\\\System32\\\\shell32.dll,328'
    WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer\\\\command' '' '$INSTDIR\\\\bin\\\\photo_video_renamer.exe'
    "
)

SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
    "
    DeleteRegKey HKCR 'Directory\\\\shell\\\\photo_video_renamer'
    DeleteRegKey HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer'
    "
)

include(CPack)
