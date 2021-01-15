/**
 * @file
 * The VisualT public header.
 */

#pragma once
#ifndef VISUALT_VISUALT_H
#define VISUALT_VISUALT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * Represents a UTF8-encoded read-only codepoint.
 *
 * Being a fully UTF8 compatible library, VisualT stores UTF8 encoded characters in 4 Bytes @c uint32_t variables.
 *
 * This is what the API expects when passing a single UTF8-encoded codepoint. Like in @func{vtFill}.
 */
typedef uint32_t VTChar;

struct VTObj {
	struct VTGlyphMap *sprites, *activeSprite;
	VTChar penChar;
	int x, y;
	unsigned int length;
	unsigned short penSize;
	bool visible;
};

/**
 * Represents a VisualT Object (@e{it's not a pointer! this is the real deal}).
 *
 * As you can see in the header file, the VTObj @c struct is defined in the public header. @e{Why is that?}
 * C doesn't have a proper information hiding system, with @e{opaque pointers} being the only means of hiding the data
 * structure implementation. The trade-off is that the user loses the ability to define statically allocated Objects,
 * and all memory management becomes inaccessible.
 *
 * With the "in plain sight" approach, instead, the user can do whatever he wants with memory, as long as he fulfills
 * the unspoken promise to ignore the implementation and work exclusively via API.
 */
typedef struct VTObj VTObj;

/**
 * A pointer to a UTF8-encoded read-only string.
 *
 * It's defined this way to enforce the requirement that @c char arrays (strings) must be equivalent to byte arrays.
 *
 * This is what the API expects when passing an UTF8-encoded string. Like in @func{vtSetText}.
 */
typedef uint8_t const *VTStr;

/**
 * A pointer to a read-only @type{VTStr}.
 *
 * This is what the API expects when passing an array of UTF8-encoded strings. Like in @func{vtInitializeStrings}.
 */
typedef VTStr const *VTStrs;

/**
 * A pointer to a read-only pair of positive integers.
 *
 * This is what the API expects when passing an array containing multiple Sprite sizes. Like in
 * @func{vtInitializeBlank}.
 */
typedef int const (*VTSizes)[2];

/**
 * A pointer to a read-only pointer to read-only @type{VTObj}.
 *
 * This is what the API expects when passing an array of pointers to Objects. Like in @func{vtRender}.
 */
typedef VTObj const *const *VTObjs;

/**
 * This enumerator is meant to be used with @func{vtOffset}.
 */
typedef enum VTDirection {
	VT_LEFT = 1, VT_RIGHT = 2, VT_TOP = 4, VT_BOTTOM = 8
} VTDirection;

/**
 * A helper macro to cast a @b literal string to @type{VTStr}.
 *
 * @see @func{vtSetText}
 */
#define LTSTR (VTStr)

/**
 * A helper macro to cast a @b literal array of strings to @type{VTStrs}.
 *
 * The main use case is when you want to initialize an Object from one or more literal strings.
 * @see @func{vtInitializeStrings}
 */
#define LTSTRS (VTStrs)(char* [])

/**
 * A helper macro to cast a @b literal array of integer pairs to @type{VTSizes}.
 *
 * The main use case is when you want to initialize a blank Object given its sprites sizes.
 * @see @func{vtInitializeBlank}
 */
#define LTSIZES (VTSizes)(unsigned int [][2])

/**
 * A helper macro to cast a @b literal array of pointers to Objects to @type{VTObjs}.
 *
 * An example is when you want to render a group of Objects.
 * @see @func{vtRender}
 */
#define LTOBJS (VTObjs)(VTObj* [])

/**
 * Prints info about VisualT.
 *
 * Prints the @e version, @e{build date}, and @e{GitHub website}.
 */
void vtAbout();

/**
 * A helper function to convert a @b{single-glyph} literal string to a @type{VTChar}.
 * @param ltChar a UTF8-encoded string containing a single glyph
 * @return The first glyph encoded in a @type{VTChar}, so that VisualT can process it.
 */
VTChar vtChar(char const *ltChar);

