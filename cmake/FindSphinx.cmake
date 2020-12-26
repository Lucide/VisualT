include(FindPackageHandleStandardArgs)

find_package(Python3 REQUIRED Interpreter)
get_filename_component(_python3_dir ${Python3_EXECUTABLE} DIRECTORY)
execute_process(COMMAND ${Python3_EXECUTABLE} -m site --user-base
                OUTPUT_VARIABLE _pkg_install_dir
                OUTPUT_STRIP_TRAILING_WHITESPACE)
file(TO_CMAKE_PATH ${_pkg_install_dir} _pkg_install_dir)
find_program(Sphinx_EXECUTABLE
             "sphinx-build"
             HINTS
             ${_pkg_install_dir}
             "${_pkg_install_dir}/Python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}"
             ${_python3_dir}
             PATH_SUFFIXES
             Scripts
             NO_CMAKE_ENVIRONMENT_PATH
             NO_CMAKE_SYSTEM_PATH
             DOC "Sphinx documentation generator")
if(Sphinx_EXECUTABLE)
    execute_process(
            COMMAND ${Sphinx_EXECUTABLE} --v
            OUTPUT_VARIABLE _sphinx_version
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX MATCH [[[0-9]+\.[0-9]+\.[0-9]+]]
           _sphinx_version
           ${_sphinx_version})
else()
    set(_sphinx_version unknown)
endif()
find_package_handle_standard_args(Sphinx
                                  REQUIRED_VARS Sphinx_EXECUTABLE
                                  VERSION_VAR ${_sphinx_version})

# Create an imported target for Sphinx
if(NOT TARGET Sphinx::sphinx)
    add_executable(Sphinx::sphinx IMPORTED GLOBAL)
    set_target_properties(Sphinx::sphinx
                          PROPERTIES
                          IMPORTED_LOCATION ${Sphinx_EXECUTABLE})
endif()

option(SPHINX_HTML_OUTPUT "Build a single HTML with the whole content." ON)
option(SPHINX_DIRHTML_OUTPUT "Build HTML pages, but with a single directory per document." OFF)
option(SPHINX_HTMLHELP_OUTPUT "Build HTML pages with additional information for building a documentation collection in htmlhelp." OFF)
option(SPHINX_QTHELP_OUTPUT "Build HTML pages with additional information for building a documentation collection in qthelp." OFF)
option(SPHINX_DEVHELP_OUTPUT "Build HTML pages with additional information for building a documentation collection in devhelp." OFF)
option(SPHINX_EPUB_OUTPUT "Build HTML pages with additional information for building a documentation collection in epub." OFF)
option(SPHINX_LATEX_OUTPUT "Build LaTeX sources that can be compiled to a PDF document using pdflatex." OFF)
option(SPHINX_MAN_OUTPUT "Build manual pages in groff format for UNIX systems." OFF)
option(SPHINX_TEXT_OUTPUT "Build plain text files." OFF)

mark_as_advanced(
        Sphinx_EXECUTABLE
        SPHINX_HTML_OUTPUT
        SPHINX_DIRHTML_OUTPUT
        SPHINX_HTMLHELP_OUTPUT
        SPHINX_QTHELP_OUTPUT
        SPHINX_DEVHELP_OUTPUT
        SPHINX_EPUB_OUTPUT
        SPHINX_LATEX_OUTPUT
        SPHINX_MAN_OUTPUT
        SPHINX_TEXT_OUTPUT)

function(_sphinx_add_target target_name builder)
    if(ARG_ALL)
        set(ARG_ALL ALL)
    else()
        set(ARG_ALL "")
    endif()
    add_custom_target(${target_name}
                      ${ARG_ALL}
                      COMMAND ${Sphinx_EXECUTABLE}
                      -b ${builder}
                      -c ${ARG_CONFIGURATION_DIR}
                      ${ARG_SOURCE_DIR}
                      "${ARG_OUTPUT_BASE_DIR}/${builder}"
                      VERBATIM
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMENT "Generating sphinx documentation: ${builder}")
    if(ARGN)
        add_dependencies(${target_name} ${ARG_DEPENDENCIES})
    endif()
endfunction()

function(sphinx_add_docs target_base_name #[[ALL]] #[[CONFIGURATION_DIR]] #[[SOURCE_DIR]] #[[OUTPUT_BASE_DIR]] #[[DEPENDENCIES]])
    set(options ALL)
    set(singleValues CONFIGURATION_DIR SOURCE_DIR OUTPUT_BASE_DIR)
    set(multiValues DEPENDENCIES)
    cmake_parse_arguments(PARSE_ARGV 1
                          "ARG"
                          "${options}"
                          "${singleValues}"
                          "${multiValues}")
    if(DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "No value given for" ${ARG_KEYWORDS_MISSING_VALUES})
    endif()

    if(SPHINX_HTML_OUTPUT)
        _sphinx_add_target("${target_base_name}_html" html)
    endif()
    if(SPHINX_DIRHTML_OUTPUT)
        _sphinx_add_target("${target_base_name}_dirhtml" dirhtml)
    endif()
    if(SPHINX_QTHELP_OUTPUT)
        _sphinx_add_target("${target_base_name}_qthelp" qthelp)
    endif()
    if(SPHINX_DEVHELP_OUTPUT)
        _sphinx_add_target("${target_base_name}_devhelp" devhelp)
    endif()
    if(SPHINX_EPUB_OUTPUT)
        _sphinx_add_target("${target_base_name}_epub" epub)
    endif()
    if(SPHINX_LATEX_OUTPUT)
        _sphinx_add_target("${target_base_name}_latex" latex)
    endif()
    if(SPHINX_MAN_OUTPUT)
        _sphinx_add_target("${target_base_name}_man" man)
    endif()
    if(SPHINX_TEXT_OUTPUT)
        _sphinx_add_target("${target_base_name}_text" text)
    endif()
endfunction()