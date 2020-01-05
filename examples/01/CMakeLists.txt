add_executable(example01 main.c)
target_compile_features(example01 PRIVATE c_std_99)

target_link_libraries(example01 PRIVATE visualt)