/**
 * Initializes  @c *obj to a blank Object with @c sizesLength Sprites of @c sizes size.
 * @code
 * vtInitializeBlank(&canvas, 2, LTSIZES{{10, 5},{5, 10}});
 * @endcode
 * The example initializes @c canvas as an Object with two blank (transparent) Sprites, of dimensions 10x5 and 5x10
 * respectively. This initializer is convenient for setting up an empty canvas.
 *
 * @param obj a pointer the Object to initialize
 * @param sizesLength the length of @c sizes, equivalent to the number of Sprites
 * @param sizes (@macro{LTSIZES}) an array of integer pairs, each pair represent the dimension of the Sprite
 */
void vtInitializeBlank(VTObj *obj, unsigned int sizesLength, VTSizes sizes);

/**
 * Initializes @c *obj from the native @e{serialized Object array} @c v.
 *
 * VisualT has its own import and export format, consisting of a sequence of @type{VTChar} that can be
 * embedded in the source as a literal array, or @func{vtInitializeFile,read from a file}. Currently, the only way to
 * serialize an Object is via @func{vtSerialize}, but it's still a quite powerful tool: it lets you leverage the library
 * to programmatically compose complex scenes from simpler Objects, and then "bake" the result to a new, standalone
 * Object.
 *
 * Exporters for existing text-art editors may be created in the future.
 *
 * @param obj a pointer to the Object to initialize
 * @param v a serialized Object array
 */
void vtInitializeArray(VTObj *obj, VTChar const *v);

/**
 * Initializes @c *obj from the @e{native serialized Object file} pointed by @c file.
 *
 * A @e{serialized Object file} is a plain text file containing a @e{serialized Object array}'s values separated by a
 * space.
 * The file is automatically closed.
 * @see @func{vtInitializeArray}
 * @param obj a pointer the Object to initialize
 * @param file a pointer to a serialized Object file
 */
int vtInitializeFile(VTObj *obj, FILE *file);

/**
 * Initializes @c *obj from the array of strings @c utf8Strings of length @c utf8StringsLength.
 *
 * This initializer is handy for creating Objects from strings on the go. Each @type{VTStr} becomes a Sprite, rows are
 * delimited by @e{line feeds} @c \\n. Each Sprite will be wide enough to accommodate the longest row, and trailing
 * empty cells will be transparent. Normal space characters will be interpreted literally (opaque), but you can use
 * @c \\v to indicate an empty cell.
 * @code
 * vtInitializeStrings(&obj, 3, LTSTRS{"sprite A","sprite\vB\nsecond\vrow"});
 * @endcode
 * - @e{Sprite 1}: width:8 height:1, fully opaque
 * - @e{Sprite 2}: width:10 height:2, all empty cells are transparent
 *
 * @param obj a pointer to the Object to initialize
 * @param utf8StringsLength the length of @c utf8Strings, equivalent to the number of Sprites
 * @param utf8Strings (@macro{LTSTRS}) an array of UTF8 strings, each string becomes a Sprite
 */
void vtInitializeStrings(VTObj *obj, unsigned int utf8StringsLength, VTStrs utf8Strings);

/**
 * Initializes @c *obj as a clone of @c *src.
 *
 * This initializer effectively creates a deep clone of an Object. All the data in the heap is copied to a new location.
 *
 * @param obj a pointer to the Object to initialize
 * @param src a pointer to the source Object
 */
void vtInitializeObj(VTObj *restrict obj, VTObj const *restrict src);

/**
 * Frees the dynamic memory of all the Objects referenced in the array @c objs.
 *
 * Once released, and Object can be re-initialized safely.
 *
 * @param objsLength the length of @c objs
 * @param objs (@macro{LTOBJS}) an array of pointers to the Objects you want to release
 */
void vtRelease(unsigned int objsLength, VTObj const *restrict const *objs);

/**
 * A @e reallocator that replaces Sprite n°@c spriteDest of @c *dest with a clone of Sprite n°@c spriteSrc of @c *src.
 *
 * The source and destination Sprites must be different.
 *
 * @param dest a pointer to the Object with the Sprite to replace
 * @param spriteDest the to-be-replaced Sprite number
 * @param src a pointer to the Object with the Sprite to clone
 * @param spriteSrc the to-be-cloned Sprite number
 */
