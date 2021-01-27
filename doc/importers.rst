Import Modules
==============

*Import modules* are standalone libraries that provide additional :ref:`initializers <api/functions:initializers, releasers and reallocators>` capable of parsing third party file types. They can be enabled through the respective ``VisualT_<filetype>_IMPORTER`` CMake option. Import modules may have third party dependencies.

`Ancient versions of VisualT <https://github.com/Lucide/VisualT/releases/tag/1.0>`_ came with their own native Object editor. It was very awkward and inconvenient, so, during the 2.0 library rewrite, I decided to ditch it and adopt an already existing text-art editor. It was a challenging task. The major requirement was non-ascii characters support, which is one of the main features of VisualT.

In the end, I settled on REXPaint, but I wish it wasn't the only one. I'm keeping an eye on `lvllvl <https://www.lvllvl.com/>`_, for example. If you have suggestions, let me know.

At the moment, the only import module available is the REXPaint import module.

.. toctree::
    :maxdepth: 1

    importers/import-xp