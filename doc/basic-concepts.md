# Basic Concepts

Here you will learn the terminology and key concepts of VisualT, with as few code references as possible. This is a very recommended read, before diving into any other section.

```eval_rst
.. tip:: If you're familiar with `Scratch <https://scratch.mit.edu/>`_, you're in luck! VisualT was modelled similarly, graphics-wise.
```

## Objects (obj)

The only top-level graphic element is the **Object**, which contains one or more [Sprites](#sprites). An Object can display one and only one Sprite at a time, called the **active Sprite**.

When Objects are passed to a function, their respective active Sprite determines which Sprite will be used. From now on, when we refer to the Sprite of an Object, we mean the active Sprite.

The attributes of an Object are:

* a pointer to the active Sprite
* a visibility flag
* its **x** and **y** position
* its pen size

``` image:: images/object.png
    :align: center
    :alt: Object structure
```

The *visibility flag* indicates if the Object will be drawn (`true`) or skipped (`false`), when passed to drawing functions.

The *pen size* regulates the pen behavior. More on [The Pen](#the-pen).

``` tip:: Objects behave similarly to a *Sprite* in Scratch.
```

## Sprites

Sprites are the text equivalent of a bitmap image, instead of pixels, you have *cells*, where every cell contains a glyph. An Object's active Sprite determines its appearance and size. Sprites are always drawn centered with respect to the Object's position.

``` image:: images/sprite.png
    :align: center
    :alt: Sprite structure
```

Sprites can have transparent cells: a value of 0 (or `\v`, in strings) is interpreted as "no-glyph". When a Sprite is drawn, you'll be able to see the background under transparent areas.

``` tip:: Sprites are equivalent to a *Costume* in Scratch.
```

## The Canvas

Objects can be composed and drawn onto another Object that acts as a canvas.\
So, given two Objects, `a` and `b`, you can either:

* draw `a` on `b`
* draw `b` on `a`

Every Object can act either as a drawable element, or as a drawable surface.

Position [0,0] is placed at the center of the stage, so the maximum values of the **x** and **y** coordinates are respectively half its width and half its height (one half will be one unit greater, if the total is odd).

That does not mean that larger values can't be used: what exceeds the visible area of the canvas simply won't be visible.

``` tip:: The canvas behaves equivalently to the *Stage* in Scratch.
```

## The Pen

Objects come equipped with a toggleable *pen* that leaves a mark on the canvas when the Object is moving. It's the classic tool that allows you to draw vector graphics!

You can choose between six stroke sizes:

``` figure:: images/pen-smaller.png
    :align: center
    :alt: pen strokes

    stroke stencils are wider than tall, to compensate for non-square glyphs
```

You can use the pen to plot functions, or parametric wireframe stuff. It works even when the drawing Object is hidden.

``` figure:: images/pen-demo.gif
    :align: center
    :alt: pen demo

    the drawing Object is hidden
```
