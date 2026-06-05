cmake_minimum_required(VERSION 3.5)
project(kronknet C)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier build/.")
endif()

file(GLOB_RECURSE KRONKNET_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(kronknet STATIC ${KRONKNET_SOURCES})

target_include_directories(kronknet PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(kronknet PRIVATE 
    -Wall 
    -Wextra 
    -O3
)
