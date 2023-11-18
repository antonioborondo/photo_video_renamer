cmake_minimum_required(VERSION 3.25)

project(photo_video_renamer VERSION 1.5.0)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

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

install(TARGETS photo_video_renamer DESTINATION bin)

install(FILES res/icon.ico DESTINATION res)

set(VC_REDISTRIBUTABLE ${CMAKE_BINARY_DIR}/vc_redist.x64.exe)

if(NOT EXISTS ${VC_REDISTRIBUTABLE})
    message(STATUS "Downloading Microsoft Visual C++ Redistributable from https://aka.ms/vs/17/release/vc_redist.x64.exe")
    file(DOWNLOAD "https://aka.ms/vs/17/release/vc_redist.x64.exe" ${VC_REDISTRIBUTABLE})
endif()

install(PROGRAMS ${VC_REDISTRIBUTABLE} DESTINATION bin)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
    "
    ExecWait '$INSTDIR\\\\bin\\\\vc_redist.x64.exe /install /quiet /norestart'
    WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer' '' 'Rename photos and videos'
    WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer' 'Icon' '$INSTDIR\\\\res\\\\icon.ico'
    WriteRegStr HKCR 'Directory\\\\shell\\\\photo_video_renamer\\\\command' '' '$INSTDIR\\\\bin\\\\photo_video_renamer.exe %1'
    WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer' '' 'Rename photos and videos'
    WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer' 'Icon' '$INSTDIR\\\\res\\\\icon.ico'
    WriteRegStr HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer\\\\command' '' '$INSTDIR\\\\bin\\\\photo_video_renamer.exe'
    EnVar::SetHKLM
    EnVar::AddValue 'PATH' '$INSTDIR\\\\bin'
    "
)

SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
    "
    DeleteRegKey HKCR 'Directory\\\\shell\\\\photo_video_renamer'
    DeleteRegKey HKCR 'Directory\\\\Background\\\\shell\\\\photo_video_renamer'
    EnVar::SetHKLM
    EnVar::DeleteValue 'PATH' '$INSTDIR\\\\bin'
    "
)

include(CPack)
