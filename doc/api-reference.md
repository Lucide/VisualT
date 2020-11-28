# API Reference

Before reading these, make sure you've understood the [basic concepts](basic-concepts).

```eval_rst
.. toctree::

    api/types
    api/functions
```

---

## Implementation Details

### Strict Aliasing

VisualT expects ``uint8_t`` to be a type compatible with ``char``, in order to respect the strict aliasing rule. If, in your implementation, ``uint8_t`` is not equivalent to ``char``, try to disable strict aliasing ([`-fno-strict-aliasing`](http://gcc.gnu.org/onlinedocs/gcc-4.6.1/gcc/Optimize-Options.html#index-fstrict_002daliasing-825) with GCC, unnecessary in windows) and see what happens. If you spot erroneous behavior, create an issue and we'll look into it.

### Signedness

Variables in the source default to signed integers. Unsigned integers are used, when possible, to avoid unsigned->signed casts when dealing with unsigned public types.\
Public types, when reasonable, use unsignedness as a self-explanatory semantic indicator, like `VTObj.length`, which supports `>0` values only.\
`VTObj.width`, instead, is also a `>0` type, but it's expressed by a signed type, because it's frequent used in calculations involving other signed types like **x** and **y** positions.
