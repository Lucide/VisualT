set(cwd "${CMAKE_CURRENT_LIST_DIR}/../..")

file(COPY "${cwd}/doc/" DESTINATION ${CMAKE_CURRENT_LIST_DIR}
     PATTERN "readthedocs" EXCLUDE)

execute_process(COMMAND ${CMAKE_COMMAND} -D VisualT_ASSUME_LITTLE_ENDIAN:BOOL=YES -S . -B build
                COMMAND ${CMAKE_COMMAND} --build build --target VisualT_doxygen_generate_xml
                WORKING_DIRECTORY ${cwd}
                COMMAND_ECHO STDOUT)