void vtCloneSprite(VTObj const *dest, unsigned int spriteDest, VTObj const *src, unsigned int spriteSrc);

/**
 * A @e reallocator that resizes @c{*obj}' Sprite to the defined @c width and @c height. The Sprite is
 * @func{vtClear,cleared} in the process.
 *
 * This comes handy when you need to resize the canvas.
 *
 * @param obj a pointer to the Object whose Sprite you want to resize
 * @param width the new width
 * @param height the new height
 */
void vtResize(VTObj *obj, unsigned int width, unsigned int height);

/**
 * Draws all the Objects referenced in the @c objs array onto the @c *canvas.
 *
 * This is the main function that renders your scene. The last Object in the array will be in the topmost layer,
 * followed by the others. Hidden Objects will be skipped.
 * @code
 * vtRender(&canvas, 3, LTOBJS{&objA, &objB, &objC});
 * @endcode
 * The main difference compared to @func{vtStamp}, is that the canvas is @func{vtClear,cleared} in the process, and that
 * allows to start drawing from the last Object, and only overwrite the empty cells.
 *
 * @param canvas a pointer to the Object that will act as the canvas
 * @param objsLength the length of @c objs
 * @param objs (@macro{LTOBJS}) an array of pointers to the Objects you want to render
 */
void vtRender(VTObj const *canvas, unsigned int objsLength, VTObjs objs);

/**
 * @e Stamps all the Objects referenced in the @c objs array onto the @c *canvas.
 *
 * The main difference compared to @func{vtRender}, is that the pre-existing content on the canvas is left intact,
 * so every Object must be drawn completely, starting from index 0, and eventually overwritten by the following
 * Objects.
 *
 * @param canvas a pointer to the Object that will act as the canvas
 * @param objsLength the length of @c objs
 * @param objs (@macro{LTOBJS}) an array of pointers to the Objects you want to stamp
 */
void vtStamp(VTObj const *canvas, unsigned int objsLength, VTObjs objs);

/**
 * Prints @c *obj to stdout. If @c border is true, also prints a frame around the output.
 *
 * The printing is row-buffered: a buffer large enough to store an entire row (a little bit more than 4*width Bytes) is
 * allocated and deallocated by the function. The last character printed is always a @e{line feed} @c \\n.
 *
 * @param obj a pointer to the Object you want to print
 * @param border print the frame?
 */
void vtPrint(VTObj const *obj, bool border);

/**
 * Prints @c *obj to @c *utf8Buffer, behaves the same as @func{vtPrint}. If @c{*utf8Buffer==NULL}, the buffer
 * will be allocated automatically. There will be no @e{line feed} @c \\n at the end.
 * - @e{Usage example 1}: the output buffer is allocated by the function:
 * @code
 * unsigned char *s = NULL; // remember to initialize the pointer to NULL!
 * size_t size = printToString(&obj, true, &s); // size = strlen(s) + 1('\0')
 * puts(s);
 * free(s); // remember to free the buffer!
 * @endcode
 * - @e{Usage example 2}: the output buffer is allocated by the user:
 * @code
 * unsigned char *s = malloc(vtPrintStringSize(&obj, true)); // remember to use the same border value
 * size_t size = printToString(&obj, true, &s); // size = strlen(s) + 1('\0')
 * puts(s);
 * free(s); // remember to free the buffer!
 * @endcode
 * If the provided buffer isn't large enough, the function will cause memory corruption. Use @func{vtPrintStringSize} to
 * get the minimum required size.

 * @param obj a pointer to the Object you want to print
 * @param border print the frame?
 * @param utf8Buffer a pointer to a pointer to a @c char buffer. @c *utf8Buffer will be allocated (thus overwritten) if
 * @c NULL
 * @return The amount of bytes written by the function, must be ≥ to the size of @c *utf8Buffer.
 */
size_t vtPrintToString(VTObj const *obj, bool border, uint8_t **utf8Buffer);

