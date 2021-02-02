list(APPEND CMAKE_MODULE_PATH "${VisualT_SOURCE_DIR}/cmake")
set(install_dir "${CMAKE_INSTALL_DATADIR}/VTExamples")
set_property(DIRECTORY
             PROPERTY EXCLUDE_FROM_ALL YES)

#=============================
# Installation
#=============================
install(DIRECTORY
        "${PROJECT_SOURCE_DIR}/"
        "${VisualT_SOURCE_DIR}/cmake"
        DESTINATION ${install_dir}
        COMPONENT VisualT_examples
        EXCLUDE_FROM_ALL
        PATTERN "FindSphinx.cmake" EXCLUDE)
string(JOIN "\n" install_script
       "set(install_dir \"${install_dir}\")"
       "set(module_path \"${CMAKE_CURRENT_LIST_DIR}/installed.cmake.in\")"
       [[
set(install_base_path ${CMAKE_INSTALL_PREFIX})
set(configured_module_path "${install_base_path}/${install_dir}/cmake/context-specific.cmake")
message(STATUS "Installing: ${configured_module_path}")
file(TO_CMAKE_PATH ${install_base_path} install_base_path)
message(STATUS "VisualT: installed examples will include \"${install_base_path}\" as a package search path")
configure_file(${module_path}
               ${configured_module_path}
               @ONLY)
]])
install(CODE "${install_script}"
        COMPONENT VisualT_examples
        EXCLUDE_FROM_ALL)
install_symbolic(EXPORT VisualT_examples
                 COMPONENT VisualT_examples
                 DESTINATION ${VisualT_export_dir}
                 EXCLUDE_FROM_ALL)
