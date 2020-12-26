add_library(VTExamples_utilities INTERFACE)

target_include_directories(VTExamples_utilities
                           INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(VTExamples_utilities
               INTERFACE
               "${CMAKE_CURRENT_SOURCE_DIR}/$<IF:$<C_COMPILER_ID:MSVC>,msvcUtils.c,posixUtils.c>")
