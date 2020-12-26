include(utils)
include(pjutils)

add_executable(VTExamples_01 example01.c)

set_target_properties(VTExamples_01 PROPERTIES
                      OUTPUT_NAME example01)

link_and_copy(VTExamples_01)
target_link_libraries(VTExamples_01
                      PRIVATE
                      VTExamples_utilities)

uses_assets(VTExamples_01)