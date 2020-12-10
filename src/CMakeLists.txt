include(TestBigEndian)
include(GNUInstallDirs)
include(utils)

#=============================
# Configuration
#=============================
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)
option(VisualT_BUILD_SHARED_LIBS "Build VisualT as a shared library" NO)
if(VisualT_BUILD_SHARED_LIB)
    set(VisualT_BUILD_SHARED_LIB SHARED)
else()
    set(VisualT_BUILD_SHARED_LIB "")
endif()

set(private_headers
    version.h
    buildDate.h)
set(public_headers
    "visualt/visualt.h")
set(sources "visualt.c")
set(private_headers_dir ${CMAKE_CURRENT_BINARY_DIR})
set(public_headers_dir "${PROJECT_BINARY_DIR}/include")
utils_rebase_paths(private_headers_paths
                   ${private_headers_dir}
                   FILES ${private_headers})
utils_rebase_paths(public_headers_paths
                   ${public_headers_dir}
                   FILES ${public_headers})

#=============================
# Targets
#=============================
add_library(VisualT_library ${VisualT_BUILD_SHARED_LIB} ${private_headers_paths} ${public_headers_paths} ${sources})
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
                      PRIVATE_HEADER "${private_headers_paths}"
                      PUBLIC_HEADER "${public_headers_paths}")
target_include_directories(VisualT_library
                           PRIVATE ${private_headers_dir}
                           PUBLIC "$<BUILD_INTERFACE:${public_headers_dir}>")
test_big_endian(isBigEndian)
if(isBigEndian)
    target_compile_definitions(VisualT_library
                               PRIVATE VISUALT_BIG_ENDIAN)
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
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
file(COPY
     "${PROJECT_SOURCE_DIR}/include/visualt/"
     DESTINATION "${public_headers_dir}/visualt")
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
utils_rebase_paths(public_headers_install_paths
                   ${CMAKE_INSTALL_INCLUDEDIR}
                   FILES ${public_headers})
install(TARGETS VisualT_library
        EXPORT ${PROJECT_NAME}
        COMPONENT VisualT_library
        INCLUDES DESTINATION ${public_headers_install_paths})
install(EXPORT ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        NAMESPACE "${PROJECT_NAME}::")


