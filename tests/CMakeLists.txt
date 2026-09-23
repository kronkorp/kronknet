# NOTE: Included from the root CMakeLists.txt with -DBUILD_TESTS=ON

include(FetchContent)

FetchContent_Declare(
    kronklab
    GIT_REPOSITORY https://github.com/kronkorp/kronklab
    GIT_TAG        main
)

FetchContent_MakeAvailable(kronklab)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/network/*.c
)

add_executable(kronknet_tests ${SOURCES})

target_include_directories(kronknet_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

target_link_libraries(kronknet_tests PRIVATE kronklab kronknet)

add_test(NAME kronknet_tests COMMAND kronknet_tests)

# NOTE: kronklab has no per-test timeout, so a stuck test must not hang CI
set_tests_properties(kronknet_tests PROPERTIES TIMEOUT 120)
