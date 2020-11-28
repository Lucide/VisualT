add_executable(example02 example02.c)

target_link_libraries(example02
        PRIVATE
        visualt
        getch)