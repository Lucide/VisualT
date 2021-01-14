include(utils)
include(pjutils)

add_executable(VTExamples_demo main.c)

set_target_properties(VTExamples_demo PROPERTIES
                      OUTPUT_NAME demo)

target_link_libraries(VTExamples_demo
                      PRIVATE
                      VisualT::library
                      VTExamples_utilities)
copy_shared_win(VTExamples_demo
                VisualT::library)

uses_assets(VTExamples_demo)