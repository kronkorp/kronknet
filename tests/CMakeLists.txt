cmake_minimum_required(VERSION 3.20)
project(kronknet_tests LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c)

add_executable(kronknet_tests ${SOURCES})

target_include_directories(kronknet_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

target_link_libraries(kronknet_tests PRIVATE kronklab kronknet)
