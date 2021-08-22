# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("renamer")

set(CMAKE_CXX_STANDARD 17)

##### NaturalSort library #####
set(NATURAL_SORT_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/lib/natural_sort/include/natural_sort/natural_sort.hpp)

if(NOT EXISTS ${NATURAL_SORT_LIBRARY})
    message(STATUS "Downloading NaturalSort from https://github.com/scopeInfinity/NaturalSort")
    file(DOWNLOAD "https://raw.githubusercontent.com/scopeInfinity/NaturalSort/master/natural_sort.hpp" ${NATURAL_SORT_LIBRARY})
endif()

add_library(natural_sort INTERFACE)

target_include_directories(natural_sort INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/lib/natural_sort/include)
##### NaturalSort library #####

# Include sub-projects.
add_subdirectory ("renamer")
