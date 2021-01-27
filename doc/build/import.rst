How do you use a CMake library?
###############################

VisualT is a CMake-first library, so it natively supports the most common ways of adding a CMake dependency to a project. If you are just a casual CMake user, this page will offer you some guidance, but if you're completely new, reading further will likely lead to confusion and frustration. In the examples we'll build both the library and the .xp importer.

.. tip:: If you're interested in learning CMake in an humane way, I can't recommend Craig Scott's `Professional CMake: A Practical Guide <https://crascit.com/professional-cmake/>`_ enough. It's a tad expensive, but for a good reason: once bought, you'll get every future edition for free! That's a very important feature, since CMake updates frequently. |n|
    You can also hang around the `CMake discourse forum <https://discourse.cmake.org/>`_.

Approach 1: install locally
***************************

VisualT can be installed on your system and automatically found by CMake. Open a terminal inside the project root directory and follow these steps (valid for most CMake projects):

#.  **Configure:**

    Here you'll be able to change VisualT's options, if required. Remember to specify the type to avoid unexpected behaviors. On Windows, you may need to run the command within a developer command prompt for certain generators. You can find a list of available options :ref:`here <build/specs:options>`.

    .. code-block:: none

        cmake -D VisualT_XP_IMPORTER:BOOL=YES -S . -B ./build

#.  **Build:**

    Eventually, you can list the targets you want to build in the ``--target`` option. You can find a list of available targets :ref:`here <build/specs:targets>`.


    .. code-block:: none

        cmake --build ./build

#.  **Install:**

    Installed files will be listed on the terminal. You can install only components whose targets have been built. You can find a list of available components :ref:`here <build/specs:install components>`.

    .. code-block:: none

        cmake --install ./build

#.  **Use**:

    If you didn't change the install destination, the library will be found automatically:

    .. code-block:: CMake
        :linenos:

        cmake_minimum_required(VERSION 3.17)

        project(myProject
                LANGUAGES C)

        set(CMAKE_C_STANDARD 99)
        set(CMAKE_C_STANDARD_REQUIRED YES)

        find_package(VisualT
                     REQUIRED
                     COMPONENTS
                     VisualT_development
                     VisualT_import_xp)

        add_executable(myCliApp "source.c")
        target_link_libraries(myCliApp PRIVATE
                            VisualT::library
                            VisualT::import_xp)

Approach 2: using the Hunter package manager
********************************************

.. todo:: After the first stable release

Approach 3: include the project as a subdirectory
*************************************************

This is probably the most straightforward way to add local dependencies to a CMake project, but it's rarely a viable approach, as it requires **all** target names to be unique across the whole project. |n|
In VisualT, every internal target is prefixed (*namespaced*) with ``VisualT_``, and every exported target with ``VisualT::``. On top of that, for every linkable target, there's an alias named as its exported name (e.g. ``VisualT::library`` alias for ``VisualT_library``). **The user should link to target aliases only**: that's a good CMake practice that guarantees a certain level of compatibility when switching between internal and exported targets.

.. code-block:: CMake
    :linenos:

    cmake_minimum_required(VERSION 3.17)

    project(myProject
            LANGUAGES C)

    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED YES)

    # set visualt options (facoltative)
    option(VisualT_XP_IMPORTER "Enable the .xp importer library." YES)

    add_subdirectory(VisualT)

    add_executable(myCliApp "source.c")
    target_link_libraries(myCliApp PRIVATE
                          VisualT::library
                          VisualT::import_xp)

Add automatic download
======================

You can use the built-in `FetchContent <https://cmake.org/cmake/help/v3.17/module/FetchContent.html>`_ CMake module to automate the "download and add as subdirectory" process. The download is performed at configure time. Replace the previous ``add_subdirectory(VisualT)`` with:

.. code-block:: CMake
    :linenos:

    include(FetchContent)

    fetchcontent_declare(VisualT
                         GIT_REPOSITORY https://github.com/Lucide/VisualT.git
                         GIT_TAG r2.0.0)
    FetchContent_MakeAvailable(VisualT)

Approach 4: external project
****************************

`ExternalProject <https://cmake.org/cmake/help/v3.17/module/ExternalProject.html>`_ is the older method to add dependencies to a project. It's much more generic and versatile than the previous approaches, but this makes it rather difficult to use, with a lot of gotchas. Anyone confident enough with CMake to use this approach needs no further explanation.