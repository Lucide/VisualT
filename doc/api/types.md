# Data Types

These types are designed to make the API parameters consistent and easily recognizable. They come already equipped with `const` qualifiers, so they may not necessarily be the best option for representing data in your program, if you plan on doing more advanced manipulations. The only requirement is that the data structures are compatible, so that casts become essentially equivalent to "const cast" (pay attention to type compatibility, if you care about strict aliasing).

VisualT's public types are all prefixed with "VT", to avoid potential collision.

``` doxygentypedef:: VTObj
```

``` doxygentypedef:: VTChar
```

``` doxygentypedef:: VTStr
```

``` doxygentypedef:: VTStrs
```

``` doxygentypedef:: VTSizes
```

``` doxygentypedef:: VTObjs
```

``` doxygenenum:: VTAlign
```

## Literal Helper Macros

You can use the following macros to express **literals** more easily. They are not meant to cast already existing data; use normal casts if you need to do that.

When a literal helper macro can be used on a parameter, it will be indicated in parentheses. See the linked examples.

``` doxygendefine:: LTSTR
```

``` doxygendefine:: LTSTRS
```

``` doxygendefine:: LTSIZES
```

``` doxygendefine:: LTOBJS
```

```eval_rst
.. note:: There's no macro to create a :type:`VTChar` literally, because that would break the strict aliasing rule. You can use :func:`vtChar()` instead.
```
