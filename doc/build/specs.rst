CMake specs
###########

+-----------------------+--------------------------------------------+
| default build type    | Release                                    |
+-----------------------+--------------------------------------------+
| supported build types | Release, Debug, RelWithDebInfo, MinSizeRel |
+-----------------------+--------------------------------------------+
| *ALL* targets         | VisualT_library, VisualT_import_xp [1]_    |
+-----------------------+--------------------------------------------+
| default installation  | VisualT::library, VisualT::import_xp [1]_  |
+-----------------------+--------------------------------------------+

.. [1] If the respective conditions are met.

.. note:: Hunter packages are built at configure time. When using a multi-config generator, they will default to Release.

Options
*******

.. glossary::

    VisualT_BUILD_SHARED_LIBS
        type: ``BOOL`` |n|
        default: ``NO`` |n|
        Build all libraries (VisualT and modules) as shared libraries. A more specific alternative to :term:`BUILD_SHARED_LIBS`, otherwise completely equivalent.

    VisualT_ASSUME_LITTLE_ENDIAN
        type: ``BOOL`` |n|
        default ``NO`` |n|
        Skip CMake endianness detection (which can take a bit long), and assume a little endian system, like all x86/x86-64 cpus.

    VisualT_PYTHON_VENV
        type: ``PATH`` |n|
        default: ``""`` |n|
        Path to a python venv where Sphinx is installed. If your python packages are installed under an unconventional path, use this option to let CMake find them.

    VisualT_XP_IMPORTER
        type: ``BOOL`` |n|
        default: ``NO`` |n|
        Enable the .xp file import module, and adds a zlib dependency that will be downloaded through Hunter.

.. rubric:: Common CMake options

.. glossary::

    `BUILD_SHARED_LIBS <https://cmake.org/cmake/help/v3.17/variable/BUILD_SHARED_LIBS.html>`_
        The standard way to select the type of libraries.

    `CMAKE_POSITION_INDEPENDENT_CODE <https://cmake.org/cmake/help/v3.17/variable/CMAKE_POSITION_INDEPENDENT_CODE.html>`_
        This is useful when you want to link a static VisualT to a shared library (instead of an executable). When building static libraries, the export of symbols is automatically disabled, to prevent them from "bleeding out" when linked to shared libraries.

Targets
*******

A list of all targets that can be defined by VisualT. If specified, the target is defined only if the respective *condition* is met.

+------------------------------+-----------------------------+----------------------------------------------------------------------------+
|            target            |         conditions          |                                description                                 |
+==============================+=============================+============================================================================+
| VisualT_library              | none                        | The VisualT library.                                                       |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VisualT_tests                | none                        | Some visual feedback tests for VisualT. Currently not very indicative.     |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VisualT_xp_importer          | :term:`VisualT_XP_IMPORTER` | The .xp file import module.                                                |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VisualT_doxygen_generate_xml | root project, sphinx found  | Step 1/2 for generating documentation.                                     |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VisualT_sphinx_generate_html | root project, sphinx found  | Step 2/2 for generating documentation.                                     |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VTExamples_demo              | root project                | An example showcasing a variety of VisualT's features, starring the VTCat. |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VTExamples_car               | root project                | An example of a proof of concept game with non-blocking input.             |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+
| VTExamples_waves             | root project                | An example showcasing the pen feature.                                     |
+------------------------------+-----------------------------+----------------------------------------------------------------------------+

Install Components
******************

A list of all install components that can be defined by VisualT. If specified, the component is defined only if the respective *condition* is met. Each component is paired with an export set with the same name. Component dependencies are checked by the config package file.

+-----------------------+------------------------------------------------------------+---------------------+-----------------------------+-----------------------------------------------------------+
|       component       |                          targets                           |    dependencies     |         conditions          |                        description                        |
+=======================+============================================================+=====================+=============================+===========================================================+
| VisualT_runtime       | VisualT_library                                            | none                | none                        | The VisualT library binaries.                             |
+-----------------------+------------------------------------------------------------+---------------------+-----------------------------+-----------------------------------------------------------+
| VisualT_development   | VisualT_library                                            | VisualT_runtime     | none                        | The VisualT library public header.                        |
+-----------------------+------------------------------------------------------------+---------------------+-----------------------------+-----------------------------------------------------------+
| VisualT_xp_importer   | VisualT_xp_importer                                        | VisualT_runtime     | :term:`VisualT_XP_IMPORTER` | The .xp file importer module binaries and public headers. |
+-----------------------+------------------------------------------------------------+---------------------+-----------------------------+-----------------------------------------------------------+
| VisualT_documentation | VisualT_doxygen_generate_xml, VisualT_sphinx_generate_html | none                | root project, sphinx found  | The html documentation.                                   |
+-----------------------+------------------------------------------------------------+---------------------+-----------------------------+-----------------------------------------------------------+
| VisualT_examples      | VTExamples_demo, VTExamples_car, VTExamples_waves          | VisualT_development | root project                | The examples, as a standalone CMake project.              |
+-----------------------+------------------------------------------------------------+---------------------+-----------------------------+-----------------------------------------------------------+
