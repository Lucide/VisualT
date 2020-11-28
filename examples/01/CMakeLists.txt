add_executable(example01 example01.c)

target_link_libraries(example01
        PRIVATE
        visualt
        getch)