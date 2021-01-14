add_library(VTExamples_utilities INTERFACE)

target_include_directories(VTExamples_utilities
                           INTERFACE
                           "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(WIN32)
    set(source "winUtils.c")
else()
    set(source "posixUtils.c")
endif()
target_sources(VTExamples_utilities
               INTERFACE
               "${CMAKE_CURRENT_SOURCE_DIR}/${source}")