cmake_minimum_required(VERSION 3.20)
project(kronknet_tests LANGUAGES C)

include(FetchContent)

FetchContent_Declare(
    kronklab
    GIT_REPOSITORY https://github.com/kronkorp/kronklab
    GIT_TAG        main
)

FetchContent_MakeAvailable(kronklab)

# NOTE: Build kronknet from this repository, not the installed one
if(NOT TARGET kronknet)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/kronknet)
endif()

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/network/*.c
)

add_executable(kronknet_tests ${SOURCES})

target_include_directories(kronknet_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

target_link_libraries(kronknet_tests PRIVATE kronklab kronknet)
