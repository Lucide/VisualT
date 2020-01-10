add_executable(tests tests.c)
target_compile_features(tests PRIVATE c_std_99)
target_link_libraries(tests
        PRIVATE
        visualt)