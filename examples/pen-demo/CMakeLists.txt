include(utils)

add_executable(VTExamples_pen_demo pen-demo.c)

set_target_properties(VTExamples_pen_demo PROPERTIES
                      OUTPUT_NAME pen-demo)

link_and_copy(VTExamples_pen_demo)
target_link_libraries(VTExamples_pen_demo
                      PRIVATE
                      VTExamples_utilities
                      "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")


