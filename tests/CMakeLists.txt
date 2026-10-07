# NOTE: Included from the root CMakeLists.txt with -DBUILD_TESTS=ON

# NOTE: The smoke test runs on every platform, against the static and the shared library
add_executable(kronknet_smoke smoke/smoke.c)
target_link_libraries(kronknet_smoke PRIVATE kronknet-static)
add_test(NAME kronknet_smoke COMMAND kronknet_smoke 42900)

add_executable(kronknet_smoke_shared smoke/smoke.c)
target_link_libraries(kronknet_smoke_shared PRIVATE kronknet-shared)
add_test(NAME kronknet_smoke_shared COMMAND kronknet_smoke_shared 42910)

set_tests_properties(kronknet_smoke kronknet_smoke_shared PROPERTIES TIMEOUT 30)

if(WIN32)
    # NOTE: Windows looks for a DLL next to the executable
    add_custom_command(TARGET kronknet_smoke_shared POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:kronknet-shared> $<TARGET_FILE_DIR:kronknet_smoke_shared>
    )

    # NOTE: kronklab runs each test in a fork(), which Windows does not have
    return()
endif()

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
