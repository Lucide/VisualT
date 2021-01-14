include(utils)

add_executable(VisualT_tests "tests.c")
target_link_libraries(VisualT_tests PRIVATE
                      VisualT::library
                      VisualT::import_xp)
copy_shared_win(VisualT_tests
                VisualT::library
                VisualT::import_xp)
uses_assets(VisualT_tests)
