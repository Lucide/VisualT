if(${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
    set(${CMAKE_FIND_PACKAGE_NAME}_comps ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS})
    _component_requires(VisualT_examples
                        VisualT_library)
else()
    # No components given, only the library is required
    set(${CMAKE_FIND_PACKAGE_NAME}_comps VisualT_library)
endif()

# Find external dependencies, storing comps in a safer variable name.
# In this example, BagOfBeans is required by the mandatory Runtime component.
#set(${CMAKE_FIND_PACKAGE_NAME}_comps ${comps})
#include(CMakeFindDependencyMacro)
#find_dependency(BagOfBeans)

# Check all required components are available before trying to load any
foreach(comp IN LISTS ${CMAKE_FIND_PACKAGE_NAME}_comps)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED_${comp} AND
       NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/VisualT_${comp}.cmake)
        set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
            "VisualT: missing required component: ${comp}")
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
        return()
    endif()
endforeach()
foreach(comp IN LISTS ${CMAKE_FIND_PACKAGE_NAME}_comps)
    # All required components are known to exist. The OPTIONAL keyword
    # allows the non-required components to be missing without error.
    include(${CMAKE_CURRENT_LIST_DIR}/VisualT_${comp}.cmake OPTIONAL)
endforeach()

macro(_component_requires component_name)
    if("${component_name}" IN_LIST ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
        list(APPEND ${CMAKE_FIND_PACKAGE_NAME}_comps ${ARGN})
        list(REMOVE_DUPLICATES ${CMAKE_FIND_PACKAGE_NAME}_comps)
    endif()
endmacro()