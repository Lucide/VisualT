# Works with 3.11 and tested through 3.15
cmake_minimum_required(VERSION 3.11...3.16)

# Project name and a few useful settings. Other commands can pick up the results
project(VisualT
        VERSION 2.0.0
        DESCRIPTION "a text-based graphic library"
        LANGUAGES C)

add_library(GNUWarnings INTERFACE)
target_compile_options(GNUWarnings INTERFACE -Werror -Wall -Wextra)

# library code is here
add_subdirectory(src)

# examples are here
add_subdirectory(examples)

# tests and resources are here
add_subdirectory(res)
