include(utils)
include(pjutils)

add_executable(VTExamples_car main.c)

set_target_properties(VTExamples_car PROPERTIES
                      OUTPUT_NAME car)

target_link_libraries(VTExamples_car
                      PRIVATE
                      VisualT::library
                      VTExamples_utilities)
copy_shared_win(VTExamples_car
                VisualT::library)

#pjutils_uses_assets(VTExamples_02)