include(utils)

#=============================
# Configuration
#=============================
set(private_headers_dirs ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set(public_headers_dirs "${PROJECT_SOURCE_DIR}/include")

#=============================
# Configure Time Generation
#=============================
string(TIMESTAMP build_date "%d %B %Y %H:%M" UTC)
list(GET private_headers_dirs 1 dir)
configure_files(${dir}
                FILES
                "version.h.in"
                "buildDate.h.in")

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



