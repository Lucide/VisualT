add_executable(VisualT_pen_demo pen-demo.c)

set_target_properties(VisualT_pen_demo PROPERTIES
                      OUTPUT_NAME pen-demo
                      )

#target_compile_definitions(VisualT_pen_demo
#                           PRIVATE _USE_MATH_DEFINES)

target_link_libraries(VisualT_pen_demo
                      PRIVATE
                      VisualT_library m)