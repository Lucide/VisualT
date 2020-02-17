#pragma once
#ifndef VISUALT_INCLUDE_VISUALT_VISUALT_H_
#define VISUALT_INCLUDE_VISUALT_VISUALT_H_

#include <stdbool.h>
#include <stdint.h>

// #define VISUALT_UNBUFFERED_PRINT

typedef uint32_t VTChar;
#define LTCHAR *(VTChar *const)

typedef const uint8_t *VTStr;
#define LTSTR (VTStr)

typedef const uint8_t *const *VTStrs;
#define LTSTRS (VTStrs)(const char *const [])

typedef const unsigned int (*VTSizes)[2];
#define LTSIZES (VTSizes)(const unsigned int [][2])

typedef const struct vtObj *const *VTObjs;
#define LTOBJS (VTObjs)(const struct vtObj *const [])

struct vtCharMap {
	VTChar *chars;
	unsigned int width, height;
};

struct vtObj {
	struct vtCharMap *sprites, *currentSprite;
	VTChar penChar;
	int x, y;
	unsigned int length;
	unsigned short penSize;
	bool visible;
};

//----MISC----
void vtAbout(); //print info about VisualT
//----INITIALIZATION----
void vtInitializeBlank(struct vtObj *obj, unsigned int sizesLength, VTSizes sizes);
void vtInitializeArray(struct vtObj *obj, const VTChar *v);
void vtInitializeFile(struct vtObj *obj, const char path[const]);
void vtInitializeString(struct vtObj *obj, unsigned int utf8StringsLength, VTStrs utf8Strings);
void vtInitializeObj(struct vtObj *obj, const struct vtObj *src);
void vtRelease(unsigned int objsLength, VTObjs objs);
void vtCloneSprite(const struct vtObj *dest, unsigned int spriteDest, const struct vtObj *src, unsigned int spriteSrc);
void vtResize(struct vtObj *obj, unsigned int width, unsigned int height);
//----REFRESH----
void vtRender(const struct vtObj *canvas, unsigned int objsLength, VTObjs objs);
void vtStamp(const struct vtObj *canvas, unsigned int objsLength, VTObjs objs);
void vtPrint(const struct vtObj *canvas, bool border);
unsigned int vtPrintToString(const struct vtObj *canvas, bool border, uint8_t **utf8String);
//----SPRITE----
unsigned int vtSprites(const struct vtObj *obj);
unsigned int vtSpriteInd(const struct vtObj *obj);
void vtNextSprite(struct vtObj *obj);
void vtPrecSprite(struct vtObj *obj);
void vtSetSprite(struct vtObj *obj, unsigned int sprite);
unsigned int vtWidth(const struct vtObj *obj);
unsigned int vtHeight(const struct vtObj *obj);
void vtSetText(struct vtObj *obj, VTStr utf8Text);
void vtClear(const struct vtObj *canvas);
void vtFill(const struct vtObj *canvas, VTChar fillChar);
void vtOverlay(const struct vtObj *dest, unsigned int spriteDest, const struct vtObj *src, unsigned int spriteSrc);
void vtPrintAxes(const struct vtObj *canvas);
//----OBJ----
bool vtVisible(const struct vtObj *obj);
void vtShow(struct vtObj *obj);
void vtHide(struct vtObj *obj);
void vtSetVisibility(struct vtObj *obj, bool visible);
//----PEN----
VTChar vtPenChar(const struct vtObj *obj);
unsigned short vtPenSize(const struct vtObj *obj);
void vtSetPenSize(struct vtObj *obj, unsigned short size);
void vtSetPenChar(struct vtObj *obj, VTChar penChar);
//----MOVE----
int vtXPosition(const struct vtObj *obj);
int vtYPosition(const struct vtObj *obj);
void vtGotoXY(const struct vtObj *canvas, struct vtObj *obj, int x, int y);
void vtGotoX(const struct vtObj *canvas, struct vtObj *obj, int x);
void vtGotoY(const struct vtObj *canvas, struct vtObj *obj, int y);
void vtChangeX(const struct vtObj *canvas, struct vtObj *obj, int x);
void vtChangeY(const struct vtObj *canvas, struct vtObj *obj, int y);
void vtAlign(struct vtObj *obj, unsigned char position);
bool vtIsTouching(const struct vtObj *canvas, const struct vtObj *obj, unsigned int objsLength, VTObjs objs);
bool vtIsTouchingChar(const struct vtObj *canvas, const struct vtObj *obj, VTChar character);
bool vtIsOutside(const struct vtObj *canvas, const struct vtObj *obj);

#endif

//----MISC----

//to free a dynamic 2d array mna of width width

//----POINTERS----
//to refer to an object's INDex
//to group "n" objects by passing their index
//to delete the data contained on a group; now you can reuse the group

//----GETTERS----
//to Get the Number of Objects loaded
//to Get the Object's INDex
//to Get the Object's NAME
//to Get the Object's Sprite Number
//to Get the Object's Sprite X size
//to Get the Object's Sprite Y size

//----INITIALIZATION----
//REQUIRED to initialize the library
//to load an object
//to create a dynamic textbox
//to clone (reload) the object
//to free all the memory allocated by the library

//----STAGE----
//to Get the STage Width
//to Get the STage Height
//to create/size a new stage
//to set the stage border visibility (enabled by default, for developers)

//----REFRESH----
//to render the scene
//to render to file
//to render to a printf() sequence, waiting for i milliseconds
//to move the cursor to a certain x y

//----TEXT----
//to change a textbox's content, works for every object

//----LOOK----
//to Get the object's current Sprite Number
//to Get the object's VISIBILITY state
//to Get the object's LAYER
//to show the object
//to hide the object
//to set the object's visibility (1/0)
//to move the object to the top layer
//to move the object to the bottom layer
//to move the object up a layer
//to move the object down a layer
//to switch to the object's next Sprite
//to switch to the object's precedent Sprite
//to switch to the object's n Sprite

//----PEN----
//to Get the object's PEN state
//to Get the PEN CHaracter in use
//to Get the PEN SIZE
//to set the pen size
//to set the pen character
//to disable the object's pen
//to enable the object's pen
//to set the object's pen (1/0)
//to stamp the object on the pen layer
//to shift the pen layer by m direction m=0:left m=1:right m=2:top m=3:down
//to set the pen layer to a character
//to clear the pen layer

//----MOVE----
//to Get the object's x position
//to Get the object's y position
//to know if two Objects are touching  (d=0 to check for any object)
//to know if the object is touching a Character (d=0 to check for border)
//to move the object to x y
//to move the object to x
//to move the object to y
//to change the object's x position by x
//to change the object's y position by y
//to move the object's corner to his current position (useful with textboxes) m=0:top-left m=1:top-right m=2:bottom-left m=3:bottom-right

//----DEBUG----
//to show your program general status
//to show the status of all the loaded objects
//to show the oject's status
//to render x and y axes
