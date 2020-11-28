include(TestBigEndian)

set(private_headers_path "${VisualT_BINARY_DIR}/src")
set(public_headers_path "${VisualT_SOURCE_DIR}/include")

set(private_headers
    "${private_headers_path}/version.h"
    "${private_headers_path}/buildDate.h")
set(public_headers
    "${public_headers_path}/visualt/visualt.h"
    "${public_headers_path}/visualt/visualtUnprefixed.h")

set(sources "visualt.c")

#Configuration will be a required step in CI for version bumps
string(TIMESTAMP build_date "%d %B %Y %H:%M" UTC)
configure_file(
        buildDate.h.in
        "${CMAKE_CURRENT_BINARY_DIR}/buildDate.h"
        @ONLY)
configure_file(
        version.h.in
        "${CMAKE_CURRENT_BINARY_DIR}/version.h"
        @ONLY)


# Targets
add_library(VisualT_library ${private_headers} ${public_headers} ${sources})
add_library(VisualT::library ALIAS VisualT_library)

set_target_properties(VisualT_library PROPERTIES
                      OUTPUT_NAME visualt
                      EXPORT_NAME visualt
                      PRIVATE_HEADER "${private_headers}"
                      PUBLIC_HEADER "${public_headers}")

target_include_directories(VisualT_library
                           PRIVATE ${private_headers_path}
                           PUBLIC ${public_headers_path})

test_big_endian(isBigEndian)
if(isBigEndian)
    target_compile_definitions(VisualT_library
                               PRIVATE VISUALT_BIG_ENDIAN)
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(VisualT_library
                               PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if(CMAKE_PROJECT_NAME STREQUAL "VisualT")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(VisualT_library
                               PRIVATE
                               "$<$<CONFIG:Debug>:-O2;-Wconversion;-Wall;-Wextra;-pedantic>")
    endif()
endif()


#=============================
# Testing
#=============================
#if(TEST_BAGOFBEANS OR CMAKE_SOURCE_DIR STREQUAL BagOfBeans_SOURCE_DIR)
#    add_test(NAME Planter.NoArgs COMMAND BagOfBeans_Planter)
#    add_test(NAME Planter.WithArgs COMMAND BagOfBeans_Planter beanType=soy)
#endif()
#=============================
# Packaging
#=============================
#include(GNUInstallDirs)
#install(TARGETS BagOfBeans_Planter
#        EXPORT BagOfBeans_Apps
#        DESTINATION ${CMAKE_INSTALL_BINDIR}
#        COMPONENT BagOfBeans_Apps
#        )


