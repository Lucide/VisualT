macro(relativify_path path)
    string(REGEX REPLACE [[^\.]] ${PROJECT_SOURCE_DIR} ${path} "${${path}}")
endmacro()

macro(set_if variable)
    if(${variable})
        set(${variable} ${ARGN})
    else()
        set(${variable} "")
    endif()
endmacro()

function(rebase_files output_var base_dir #[[FILES]])
    set(multiValues FILES)
    cmake_parse_arguments(PARSE_ARGV 2
                          "ARG"
                          ""
                          ""
                          "${multiValues}")
    foreach(file IN LISTS ARG_FILES)
        list(APPEND ${output_var} "${base_dir}/${file}")
    endforeach()
    set(${output_var} ${${output_var}} PARENT_SCOPE)
endfunction()

function(configure_files dest_dir #[[FILES]])
    set(multiValues FILES)
    cmake_parse_arguments(PARSE_ARGV 1
                          "ARG"
                          ""
                          ""
                          "${multiValues}")
    foreach(file IN LISTS ARG_FILES)
        if(file MATCHES [[(.+)\.in$]])
            configure_file(${file}
                           "${dest_dir}/${CMAKE_MATCH_1}"
                           @ONLY)
        endif()
    endforeach()
endfunction()

function(copy_shared_win target)
    if(WIN32)
        foreach(library IN LISTS ARGN)
            get_target_property(library_type ${library} TYPE)
            if(library_type STREQUAL "SHARED_LIBRARY")
                add_custom_command(TARGET ${target}
                                   POST_BUILD
                                   COMMAND ${CMAKE_COMMAND}
                                   -E copy_if_different "$<TARGET_FILE:${library}>" ${CMAKE_CURRENT_BINARY_DIR}
                                   COMMAND ${CMAKE_COMMAND}
                                   -E echo "${library} dll copied to \"${target}\"'s binary dir"
                                   VERBATIM)
                set_property(TARGET ${target}
                             APPEND
                             PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${library}>")
            endif()
        endforeach()
    endif()
endfunction()

function(uses_assets target)
    add_custom_command(TARGET ${target}
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND}
                       -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets"
                       COMMENT "assets are needed by ${target}"
                       VERBATIM)
    set_property(TARGET ${target}
                 APPEND
                 PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/assets")
endfunction()

function(install_symbolic #[[EXPORT]] #[[COMPONENT]] #[[DESTINATION]] #[[EXCLUDE_FROM_ALL]])
    set(options EXCLUDE_FROM_ALL)
    set(singleValues EXPORT COMPONENT DESTINATION)
    cmake_parse_arguments(PARSE_ARGV 0
                          "ARG"
                          "${options}"
                          "${singleValues}"
                          "")
    set_if(ARG_EXCLUDE_FROM_ALL EXCLUDE_FROM_ALL)
    string(JOIN "\n" install_script
           "set(export_name \"${ARG_EXPORT}\")"
           "set(export_dir \"${ARG_DESTINATION}\")"
           [[
set(install_base_path ${CMAKE_INSTALL_PREFIX})
set(export_path "${install_base_path}/${export_dir}/${export_name}.cmake")
message(STATUS "Installing: ${export_path}")
file(TO_CMAKE_PATH ${export_path} export_path)
file(WRITE ${export_path} "")
]])
    install(CODE "${install_script}"
            COMPONENT ${ARG_COMPONENT}
            ${ARG_EXCLUDE_FROM_ALL})
endfunction()
