include(utils)

add_executable(VisualT_tests "tests.c")
set_target_properties(VisualT_tests PROPERTIES
                      OUTPUT_NAME tests)
target_link_libraries(VisualT_tests PRIVATE VisualT::library)
copy_shared_win(VisualT_tests VisualT::library)
if(VisualT_XP_IMPORTER)
    target_compile_definitions(VisualT_tests PRIVATE VISUALT_IMPORT_XP)
    target_link_libraries(VisualT_tests PRIVATE VisualT::import_xp)
    copy_shared_win(VisualT_tests VisualT::import_xp)
endif()
uses_assets(VisualT_tests)
