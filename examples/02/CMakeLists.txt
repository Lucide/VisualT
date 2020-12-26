include(utils)
include(pjutils)

add_executable(VTExamples_02 example02.c)

set_target_properties(VTExamples_02 PROPERTIES
                      OUTPUT_NAME example02)

link_and_copy(VTExamples_02)
target_link_libraries(VTExamples_02
                      PRIVATE
                      VTExamples_utilities)

#pjutils_uses_assets(VTExamples_02)