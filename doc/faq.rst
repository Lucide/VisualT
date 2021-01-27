Frequently Asked Questions
##########################

Why such a high minimum CMake version?
======================================

Because CMake is **not** a compiler! In fact, it's designed from the ground up to be updated safely through a sophisticated `policies system <https://cmake.org/cmake/help/v3.17/manual/cmake-policies.7.html>`_. Unfortunately, that's a little known fact, and most people stick to ancient version and hurt themselves. |n|
`Update your CMake! <https://cmake.org/download/#latest>`_

.. tip:: If you're using Ubuntu, Kitware provides an `APT repository <https://apt.kitware.com/>`_ that you can add to your system.

Oh come on, why no colors?
==========================

Colors are cool, but since every terminal does colors differently, VisualT cannot reasonably support them all. If you *need* colors, then NCurses might be a better option. On Windows, things might be even harder.

Some characters are broken on Windows?
======================================

Windows is an ancient OS, a lot of decisions were made before the creation of common standards. The default character encoding is a mixture of UCS-2 and UTF-16. VisualT uses UTF-8, like most of the world today. There's this `Stack Overflow answer <https://stackoverflow.com/a/57134096/3734108>`_ that explains well the procedure to enable UTF-8 and the potential side effects. Remember to use a unicode capable font!

Some Objects are not drawn, despite being marked as visible, I checked!
=======================================================================

That happens often enough that deserves its own faq entry: check the length parameter in the function calls. Probably it's just C being C 🙂.

Who is simba?
=============

