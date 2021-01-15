include(utils)

#=============================
# Configuration
#=============================
set(src_source_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(src_binary_dir ${CMAKE_CURRENT_BINARY_DIR})
set(public_headers_dir "${PROJECT_SOURCE_DIR}/include")

#=============================
# Configure Time Generation
#=============================
configure_files(${src_binary_dir}
                FILES
                "version.h.in")

#=============================
# Targets Configuration
#=============================
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options("$<$<CONFIG:Debug>:-O2;-Wconversion;-Wall;-Wextra;-pedantic>")
    endif()
endif()

#=============================
# Subdirectories
#=============================
add_subdirectory(visualt)
if(VisualT_XP_IMPORTER)
    add_subdirectory(import-xp)
endif()



