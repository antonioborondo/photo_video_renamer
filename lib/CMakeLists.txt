set(NATURAL_SORT_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/natural_sort/include/natural_sort/natural_sort.hpp)

if(NOT EXISTS ${NATURAL_SORT_LIBRARY})
    message(STATUS "Downloading NaturalSort library from https://github.com/scopeInfinity/NaturalSort")
    file(DOWNLOAD https://raw.githubusercontent.com/scopeInfinity/NaturalSort/master/natural_sort.hpp ${NATURAL_SORT_LIBRARY})
endif()

add_library(natural_sort INTERFACE)

target_include_directories(natural_sort
    INTERFACE
    natural_sort/include
)