/**
 * Calculates the size of the buffer @c *obj would occupy if printed. With or without @c border.
 *
 * This is useful if you want to allocate the buffer yourself when using @func{vtPrintToString}.
 *
 * @param obj a pointer to the Object you want to process
 * @param border add the frame to the calculation?
 * @return The size of the buffer @c *obj would occupy if printed.
 */
size_t vtPrintStringSize(VTObj const *obj, bool border);

/**
 * Gets @c{*obj}'s amount of Sprites.
 * @param obj a pointer to the Object
 * @return The amount of Sprites.
 */
unsigned int vtSprites(VTObj const *obj);

/**
 * Gets the index of @c{*obj}'s active Sprite.
 * @param obj a pointer to the Object
 * @return The index of the active Sprite.
 */
unsigned int vtSpriteInd(VTObj const *obj);

/**
 * Sets @c{*obj}'s next Sprite as active.
 * @param obj a pointer to the Object
 */
void vtNextSprite(VTObj *obj);

/**
 * Sets @c{*obj}'s preceding Sprite as active.
 * @param obj a pointer to the Object
 */
void vtPrecSprite(VTObj *obj);

/**
 * Sets @c{*obj}'s Sprite of index @c index as active.
 * @param obj a pointer to the Object
 * @param index the index of the Sprite you want to set as active
 */
void vtSetSprite(VTObj *obj, unsigned int index);

/**
 * Gets the width of @c{*obj}'s active Sprite.
 * @param obj a pointer to the Object
 * @return The width of the active Sprite.
 */
int vtWidth(VTObj const *obj);

/**
 * Gets the height of @c{*obj}'s active Sprite.
 * @param obj a pointer to the Object
 * @return The height of the active Sprite.
 */
int vtHeight(VTObj const *obj);

/**
 * Gets the maximum visible coordinate (extrema) of the given sprite
 * @param obj a pointer to the Object
 * @param direction specifies the desired direction
 * @return The coordinate value.
 */
int vtExtremum(VTObj const *obj, VTDirection direction);

/**
 * Sets @c{*obj}'s active Sprite content to @c utf8String. If @c fitToContent is @c true, the sprite will be reallocated
 * to fit @c utf8String. The parsing is done the same as in @func{vtInitializeStrings}.
 * @code
 * vtSetText(&obj, LTSTR "A\vKaomoji\n\v(o˘◡˘o)");
 * @endcode
 * @param obj a pointer to the Object
 * @param fitToContent resize the sprite
 * @param utf8String (@macro{LTSTR}) an UTF8 string
 */
void vtSetText(VTObj *obj, bool fitToContent, VTStr utf8String);

/**
 * Clears @c{*obj}'s active Sprite. Clearing a Sprite sets all its cell to 0 (transparent).
 *
 * Equivalent to @c{fill(&obj, 0);}.
 *
 * @param obj a pointer to the Object you want to clear
 */
void vtClear(VTObj const *obj);

/**
 * Sets every cell of @c{*obj}'s active Sprite to the glyph in @c fillChar.
 * @code
 * vtFill(&obj, vtChar(LTSTR "▀"));
 * @endcode
 * @param obj a pointer to the Object you want to fill
 * @param fillChar (@func{vtChar}) the character you want to fill the obj with
 */
void vtFill(VTObj const *obj, VTChar fillChar);

/**
 * Replaces every cell of @c{*obj}'s active Sprite containing @c oldChar with the glyph in @c newChar.
 * @param obj a pointer to the Object
 * @param oldChar (@func{vtChar}) the character you want to replace
 * @param newChar (@func{vtChar}) the replacing character
 */
void vtReplace(VTObj const *obj, VTChar oldChar, VTChar newChar);

/**
 * Shifts every cell of @c{*obj}'s active Sprite of @c amount in the specified @c direction.
 *
 * Trailing cells will be empty.
 *
 * @param obj a pointer to the Object
 * @param direction in which direction to perform the shift
 * @param amount the shift amount. Negative values are supported
 */
void vtShift(VTObj const *obj, VTDirection direction, int amount);

