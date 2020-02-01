add_executable(example02 main.c)
set_target_properties(example02
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(example02
        PRIVATE
        visualt
        getch)