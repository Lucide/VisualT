add_executable(example01 example01.c)
set_target_properties(example01
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(example01
        PRIVATE
        visualt
        getch)