#=============================
# Preamble
#=============================
cmake_minimum_required(VERSION 3.17)
project(VisualT
        VERSION 2.0.0
        DESCRIPTION "a text-based graphic library"
        HOMEPAGE_URL "https://lucide.github.io/VisualT"
        LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
#enable_testing()

#=============================
# Top level project only
#=============================
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME}")
    message("installation directories:\n"
            "prefix \"${CMAKE_INSTALL_PREFIX}\"\n"
            "BINDIR \"${CMAKE_INSTALL_BINDIR}\"\n"
            "LIBDIR \"${CMAKE_INSTALL_LIBDIR}\"\n"
            "INCLUDEDIR \"${CMAKE_INSTALL_INCLUDEDIR}\"\n"
            "DATAROOTDIR \"${CMAKE_INSTALL_DATAROOTDIR}\"\n"
            "DATADIR \"${CMAKE_INSTALL_DATADIR}\"\n"
            "DOCDIR \"${CMAKE_INSTALL_DOCDIR}\"\n")
endif()

#=============================
# Project wide setup
#=============================
set(VisualT_export_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
get_property(isMultiConfig GLOBAL
             PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig)
    set(allowedBuildTypes Debug Release RelWithDebInfo MinSizeRel)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                 STRINGS "${allowedBuildTypes}")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                 HELPSTRING "Supported build types are: Debug, Release (default), RelWithDebInfo, and MinSizeRel")
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
        message(STATUS "CMAKE_BUILD_TYPE is undefined. VisualT will default to ${CMAKE_BUILD_TYPE}")
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
        message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#=============================
# Subdirectories
#=============================
add_subdirectory(dependencies)
add_subdirectory(src)
add_subdirectory(tests)
option(VisualT_HIDE_EXAMPLES_TARGETS "Disables the examples targets. Reduces crowding when you import the project directly." NO)
if(NOT VisualT_HIDE_EXAMPLES_TARGETS)
    add_subdirectory(examples)
endif()
option(VisualT_HIDE_DOC_TARGETS "Disables the documentation targets. Reduces crowding when you import the project directly." NO)
if(NOT VisualT_HIDE_DOC_TARGETS)
    add_subdirectory(doc)
endif()

#=============================
# Installation
#=============================
write_basic_package_version_file("${PROJECT_BINARY_DIR}/VisualTVersion.cmake"
                                 COMPATIBILITY SameMinorVersion)
install(FILES
        VisualTConfig.cmake
        "${PROJECT_BINARY_DIR}/VisualTVersion.cmake"
        COMPONENT VisualT_runtime
        DESTINATION ${VisualT_export_dir})
