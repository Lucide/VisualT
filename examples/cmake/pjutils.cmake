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