/**
 * Rotates each row/column of @c{*obj}'s active Sprite of @c amount in the specified @c direction.
 * @param obj a pointer to the Object
 * @param direction in which direction to perform the row/column rotation
 * @param amount the rotation amount. Negative values are supported
 */
void vtRotate(VTObj const *obj, VTDirection direction, int amount);

/**
 * Draws the content of @c{*src}'s Sprite n°@c spriteSrc onto @c{*dest}'s Sprite n°@c spriteDest of the same size.
 *
 * This function exists to solve a very specific task: to flatten the canvas layers. You can set up a canvas with two or
 * more Sprites @b{of the same size}, and use them as layers (e.g. render layer + pen layer). This function lets you
 * @e overlay a Sprite on another, even if they belong to the same Object.
 *
 * @param dest a pointer to the Object with the Sprite on which to draw
 * @param spriteDest the to-be-drawn onto Sprite number
 * @param src a pointer to the Object with the Sprite to overlay
 * @param spriteSrc the to-be-overlaid Sprite number
 */
void vtOverlay(VTObj const *dest, unsigned int spriteDest, VTObj const *src, unsigned int spriteSrc);

/**
 * Draws a visual representation of the coordinate system on the @c{*obj}.
 * @param obj a pointer to the affected Object
 */
void vtDrawAxes(VTObj const *obj);

/**
 * Gets @c{*obj}'s visibility flag.
 * @param obj a pointer to the processed Object
 * @return @c true if the Object is visible, @c false otherwise.
 */
bool vtVisible(VTObj const *obj);

/**
 * Sets @c{*obj}'s visibility flag to @c true.
 * @param obj a pointer to the affected Object
 */
void vtShow(VTObj *obj);

/**
 * Sets @c{*obj}'s visibility flag to @c false.
 * @param obj a pointer to the affected Object
 */
void vtHide(VTObj *obj);

/**
 * Sets @c{*obj}'s visibility flag to @c visible.
 * @param obj a pointer to the affected Object
 * @param visible the value you want to set
 */
void vtSetVisibility(VTObj *obj, bool visible);

/**
 * Exports @c *obj as a @e{serialized object}, and writes it to the array @c *v if @c v!=NULL, otherwise to stdout.
 *
 * If the provided array isn't large enough, the function will cause memory corruption. Use @func{vtSerializedArraySize}
 * to get the minimum required size.
 *
 * @param obj a pointer to the processed Object
 * @param v @e{nullable}: a pointer to a @c uint32_t array
 */
void vtSerialize(VTObj const *obj, unsigned int *v);

/**
 * Calculates the size of the array that @c *obj would occupy if serialized.
 *
 * This is useful if you want to allocate the array yourself when using @func{vtSerialize}.
 *
 * @param obj a pointer to the processed Object
 * @return The size of the array @c *obj would occupy if serialized.
 */
size_t vtSerializedArraySize(VTObj const *obj);

/**
 * Gets @c{*obj}'s pen glyph.
 * @param obj a pointer to the Object you wan to process
 * @return The Object's pen glyph.
 */
VTChar vtPenGlyph(VTObj const *obj);

/**
 * Gets @c{*obj}'s pen size.
 * @param obj a pointer to the Object you wan to process
 * @return The Object's pen size, if outside the [1,6] interval, it will default to 1
 */
unsigned short vtPenSize(VTObj const *obj);

/**
 * Sets @c{*obj}'s pen size to @c size.
 * @param obj a pointer to the affected Object
 * @param size the value you want to set, if outside of the [1,6] interval, the pen size defaults to 1
 */
void vtSetPenSize(VTObj *obj, unsigned short size);

/**
 * Sets @c{*obj}'s pen glyph to the value of @c penChar.
 * @param obj a pointer to the affected Object
 * @param penChar (@func{vtChar}) the value you want to set
 */
void vtSetPenGlyph(VTObj *obj, VTChar penChar);

/**
 * Gets @c{*obj}'s @b x position.
 * @param obj a pointer to the processed Object
 * @return The @b x position of the Object.
 */
int vtXPosition(VTObj const *obj);

/**
 * Gets @c{*obj}'s @b x position.
 * @param obj obj a pointer to the processed Object
 * @return The @b x position of the Object.
 */
