API Reference
#############

Before reading further, make sure you've understood the :ref:`basic-concepts:basic concepts`.

.. toctree::

    api/types
    api/functions

----

Implementation Details
**********************

.. warning:: VisualT is currently broken on big endian systems. The issue will be fixed in the following updates.

.. rubric:: Restrict qualifier

The ``restrict`` keyword basically means that the qualified pointer must be unique among the function parameters. In VisualT's API, its meaning is extended to *"the referenced base Object must be unique among the function parameters"*. That's to indicate that multiple pointers to instances created from a common base Object aren't allowed either. |n|
This in important to keep in mind when dealing with allocators and releasers, like :func:`vtRelease`. Instances are a more advanced "implicit feature" of VisualT, and should only be used by users familiar with C.

.. rubric:: Strict Aliasing

VisualT expects ``uint8_t`` to be a type compatible with ``char``, in order to respect the strict aliasing rule. If, in your implementation, ``uint8_t`` is not equivalent to ``char``, try to disable strict aliasing (`-fno-strict-aliasing <http://gcc.gnu.org/onlinedocs/gcc-4.6.1/gcc/Optimize-Options.html#index-fstrict_002daliasing-825) with GCC, unnecessary in windows>`_) and see what happens. If you spot erroneous behavior, create an issue and we'll look into it.

.. rubric:: Signedness

Variables in the source default to signed integers. Unsigned integers are used, when possible, to avoid unsigned->signed casts when dealing with unsigned public types. |n|
Public types, when reasonable, use unsignedness as a self-explanatory semantic indicator, like ``VTObj.length``, which supports ``>0`` values only. |n|
``VTObj.width``, instead, is also a ``>0`` type, but it's expressed by a signed type, because it's frequent used in calculations involving other signed types like **x** and **y** positions.
