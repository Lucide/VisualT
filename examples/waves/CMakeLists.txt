include(utils)

add_executable(VTExamples_waves main.c)

set_target_properties(VTExamples_waves PROPERTIES
                      OUTPUT_NAME waves)

target_link_libraries(VTExamples_waves
                      PRIVATE
                      VisualT::library
                      VTExamples_utilities
                      "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")
copy_shared_win(VTExamples_waves
                VisualT::library)