int vtYPosition(VTObj const *obj);

/**
 * Moves @c *obj to @c x and @c y. The movement will leave a pen trail on the @c *canvas, if specified.
 * @param canvas @e{nullable}: a pointer to the Object the pen will draw onto, usually the canvas
 * @param obj a pointer to the affected Object
 * @param x the target @b x coordinate
 * @param y the target @b y coordinate
 */
void vtGotoXY(VTObj const *canvas, VTObj *obj, int x, int y);

/**
 * Moves @c *obj to @c x. The movement will leave a pen trail on the @c *canvas, if specified.
 * @param canvas @e{nullable}: a pointer to the Object the pen will draw onto, usually the canvas
 * @param obj a pointer to the affected Object
 * @param x the target @b x coordinate
 */
void vtGotoX(VTObj const *canvas, VTObj *obj, int x);

/**
 * Moves @c *obj to @c y. The movement will leave a pen trail on the @c *canvas, if specified.
 * @param canvas @e{nullable}: a pointer to the Object the pen will draw onto, usually the canvas
 * @param obj a pointer to the affected Object
 * @param y the target @b y coordinate
 */
void vtGotoY(VTObj const *canvas, VTObj *obj, int y);

/**
 * Changes @c{*obj}'s @b x position by @c x. The movement will leave a pen trail on the @c *canvas, if specified.
 * @param canvas @e{nullable}: a pointer to the Object the pen will draw onto, usually the canvas
 * @param obj a pointer to the affected Object
 * @param x the @b x coordinate shift
 */
void vtChangeX(VTObj const *canvas, VTObj *obj, int x);

/**
 * Changes @c{*obj}'s @b y position by @c y. The movement will leave a pen trail on the @c *canvas, if specified.
 * @param canvas @e{nullable}: a pointer to the Object the pen will draw onto, usually the canvas
 * @param obj a pointer to the affected Object
 * @param y the @b y coordinate shift
 */
void vtChangeY(VTObj const *canvas, VTObj *obj, int y);

/**
 * Modifies @c{*obj}'s coordinates so that the specified @c position matches with the Object position.
 * @param obj a pointer to the affected Object
 * @param direction how to align the Object, see @enum{VTAlign}
 */
void vtAlign(VTObj *obj, VTDirection direction);


/**
 * Detects if @c *obj is overlapping any of the Objects referenced in @c objs.
 *
 * This sensor detects pixel perfect Object-on-Object collisions, in the space delimited by the @c *canvas. Transparent
 * pixels won't be considered. This is the slowest sensor, as it also allocates memory.
 *
 * @param canvas a pointer to the Object whose area will be searched for collisions, usually the canvas
 * @param obj a pointer to the Object you want to process
 * @param objsLength the length of @c objs
 * @param objs (@macro{LTOBJS}) an array of pointers to the Objects you want to consider in the test
 * @return @c true if there is a collision, @c false otherwise.
 */
bool vtIsTouching(VTObj const *canvas, VTObj const *obj, unsigned int objsLength, VTObjs objs);

/**
 * Detects if @c *obj is overlapping any cell in the @c *canvas containing @c testChar.
 *
 * This sensor detects pixel perfect Object-on-char collisions. Transparent pixels won't be considered.
 *
 * @param canvas a pointer to the Object whose area will be searched for collisions, usually the canvas
 * @param obj a pointer to the Object you want to process
 * @param testChar (@func{ltChar}) the character you want to test
 * @return @c true if there is a collision, @c false otherwise.
 */
bool vtIsTouchingGlyph(VTObj const *canvas, VTObj const *obj, VTChar testChar);

/**
 * Detects if @c *obj is spilling outside the @c *canvas area.
 *
 * This sensor detects pixel perfect Object-on-area collisions. Transparent pixels won't be considered.
 *
 * @param canvas a pointer to the Object whose area will be tested, usually the canvas
 * @param obj a pointer to the Object you want to process
 * @return @c true if there is a collision, @c false otherwise.
 */
bool vtIsOutside(VTObj const *canvas, VTObj const *obj);

#endif

