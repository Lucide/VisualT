//Cavasin Riccardo

#ifndef VISUALT_H
#define VISUALT_H

#include <stdbool.h>
#include <stdint.h>

struct Canvas;
struct Obj;

//----MISC----
void about();
//----INITIALIZATION----
void initializeCanvas(struct Canvas *canvas, unsigned int width, unsigned int height);
void initializeArrayObj(struct Obj *obj, const uint32_t *v);
void initializeFileObj(struct Obj *obj, const char path[const]);
void initializeStringObj(struct Obj *obj, const uint8_t *utf8Text);
void deleteCanvas(const struct Canvas *canvas);
void deleteObj(const struct Obj *obj);
void cloneCanvas(struct Canvas *dest, const struct Canvas *src);
void cloneObj(struct Obj *dest, const struct Obj *src);
//----CANVAS----
void resizeCanvas(struct Canvas *canvas, unsigned int width, unsigned int height);
unsigned int canvasWidth(const struct Canvas *canvas);
unsigned int canvasHeight(const struct Canvas *canvas);
void canvasBorder(struct Canvas *canvas, bool border);
//----REFRESH----
void draw(const struct Canvas *canvas, unsigned int objsLength, const struct Obj *const objs[objsLength]);
void drawToString(const struct Canvas *canvas, unsigned int objsLength, const struct Obj *const objs[objsLength], unsigned int *stringLength, uint8_t **utf8Text);
//----TEXT----
void setSpriteText(struct Obj *obj, const uint8_t *utf8Text);
//----SPRITE----
unsigned int sprites(const struct Obj *obj);
unsigned int width(const struct Obj *obj);
unsigned int height(const struct Obj *obj);
void nextSprite(struct Obj *obj);
void precSprite(struct Obj *obj);
void setSprite(struct Obj *obj, unsigned int sprite);
//----OBJ----
bool visible(const struct Obj *obj);
void show(struct Obj *obj);
void hide(struct Obj *obj);
void setVisible(struct Obj *obj, bool visible);
//----PEN----
bool isPen(const struct Obj *obj);
uint32_t penChar(const struct Obj *obj);
unsigned short penSize(const struct Obj *obj);
void setPenSize(struct Obj *obj, unsigned short size);
void setPenChar(struct Obj *obj, uint32_t penChar);
void penUp(struct Obj *obj);
void penDown(struct Obj *obj);
void setPen(struct Obj *obj, bool pen);
void stamp(const struct Canvas *canvas, const struct Obj *obj);
void penFill(const struct Canvas *canvas, uint32_t fillChar);
void penShift(const struct Canvas *canvas, unsigned char direction);
void printAxes(const struct Canvas *canvas);
void clearPen(const struct Canvas *canvas);
//----MOVE----
int xPostion(const struct Obj *obj);
int yPosition(const struct Obj *obj);
void moveTo(const struct Canvas *canvas, struct Obj *obj, int x, int y);
void setX(const struct Canvas *canvas, struct Obj *obj, int x);
void setY(const struct Canvas *canvas, struct Obj *obj, int y);
void changeX(const struct Canvas *canvas, struct Obj *obj, int x);
void changeY(const struct Canvas *canvas, struct Obj *obj, int y);
void align(struct Obj *obj, unsigned char position);
bool isTouching(const struct Canvas *canvas, const struct Obj *obj, unsigned int objsLength, const struct Obj *const objs[objsLength]);
bool isTouchingChar(const struct Canvas *canvas, const struct Obj *obj, uint32_t character);
bool isOutside(const struct Canvas *canvas, const struct Obj *obj);

#endif
