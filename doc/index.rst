Welcome!
########

.. toctree::
    :hidden:
    :caption: VISUALT DOCUMENTATION

    self
    basic-concepts
    examples
    importers
    build
    api
    faq

VisualT is a cross-platform C library that makes creating UTF8 text-based UIs easier and faster.

For a quick overview of the main features, check out the :doc:`basic-concepts` page.

If you appreciated the work, leave a star on the `GitHub repo <https://github.com/Lucide/VisualT>`_.

.. rubric:: Highlights

* Has no dependencies other than standard libraries.
* It works wherever UTF-8 encoded text can be displayed properly, and that includes even Windows 10! See on the :ref:`faq <faq:some characters are broken on windows?>`.
* Makes no assumptions about the output, it just outputs text.
* It's *much* more lightweight than more feature-packed libraries like NCurses.
* Good CMake code is one of the main objectives.


.. note:: The documentation is still work in progress, but the main sections are content complete. That includes:

    * :doc:`The api documentation <api>`
    * :doc:`The CMake specs and build instructions <build>`
    * :doc:`Frequently Asked Questions <faq>`

----

.. rubric:: VisualT's dependencies are managed by the `Hunter Package Manager <https://github.com/cpp-pm/hunter>`_:

Hunter is a CMake powered package manager, which means it's completely *written* in CMake! Other than being inherently cross-patform, it offers a CMake-friendly interface for every package, which is a precious quality. It is also committed to complying with modern CMake best practices. If you're starting a new project, give it a try, and spread the word!