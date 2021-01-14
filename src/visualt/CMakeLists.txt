include(TestBigEndian)
include(utils)

#=============================
# Configuration
#=============================
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

set(sources "visualt.c")

list(GET private_headers_dirs 0 dir)
rebase_files(private_headers
             ${dir}
             FILES
             "visualt-internals.h")
list(GET private_headers_dirs 1 dir)
rebase_files(private_headers
             ${dir}
             FILES
             "version.h"
             "buildDate.h")

list(GET public_headers_dirs 0 dir)
rebase_files(public_headers
             ${dir}
             FILES
             "visualt/visualt.h")

#=============================
# Targets
#=============================
add_library(VisualT_library
            ${private_headers}
            ${public_headers}
            ${sources})
add_library(VisualT::library ALIAS VisualT_library)
set_target_properties(VisualT_library PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR})

#=============================
# Targets Configuration
#=============================
set_target_properties(VisualT_library PROPERTIES
                      OUTPUT_NAME visualt
                      EXPORT_NAME library
                      PUBLIC_HEADER "${public_headers}")
target_include_directories(VisualT_library
                           PRIVATE ${private_headers_dirs}
                           PUBLIC "$<BUILD_INTERFACE:${public_headers_dirs}>")
option(VisualT_ASSUME_LITTLE_ENDIAN "Skip the endianness check and assume a little endian system." NO)
if(NOT VisualT_ASSUME_LITTLE_ENDIAN)
    test_big_endian(isBigEndian)
    if(isBigEndian)
        target_compile_definitions(VisualT_library
                                   PRIVATE VISUALT_BIG_ENDIAN)
    endif()
endif()

#=============================
# Installation
#=============================
install(TARGETS VisualT_library
        EXPORT VisualT_runtime
        COMPONENT VisualT_runtime
        PUBLIC_HEADER
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/visualt"
        COMPONENT VisualT_development
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT VisualT_runtime
        COMPONENT VisualT_runtime
        DESTINATION ${VisualT_export_dir}
        NAMESPACE "${PROJECT_NAME}::")
install_symbolic(EXPORT VisualT_development
                 COMPONENT VisualT_development
                 DESTINATION ${VisualT_export_dir})


