include(GenerateExportHeader)
include(utils)

#=============================
# Dependencies
#=============================
find_package(ZLIB REQUIRED)

#=============================
# Configuration
#=============================
set(sources
    "importXp.c"
    "loadXp.c"
    "translateXp.c")

set(private_headers_dirs
    ${src_source_dir}
    ${src_binary_dir})
rebase_files(private_headers
             ${src_source_dir}
             FILES
             "visualt-internals.h")

set(public_headers_dirs
    ${public_headers_dir}
    ${CMAKE_CURRENT_BINARY_DIR})
rebase_files(public_headers
             ${public_headers_dir}
             FILES
             "visualt/visualt.h"
             "visualt/import-xp.h")
rebase_files(public_headers
             ${CMAKE_CURRENT_BINARY_DIR}
             FILES
             "import_xp_export.h")


#=============================
# Targets
#=============================
add_library(VisualT_import_xp
            ${private_headers}
            ${public_headers}
            ${sources})
add_library(VisualT::import_xp ALIAS VisualT_import_xp)
set_target_properties(VisualT_import_xp PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR}
                      C_VISIBILITY_PRESET hidden
                      VISIBILITY_INLINES_HIDDEN YES)
generate_export_header(VisualT_import_xp
                       BASE_NAME import_xp
                       PREFIX_NAME VISUALT_)

#=============================
# Targets Configuration
#=============================
if(UNIX)
    # enable fileno()
    target_compile_definitions(VisualT_import_xp
                               PRIVATE _POSIX_C_SOURCE=1)
endif()
target_link_libraries(VisualT_import_xp PRIVATE
                      ZLIB::ZLIB)
# disable linker warnings about missing zlib .pdb files on windows
target_link_options(VisualT_import_xp
                    PUBLIC "$<$<C_COMPILER_ID:MSVC>:LINKER:/ignore:4099>")
set_target_properties(VisualT_import_xp PROPERTIES
                      OUTPUT_NAME importXp
                      EXPORT_NAME import_xp
                      PUBLIC_HEADER "${public_headers}")
target_include_directories(VisualT_import_xp
                           PRIVATE ${private_headers_dirs}
                           PUBLIC "$<BUILD_INTERFACE:${public_headers_dirs}>")

set_target_properties(ZLIB::ZLIB PROPERTIES
                      MAP_IMPORTED_CONFIG_MINSIZEREL Release
                      MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

#=============================
# Installation
#=============================
install(TARGETS VisualT_import_xp
        EXPORT VisualT_import_xp
        COMPONENT VisualT_import_xp
        PUBLIC_HEADER
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/visualt"
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT VisualT_import_xp
        COMPONENT VisualT_import_xp
        DESTINATION ${VisualT_export_dir}
        NAMESPACE "${PROJECT_NAME}::")