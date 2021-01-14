#=============================
# Preamble
#=============================
cmake_minimum_required(VERSION 3.17)
project(VTExamples
        VERSION 1.0.0
        DESCRIPTION "a CMake project containing various VisualT examples"
        HOMEPAGE_URL "https://lucide.github.io/VisualT"
        LANGUAGES C)

#=============================
# Project wide setup
#=============================
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#=============================
# Context-specific cmake code
#=============================
include(context-specific)

#=============================
# Subdirectories
#=============================
add_subdirectory(utilities)
add_subdirectory(demo)
add_subdirectory(car)
add_subdirectory(waves)