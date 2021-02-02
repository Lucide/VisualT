The REXPaint import module
##########################

.. image:: /images/rexpaint-logo.gif
    :align: center
    :alt: REXPaint logo

The REXPaint import module can initialize an Object from one or many .xp files. REXPaint is a popular text-art editor for Windows, available for free on `gridsagegames.com <https://www.gridsagegames.com/rexpaint/>`_. (Should run fine on Wine)

When enabled, an additional zlib dependency will be added to the project. The download and build will be performed automatically by the `Hunter package manager <https://github.com/cpp-pm/hunter>`_. A pre-existing distribution of zlib will probably not be suitable, as the module requires some custom build parameters.

.. tip:: If you find the plaintext manual a bit hard to read, I'm maintaining an `unofficial markdown edition <https://github.com/Lucide/REXPaint-manual/blob/master/manual.md>`_. Contributions welcome!

The REXPaint+VisualT workflow
*****************************

REXPaint needs some configuration in order to reach an optimum level of productivity. We'll set everything up with the help of the `Font Atlas Generator <https://lucide.github.io/font-atlas-generator/>`_ webapp, which I wrote for the purpose.

1. Create a new font atlas
==========================

* Choose a reference font to design your Sprites. You could use the default font of your System's terminal, or a readily available one. The generator uses *DejaVu Sans Mono* as default, which is the default font of the Ubuntu terminal. The only requirement is that it must be a **monospaced font**.
* On the generator app, define the **charset** (*glyph palette*) you want to use. Avoid making the character set too large or it'll become awkward to use. The font atlas must be **16 columns wide**. The generator creates square cells by default. While that can be appropriate for bitmap artworks, you'll notice that everything looks squashed horizontally once exported to actual text (just see what happens to the VTCat in the examples!). That's because most font's glyphs are **taller than wide**. Try to find a compromise between fidelity and usability.
* Click on the *save image* and *export charset for REXPaint* buttons to save the output.

2. Add the font to REXPaint (also explained in the REXPaint manual)
===================================================================

* Move to REXPaint's root dir.
* Move the two downloaded files to ``data\fonts``.
* Open ``_config.xt`` with a text editor, the file contains a two-sided text table: ``GUI`` on the left and ``Art`` on the right. We'll operate on the ``Art`` **side**. Add a new row by copy-pasting the ``CP437 16x16`` line, and change the ``Set Name`` field as desired. Replace the contents of the ``File`` field with your atlas' file name, and update the ``Columns`` and ``Rows`` values.
* If you want, you can also replace the ``Unicode`` field with the file name of your charset file. Since REXPaint is a bitmap editor, it does not store any associations between font tiles and their corresponding character, not even in the .xp files. You can provide that data yourself by specifying a *charset file*. That's also the reason why VisualT requires a charset file to import .xp files.

CMake specs
***********

+--------------+-----------------------------+
| conditions   | :term:`VisualT_XP_IMPORTER` |
+--------------+-----------------------------+
| targets      | VisualT_xp_importer         |
+--------------+-----------------------------+
| components   | VisualT_xp_importer         |
+--------------+-----------------------------+
| dependencies | VisualT_runtime             |
+--------------+-----------------------------+

See more on :doc:`/build`.

API
***

.. doxygenenum:: VTXpLoadMode

.. doxygenfunction:: vtInitializeXp