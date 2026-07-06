cmake_minimum_required(VERSION 3.15)
project(kronknet LANGUAGES C)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

file(GLOB_RECURSE KRONKNET_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

set(CMAKE_C_VISIBILITY_PRESET hidden)

include(GenerateExportHeader)

# Static
add_library(kronknet-static STATIC ${KRONKNET_SOURCES})
set_target_properties(kronknet-static PROPERTIES OUTPUT_NAME kronknet)

target_include_directories(kronknet-static PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Shared
add_library(kronknet-shared SHARED ${KRONKNET_SOURCES})
set_target_properties(kronknet-shared PROPERTIES OUTPUT_NAME kronknet)

if(WIN32)
    target_link_libraries(kronknet-shared PRIVATE ws2_32)
    target_link_libraries(kronknet-static PRIVATE ws2_32)
endif()

target_include_directories(kronknet-shared PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# generate_export_header(kronknet-shared
#   BASE_NAME KRONKNET
#   EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/kronknet_export.h"
# )
target_include_directories(kronknet-shared PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

if(MSVC)
    target_compile_options(kronknet-static PRIVATE /W4 /O2)
    target_compile_options(kronknet-shared PRIVATE /W4 /O2)
else()
    target_compile_options(kronknet-static PRIVATE -Wall -Wextra -O3)
    target_compile_options(kronknet-shared PRIVATE -Wall -Wextra -O3)
endif()

add_library(kronknet ALIAS kronknet-static)

include(GNUInstallDirs)

install(TARGETS kronknet-static kronknet-shared
    EXPORT kronknetTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronknet
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/kronknet_export.h"
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
