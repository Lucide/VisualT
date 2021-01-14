macro(_component_requires component_name)
    if("${component_name}" IN_LIST ${CMAKE_FIND_PACKAGE_NAME}_comps)
        list(APPEND ${CMAKE_FIND_PACKAGE_NAME}_comps ${ARGN})
        list(REMOVE_DUPLICATES ${CMAKE_FIND_PACKAGE_NAME}_comps)
    endif()
endmacro()

if(${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
    set(${CMAKE_FIND_PACKAGE_NAME}_comps ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS})
    _component_requires(VisualT_examples
                        VisualT_development)
    _component_requires(VisualT_import_xp
                        VisualT_runtime)
    _component_requires(VisualT_development
                        VisualT_runtime)
else()
    # No components given, only the library is required
    set(${CMAKE_FIND_PACKAGE_NAME}_comps VisualT_runtime)
endif()

# Check all required components are available before trying to load any
foreach(comp IN LISTS ${CMAKE_FIND_PACKAGE_NAME}_comps)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED_${comp}
       AND NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${comp}.cmake")
        set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
            "VisualT: The \"${comp}\" component is required but hasn't been installed")
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
        return()
    endif()
endforeach()
foreach(comp IN LISTS ${CMAKE_FIND_PACKAGE_NAME}_comps)
    # All required components are known to exist. The OPTIONAL keyword
    # allows the non-required components to be missing without error.
    include(${CMAKE_CURRENT_LIST_DIR}/${comp}.cmake OPTIONAL)
endforeach()