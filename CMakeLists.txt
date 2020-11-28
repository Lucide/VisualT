# Preamble
cmake_minimum_required(VERSION 3.17)

project(VisualT
        VERSION 3.2.0
        DESCRIPTION "a text-based graphic library"
        HOMEPAGE_URL "https://github.com/Lucide/VisualT"
        LANGUAGES C)
enable_testing()

# Project wide setup
get_property(isMultiConfig GLOBAL
             PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig)
    set(allowedBuildTypes Debug Release RelWithDebInfo MinSizeRel)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                 STRINGS "${allowedBuildTypes}")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                 HELPSTRING "Supported build types are: Debug, Release (default), RelWithDebInfo, and MinSizeRel")
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release
            CACHE STRING "Build type defaults to Release" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
        message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()
list(APPEND CMAKE_MODULE_PATH "${VisualT_SOURCE_DIR}/cmake")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

# Externally provided content
add_subdirectory(dependencies)

# Main targets built by this project
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(doc)

# Things typically only needed if we are the top level project

include(GNUInstallDirs)
message(
        "installation directories:\n"
        "prefix \"${CMAKE_INSTALL_PREFIX}\"\n"
        "ARCHIVE \"${CMAKE_INSTALL_LIBDIR}\"\n"
        "LIBRARY \"${CMAKE_INSTALL_LIBDIR}\"\n"
        "RUNTIME \"${CMAKE_INSTALL_BINDIR}\"\n"
        "PUBLIC_HEADER \"${CMAKE_INSTALL_INCLUDEDIR}\"\n"
        "PRIVATE_HEADER \"${CMAKE_INSTALL_INCLUDEDIR}\"\n"
)

#set(version_in_path "${VisualT_SOURCE_DIR}/src/version.h.in")
#set(version_configured_path "${VisualT_BINARY_DIR}/src/version.h")
#set(doxyfile_in_path "${VisualT_SOURCE_DIR}/doxyfile.in")
#set(doxyfile_configured_path "${VisualT_BINARY_DIR}/doxyfile")