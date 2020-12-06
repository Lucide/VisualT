macro(utils_relativify_path path)
    string(REGEX REPLACE [[^\.]] ${PROJECT_SOURCE_DIR} ${path} "${${path}}")
endmacro()

function(utils_intlink_library target)
    target_link_libraries(${target}
                          PRIVATE VisualT_library)
    if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        get_target_property(library_type VisualT_library TYPE)
        if(library_type STREQUAL "SHARED_LIBRARY")
            add_custom_command(TARGET ${target}
                               POST_BUILD
                               COMMAND ${CMAKE_COMMAND}
                               -E copy_if_different "$<TARGET_FILE:VisualT_library>" ${CMAKE_CURRENT_BINARY_DIR}
                               COMMENT "built dll is needed by ${target}"
                               VERBATIM)
            set_property(TARGET ${target}
                         APPEND
                         PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:VisualT_library>")
        endif()
    endif()
endfunction()