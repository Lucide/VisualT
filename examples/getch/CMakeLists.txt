add_library(getch INTERFACE)
target_include_directories(getch
        INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(getch INTERFACE c_std_99)