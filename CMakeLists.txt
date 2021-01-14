#=============================
# Preamble
#=============================
cmake_minimum_required(VERSION 3.17)

unset(HUNTER_STATUS_DEBUG CACHE)
#=============================
# Hunter-related setup
#=============================
set(HUNTER_ROOT "${CMAKE_CURRENT_BINARY_DIR}/hunter" CACHE PATH "Path to Hunter root directory.")
option(HUNTER_BUILD_SHARED_LIBS "Build dependencies as shared libraries. Not explicitly supported." NO)
mark_as_advanced(HUNTER_ROOT HUNTER_BUILD_SHARED_LIBS)
option(VisualT_BUILD_SHARED_LIBS "Build VisualT libraries as shared libraries. A more specific alternative to BUILD_SHARED_LIBS." NO)
if(VisualT_BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS YES)
endif()
option(VisualT_XP_IMPORTER "Build the .xp importer library." NO)
if(VisualT_XP_IMPORTER)
    list(APPEND HUNTER_PACKAGES "ZLIB")
    if(BUILD_SHARED_LIBS)
        list(APPEND HUNTER_ZLIB_CMAKE_ARGS
             "CMAKE_POSITION_INDEPENDENT_CODE:BOOL=YES")
    endif()
list(APPEND HUNTER_ZLIB_CMAKE_ARGS
     "CMAKE_POLICY_DEFAULT_CMP0063:STRING=NEW"
     "CMAKE_C_VISIBILITY_PRESET:BOOL=hidden"
     "CMAKE_VISIBILITY_INLINES_HIDDEN:BOOL=YES")
endif()

include(FetchContent)
fetchcontent_declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
fetchcontent_makeavailable(SetupHunter)

#=============================
# Project wide setup
#=============================
project(VisualT
        VERSION 2.0.0
        DESCRIPTION "a text-based graphic library"
        HOMEPAGE_URL "https://lucide.github.io/VisualT"
        LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
#enable_testing()

set(CMAKE_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME}")
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
# Top level project only
#=============================
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(DEBUG "installation directories:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(DEBUG "default prefix \"${CMAKE_INSTALL_PREFIX}\"")
    message(DEBUG "BINDIR \"${CMAKE_INSTALL_BINDIR}\"")
    message(DEBUG "LIBDIR \"${CMAKE_INSTALL_LIBDIR}\"")
    message(DEBUG "INCLUDEDIR \"${CMAKE_INSTALL_INCLUDEDIR}\"")
    message(DEBUG "DATAROOTDIR \"${CMAKE_INSTALL_DATAROOTDIR}\"")
    message(DEBUG "DATADIR \"${CMAKE_INSTALL_DATADIR}\"")
    message(DEBUG "DOCDIR \"${CMAKE_INSTALL_DOCDIR}\"")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()

#=============================
# Subdirectories
#=============================
add_subdirectory(src)
add_subdirectory(tests)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples)
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
