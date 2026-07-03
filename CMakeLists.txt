cmake_minimum_required(VERSION 3.15)
project(kronknet LANGUAGES C)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

include(FetchContent)

FetchContent_Declare(
    heracles
    GIT_REPOSITORY https://github.com/GauthierMalfilatre/Heracles
    GIT_TAG        main
)

FetchContent_MakeAvailable(heracles)

file(GLOB_RECURSE KRONKNET_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(kronknet-obj OBJECT ${KRONKNET_SOURCES})

set_target_properties(kronknet-obj PROPERTIES
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(kronknet-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(kronknet-obj PRIVATE
    -Wall
    -Wextra
    -O3
)

add_library(kronknet-static STATIC $<TARGET_OBJECTS:kronknet-obj>)
target_link_libraries(kronknet-static PUBLIC kronknet-obj) 

add_library(kronknet-shared SHARED $<TARGET_OBJECTS:kronknet-obj>)
target_link_libraries(kronknet-shared PUBLIC kronknet-obj)
target_compile_definitions(kronknet-shared PRIVATE KRONKNET_BUILD_DLL)

add_library(kronknet ALIAS kronknet-static)

set_target_properties(kronknet-static PROPERTIES OUTPUT_NAME kronknet)
set_target_properties(kronknet-shared PROPERTIES OUTPUT_NAME kronknet)

include(GNUInstallDirs)

install(TARGETS kronknet-static kronknet-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronknet
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
