add_executable(example01 main.c)
target_compile_features(example01 PRIVATE c_std_99)
set_target_properties(example01
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(example01
        PRIVATE
        visualt
        getch)