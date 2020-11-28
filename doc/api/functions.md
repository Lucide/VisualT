# Functions

As with types, function names are prefixed with "vt", to avoid collisions.

```eval_rst
.. note:: - Unless explicitly stated (e.g. positioning functions), ``NULL`` is not an expected input.
             - No function expects uninitialized or dangling pointers as input.
             - Sizes and lengths must always be greater than 0.
```

## Miscellanous

``` doxygenfunction:: vtAbout
```

``` doxygenfunction:: vtChar
```

## Initializers, Releasers and Reallocators

Initializer functions modify or initialize Objects. Be careful to not re-initialize Objects without releasing them before, or you'll cause memory leaks.

They are called "initializers" (and not allocators) for a reason: the Object structure is not allocated, it's just initialized. You are free to declare an Object the way you want, like:

```c
VTObj obj;
vtInitializeString(&obj, 1, LTSTRS{"An Object in the stack"});
```

or:

```c
VTObj *obj = malloc(sizeof(VTObj));
vtInitializeString(obj, 1, LTSTRS{"An Object in the heap"});
```

Initializers always set the Object's attributes to the following values:

```eval_rst
+-----------------+----------------+
|    attribute    | default value  |
+=================+================+
| active Sprite   | Sprite index 0 |
+-----------------+----------------+
| visibility flag |    ``true``    |
+-----------------+----------------+
| position        |     [0,0]      |
+-----------------+----------------+
| pen size        |       0        |
+-----------------+----------------+
```

``` note:: Reallocators, like *initializers*, perform both memory deallocation and allocation to change the structure of an Object. They might seem convenient in several situations, but be aware of the potential performance impact of using them too often (irrelevant if confronted with I/O times).
```

``` doxygenfunction:: vtInitializeBlank
```

``` doxygenfunction:: vtInitializeArray
```

``` doxygenfunction:: vtInitializeFile
```

``` doxygenfunction:: vtInitializeString
```

``` doxygenfunction:: vtInitializeObj
```

``` doxygenfunction:: vtRelease
```

``` doxygenfunction:: vtCloneSprite
```

``` doxygenfunction:: vtResize
```

## Drawing

``` doxygenfunction:: vtRender
```

``` doxygenfunction:: vtStamp
```

``` doxygenfunction:: vtPrint
```

``` doxygenfunction:: vtPrintToString
```

``` doxygenfunction:: vtPrintStringSize
```

## Sprite Operations

``` doxygenfunction:: vtSprites
```

``` doxygenfunction:: vtSpriteInd
```

``` doxygenfunction:: vtNextSprite
```

``` doxygenfunction:: vtPrecSprite
```

``` doxygenfunction:: vtSetSprite
```

``` doxygenfunction:: vtWidth
```

``` doxygenfunction:: vtHeight
```

``` doxygenfunction:: vtSetText
```

``` doxygenfunction:: vtClear
```

``` doxygenfunction:: vtFill
```

``` doxygenfunction:: vtOverlay
```

``` doxygenfunction:: vtPrintAxes
```

## Generic Object Operations

``` doxygenfunction:: vtVisible
```

``` doxygenfunction:: vtShow
```

``` doxygenfunction:: vtHide
```

``` doxygenfunction:: vtSetVisibility
```

``` doxygenfunction:: vtSerialize
```

``` doxygenfunction:: vtSerializedArraySize
```

## Pen Operations

``` doxygenfunction:: vtPenGlyph
```

``` doxygenfunction:: vtPenSize
```

``` doxygenfunction:: vtSetPenSize
```

``` doxygenfunction:: vtSetPenGlyph
```

## Movement

Movement functions move the Objects in the scene by manipulating their coordinates.\
They aren't simple setters, because if the `canvas` parameter is `!=NULL`, the Object will leave a mark for where it has passed. Read [the pen introduction](<basic-concepts>) to know more.

``` doxygenfunction:: vtXPosition
```

``` doxygenfunction:: vtYPosition
```

``` doxygenfunction:: vtGotoXY
```

``` doxygenfunction:: vtGotoX
```

``` doxygenfunction:: vtGotoY
```

``` doxygenfunction:: vtChangeX
```

``` doxygenfunction:: vtChangeY
```

``` doxygenfunction:: vtAlign
```

## Sensors

Sensor functions can detect if an Object is touching (overlapping) something else in the scene.

``` doxygenfunction:: vtIsTouching
```

``` doxygenfunction:: vtIsTouchingGlyph
```

``` doxygenfunction:: vtIsOutside
```
