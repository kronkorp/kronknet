cmake_minimum_required(VERSION 3.15)
project(kronknet LANGUAGES C)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

file(GLOB_RECURSE KRONKNET_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# NOTE: What differs between platforms is written once per platform, in
#       src/platform/<platform>/: only the ones of the target are built
list(FILTER KRONKNET_SOURCES EXCLUDE REGEX "/src/platform/")

if(WIN32)
    set(KRONKNET_PLATFORMS win32)
elseif(UNIX)
    set(KRONKNET_PLATFORMS posix)
else()
    message(FATAL_ERROR "kronknet does not support ${CMAKE_SYSTEM_NAME}")
endif()

foreach(platform ${KRONKNET_PLATFORMS})
    file(GLOB platform_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/${platform}/*.c")
    list(APPEND KRONKNET_SOURCES ${platform_sources})
endforeach()

if(MSVC)
    # NOTE: No /O2 here: CMake gives it in Release, and it cannot go with the /RTC1 of Debug
    set(KRONKNET_C_FLAGS /W4)
else()
    set(KRONKNET_C_FLAGS -Wall -Wextra -O3)
endif()

# NOTE: The static and the shared library are compiled each on their own: on Windows the DLL
#       exports its API (KRONKNET_BUILD_DLL), so their objects cannot be shared
function(kronknet_add_library target type)
    add_library(${target} ${type} ${KRONKNET_SOURCES})

    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME kronknet
        C_STANDARD 11
        C_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
    )

    target_include_directories(${target}
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    target_compile_options(${target} PRIVATE ${KRONKNET_C_FLAGS})

    if(WIN32)
        # NOTE: Windows Vista (0x0600) at least, for WSAPoll, inet_ntop and inet_pton
        target_compile_definitions(${target} PRIVATE WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0600)
        target_link_libraries(${target} PRIVATE ws2_32)
    endif()
endfunction()

kronknet_add_library(kronknet-static STATIC)
kronknet_add_library(kronknet-shared SHARED)

target_compile_definitions(kronknet-shared
    PRIVATE KRONKNET_BUILD_DLL
    INTERFACE KRONKNET_USE_DLL
)

if(MSVC)
    # NOTE: The import library of the DLL is already kronknet.lib
    set_target_properties(kronknet-static PROPERTIES OUTPUT_NAME kronknet_static)
endif()

add_library(kronknet ALIAS kronknet-static)

include(GNUInstallDirs)

install(TARGETS kronknet-static kronknet-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronknet
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

option(BUILD_TESTS "Build the unit tests (run them with ctest)" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
