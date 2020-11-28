include(utils)

find_package(Doxygen REQUIRED)
set(VisualT_PYTHON_VENV "" CACHE PATH "If the python dependencies are installed in a virtual environment, set the path here.")
utils_relativify_path(VisualT_PYTHON_VENV)
list(APPEND CMAKE_PREFIX_PATH ${VisualT_PYTHON_VENV})
find_package(Sphinx REQUIRED)
list(POP_BACK CMAKE_PREFIX_PATH)
string(TIMESTAMP VisualT_BUILD_YEAR "%Y")
configure_file(
        conf.py.in
        "${CMAKE_CURRENT_BINARY_DIR}/conf.py"
        @ONLY)

set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)

set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C NO)
set(DOXYGEN_MARKDOWN_SUPPORT NO)
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_FILE_PATTERNS *.h)
list(APPEND DOXYGEN_ALIASES
     [["rst=@verbatim embed:rst:leading-asterisk"]] #begin rst block inside comment block
     [["rsti=@verbatim embed:rst:inline"]]          #begin inline rst text
     [["endrst=@endverbatim"]]                      #end rst block
     [["ref{2}=@rsti :\1:`\2` @endrst"]]            #crossreference
     [["ref{3}=@rsti :\1:`\3 <\2>` @endrst"]]       #named cross reference
     [["type{1}=@rsti :type:`\1` @endrst"]]         #reference to a typedef
     [["type{2}=@rsti :type:`\2 <\1>` @endrst"]]    #named reference to a typedef
     [["func{1}=@rsti :func:`\1()` @endrst"]]       #reference to a function
     [["func{2}=@rsti :func:`\2 <\1()>` @endrst"]]  #named reference to a function
     [["enum{1}=@rsti :enum:`\1` @endrst"]]         #reference to an enum
     [["enum{2}=@rsti :enum:`\2 <\1>` @endrst"]]    #named reference to an enum
     [["macro{1}=@rsti :macro:`\1` @endrst"]]       #reference to a macro
     [["macro{2}=@rsti :macro:`\2 <\1>` @endrst"]]  #named reference to a macro

     [["b{1}=<b>\1</b>"]]                           #bold
     [["e{1}=<em>\1</em>"]]                         #italic
     [["c{1}=<tt>\1</tt>"]])                        #code

#=============================
# Define targets
#=============================
doxygen_add_docs(VisualT_doxygen_generate_xml
                 "${VisualT_SOURCE_DIR}/include/visualt/visualt.h")

sphinx_add_docs(VisualT_sphinx_generate
                SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                CONFIGURATION_DIR ${CMAKE_CURRENT_BINARY_DIR}
                OUTPUT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(VisualT_sphinx_generate_html VisualT_doxygen_generate_xml)

add_custom_target(VisualT_documentation ALL)
add_dependencies(VisualT_documentation VisualT_sphinx_generate_html)
#=============================
# Packaging
#=============================