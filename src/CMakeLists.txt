include(TestBigEndian)
include(GNUInstallDirs)
include(utils)

#=============================
# Configuration
#=============================
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)
option(VisualT_BUILD_SHARED_LIBS "Build VisualT as a shared library. A more specific alternative to BUILD_SHARED_LIBS." NO)
set_if(VisualT_BUILD_SHARED_LIB SHARED)

set(private_headers
    version.h
    buildDate.h)
set(public_headers
    "visualt/visualt.h")
set(sources "visualt.c")
set(private_headers_dir ${CMAKE_CURRENT_BINARY_DIR})
set(public_headers_dir "${PROJECT_SOURCE_DIR}/include")
rebase_dirs(private_headers_paths
            ${private_headers_dir}
            FILES ${private_headers})
rebase_dirs(public_headers_paths
            ${public_headers_dir}
            FILES ${public_headers})

#=============================
# Targets
#=============================
add_library(VisualT_library ${VisualT_BUILD_SHARED_LIB}
            ${private_headers_paths}
            ${public_headers_paths}
            ${sources})
set_target_properties(VisualT_library PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR})
add_library(VisualT::library ALIAS VisualT_library)

#=============================
# Targets Configuration
#=============================
set_target_properties(VisualT_library PROPERTIES
                      OUTPUT_NAME visualt
                      EXPORT_NAME library
                      PUBLIC_HEADER "${public_headers_paths}")
target_include_directories(VisualT_library
                           PRIVATE ${private_headers_dir}
                           PUBLIC "$<BUILD_INTERFACE:${public_headers_dir}>")
test_big_endian(isBigEndian)
if(isBigEndian)
    target_compile_definitions(VisualT_library
                               PRIVATE VISUALT_BIG_ENDIAN)
endif()
if(MSVC)
    target_compile_definitions(VisualT_library
                               PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(VisualT_library
                               PRIVATE
                               "$<$<CONFIG:Debug>:-O2;-Wconversion;-Wall;-Wextra;-pedantic>")
    endif()
endif()

#=============================
# Configure Time Generation
#=============================
string(TIMESTAMP build_date "%d %B %Y %H:%M" UTC)
configure_file(buildDate.h.in
               "${private_headers_dir}/buildDate.h"
               @ONLY)
configure_file(version.h.in
               "${private_headers_dir}/version.h"
               @ONLY)

#=============================
# Installation
#=============================
#rebase_dirs(public_headers_install_paths
#
#            FILES ${public_headers})
install(TARGETS VisualT_library
        EXPORT VisualT_runtime
        COMPONENT VisualT_runtime
        PUBLIC_HEADER
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/$<LOWER_CASE:${PROJECT_NAME}>"
        COMPONENT VisualT_development
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT VisualT_runtime
        COMPONENT VisualT_runtime
        DESTINATION ${VisualT_export_dir}
        NAMESPACE "${PROJECT_NAME}::")
install_symbolic(EXPORT VisualT_development
                 COMPONENT VisualT_development
                 DESTINATION ${VisualT_export_dir}
                 EXCLUDE_FROM_ALL)


