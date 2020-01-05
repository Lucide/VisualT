//Cavasin Riccardo

#include <visualt/visualt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct CharMap CharMap;

typedef struct Obj Obj;

typedef struct Canvas Canvas;

//----INTERNALs----
#define vtPuts(STRING) fputs((STRING), stdout)

#define vt32Puts(UINT32, LENGTH) fwrite((UINT32), sizeof(uint32_t), (LENGTH), stdout)

#define vt32Putchar(UINT32) putchar((UINT32)>>0&0xffu);   \
                            putchar((UINT32)>>8&0xffu);   \
                            putchar((UINT32)>>16&0xffu);  \
                            putchar((UINT32)>>24&0xffu)

#define vt32EndiannessSwap(UINT32) (((UINT32)>>24u)&0xffu|((UINT32)>>8u)&0xff00u|((UINT32)<<8u)&0xff0000u|((UINT32)<<24u)&0xff000000u)

#define vtInitializeObj(OBJ)  (OBJ)->x = 0;           \
                              (OBJ)->y = 0;           \
                              (OBJ)->visible = true;  \
                              (OBJ)->pen = false;     \
                              (OBJ)->currentSprite = (OBJ)->sprites

#define vtInitializeCharMap(CHARMAP, WIDTH, HEIGHT) (CHARMAP)->width = (WIDTH);   \
                                                    (CHARMAP)->height = (HEIGHT); \
                                                    (CHARMAP)->chars = malloc(vtSizeofChars(CHARMAP))

#define vtSizeofChars(CHARMAP) ((CHARMAP)->width*(CHARMAP)->height*sizeof(uint32_t))

#define vtNormalizePosition(CHARMAP, SPRITE, SPRITEX, SPRITEY, X, Y)  (X) = (int)((CHARMAP)->width/2-(SPRITE)->width/2+(SPRITEX));  \
                                                                      (Y) = (int)((CHARMAP)->height/2-(SPRITE)->height/2-(SPRITEY))

static void vtInitializeStringCharMap(CharMap *const charMap, const uint8_t *const utf8Text) {
  unsigned int charMapWidth = 1, charMapHeight = 1;
  //count size
  for(unsigned int t = 0, i = 0; utf8Text[i] != 0; i++) {
    if((utf8Text[i]&0xc0u) != 0x80u) { // partial code point: 10xx xxxx
      t++;
      if(t > charMapWidth) {
        charMapWidth = t;
      }
    } else if(utf8Text[i] == '\n') {
      charMapHeight++;
      t = 0;
    }
  }
  vtInitializeCharMap(charMap, charMapWidth, charMapHeight);
  //parse
  for(unsigned int i = 0, y = 0; y < charMapHeight; i++, y++) {
    for(unsigned int x = 0; x < charMapWidth; x++) {
      if(utf8Text[i] >= 0xf0) {        //4B code point: 1111 0xxx
        charMap->chars[x+y*charMapWidth] = (uint32_t)utf8Text[i]|((uint32_t)utf8Text[i+1]<<8u)|((uint32_t)utf8Text[i+2]<<16u)|((uint32_t)utf8Text[i+3]<<24u);
        i += 4;
      } else if(utf8Text[i] >= 0xe0) { //3B code point: 1110 xxxx
        charMap->chars[x+y*charMapWidth] = (uint32_t)utf8Text[i]|((uint32_t)utf8Text[i+1]<<8u)|((uint32_t)utf8Text[i+2]<<16u);
        i += 3;
      } else if(utf8Text[i] >= 0xc0) { //2B code point: 110x xxxx
        charMap->chars[x+y*charMapWidth] = (uint32_t)utf8Text[i]|((uint32_t)utf8Text[i+1]<<8u);
        i += 2;
      } else {
        if(utf8Text[i] != '\n') {      //1B code point: 0xxx xxxx
          charMap->chars[x+y*charMapWidth] = (uint32_t)utf8Text[i];
        }
        i += 1;
      }
    }
  }
}

static void vtPrintCharMap(const CharMap *const charMap, const bool border) {
  const unsigned int width = charMap->width, height = charMap->height;
  const uint32_t *const chars = charMap->chars;

  if(border) {
    vtPuts("┌");
    for(unsigned int x = 0; x < width-1; x++) {
      vtPuts("─");
    }
    vtPuts("─┐\n");
    for(unsigned int y = 0; y < width*height; y += width) {
      vtPuts("│");
      vt32Puts(&chars[0+y], width);
      vtPuts("│\n");
    }
    vtPuts("└");
    for(unsigned int x = 0; x < width-1; x++) {
      vtPuts("─");
    }
    vtPuts("─┘\n");
  } else {
    for(unsigned int y = 0; y < width*height; y += width) {
      vtPuts("│");
      vt32Puts(&chars[0+y], width);
      vtPuts("│\n");
    }
  }
}

static void vtPrintStringCharMap(const CharMap *const charMap, const bool border, unsigned int *const stringLength, uint8_t **const utf8Text) {
  const unsigned int width = charMap->width, height = charMap->height;
  const uint32_t *const chars = charMap->chars;
  unsigned int i = 0;

  *stringLength = 0;
  for(unsigned int y = 0; y < height; (*stringLength)++, y++) {
    uint32_t t;
    for(unsigned int x = 0; x < width; x++) {
      t = chars[x+y*width]&0xffu;
      if(t >= 0xf0) {        //4B code point: 1111 0xxx
        *stringLength += 4;
      } else if(t >= 0xe0) { //3B code point: 1110 xxxx
        *stringLength += 3;
      } else if(t >= 0xc0) { //2B code point: 110x xxxx
        *stringLength += 2;
      } else {               //1B code point: 0xxx xxxx
        *stringLength += 1;
      }
    }
  }
  if(border) {
    *stringLength += 3*((width+2)*2)+2+3*(2*height);
  }

  *utf8Text = malloc(*stringLength*sizeof(uint8_t));
  uint8_t *const text = *utf8Text;

  if(border) {
    i = *stringLength-2;
    text[i--] = 0x98u;  //┘
    text[i--] = 0x94u;  //┘
    text[i--] = 0xe2u;  //┘
    for(unsigned int j = 0; j < width; j++) {
      text[i--] = 0x80u;  //─
      text[i--] = 0x94u;  //─
      text[i--] = 0xe2u;  //─
    }
    text[i--] = 0x94u;  //└
    text[i--] = 0x94u;  //└
    text[i] = 0xe2u;    //└
    i = 0;
    text[i++] = 0xe2u;  //┌
    text[i++] = 0x94u;  //┌
    text[i++] = 0x8cu;  //┌
    for(unsigned int j = 0; j < width; j++) {
      text[i++] = 0xe2u;  //─
      text[i++] = 0x94u;  //─
      text[i++] = 0x80u;  //─
    }
    text[i++] = 0xe2u;  //┐
    text[i++] = 0x94u;  //┐
    text[i++] = 0x90u;  //┐
    text[i++] = '\n';
  }

  for(unsigned int y = 0; y < height; y++) {
    uint32_t t;
    for(unsigned int x = 0; x < width; x++) {
      if(border) {
        text[i++] = 0xe2u;  //│
        text[i++] = 0x94u;  //│
        text[i++] = 0x82u;  //│
      }
      t = chars[x+y*width];
      if((t&0xffu) >= 0xf0) {        //4B code point: 1111 0xxx
        text[i++] = (uint8_t)((t>>0u)&0xffu);
        text[i++] = (uint8_t)((t>>8u)&0xffu);
        text[i++] = (uint8_t)((t>>16u)&0xffu);
        text[i++] = (uint8_t)((t>>24u)&0xffu);
      } else if((t&0xffu) >= 0xe0) { //3B code point: 1110 xxxx
        text[i++] = (uint8_t)((t>>0u)&0xffu);
        text[i++] = (uint8_t)((t>>8u)&0xffu);
        text[i++] = (uint8_t)((t>>16u)&0xffu);
      } else if((t&0xffu) >= 0xc0) { //2B code point: 110x xxxx
        text[i++] = (uint8_t)((t>>0u)&0xffu);
        text[i++] = (uint8_t)((t>>8u)&0xffu);
      } else {                       //1B code point: 0xxx xxxx
        text[i++] = (uint8_t)((t>>0u)&0xffu);
      }
    }
    if(border) {
      text[i++] = 0xe2u;  //│
      text[i++] = 0x94u;  //│
      text[i++] = 0x82u;  //│
    }
    text[i++] = '\n';
  }
  text[*stringLength-1] = '\0';
}

static void vtStamp(const CharMap *const charMap, const CharMap *const sprite, int spriteX, int spriteY, const bool maskMode) {
  vtNormalizePosition(charMap, sprite, spriteX, spriteY, spriteX, spriteY);
  if(maskMode) {
    for(unsigned int y = 0; y < sprite->height; y++) {
      for(unsigned int x = 0; x < sprite->width; x++) {
        if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < charMap->width && spriteY+y >= 0 && spriteY+y < charMap->height) {
          charMap->chars[(spriteX+x)+(spriteY+y)*charMap->width] = 1;
        }
      }
    }
  } else {
    for(unsigned int y = 0; y < sprite->height; y++) {
      for(unsigned int x = 0; x < sprite->width; x++) {
        if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < charMap->width && spriteY+y >= 0 && spriteY+y < charMap->height) {
          charMap->chars[(spriteX+x)+(spriteY+y)*charMap->width] = sprite->chars[x+y*sprite->width];
        }
      }
    }
  }
}

static void vtRender(const Canvas *const canvas, const unsigned int objLength, const Obj *const objs[const objLength]) {
  memcpy(canvas->mnaCanvas.chars, canvas->penCanvas.chars, vtSizeofChars(&canvas->mnaCanvas));
  for(unsigned int i = 0; i < objLength; i++) {
    if(objs[i]->visible) {
      vtStamp(&canvas->mnaCanvas, objs[i]->currentSprite, objs[i]->x, objs[i]->y, false);
    }
  }
}

static void vtLine(const CharMap *const canvas, const unsigned short penSize, const uint32_t penChar, int x0, int y0, const int x1, const int y1) {
  CharMap stroke;
  switch(penSize) {
    default:
    case 0:
      stroke = (CharMap){
          .width=1, .height=1,
          .chars=(uint32_t[1*1]){penChar}
      };
      break;
    case 1:
      stroke = (CharMap){
          .width=2, .height=2,
          .chars=(uint32_t[2*2]){penChar, penChar, penChar, penChar}
      };
      break;
    case 2:
      stroke = (CharMap){
          .width=4, .height=3,
          .chars=(uint32_t[4*3]){0, penChar, penChar, 0, penChar, penChar, penChar, penChar, 0, penChar, penChar, 0}
      };
      break;
    case 3:
      stroke = (CharMap){
          .width=5, .height=3,
          .chars=(uint32_t[5*3]){0, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, 0}
      };
      break;
    case 4:
      stroke = (CharMap){
          .width=7, .height=4,
          .chars=(uint32_t[7*4]){0, 0, penChar, penChar, penChar, 0, 0, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, 0, 0, penChar, penChar, penChar, 0, 0}
      };
      break;
    case 5:
      stroke = (CharMap){
          .width=8, .height=4,
          .chars=(uint32_t[8*4]){0, penChar, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, penChar, 0}
      };
      break;
  }
  int dx = abs(x1-x0), dy = abs(y1-y0);
  int t, err = (dx > dy ? dx : -dy)/2;
  const short stepX = x0 < x1 ? (short)1 : (short)-1, stepY = y0 < y1 ? (short)1 : (short)-1;
  while(1) {
    vtStamp(canvas, &stroke, x0, y0, false);
    if(x0 == x1 && y0 == y1) {
      break;
    }
    t = err;
    if(t > -dx) {
      err -= dy;
      x0 += stepX;
    }
    if(t < dy) {
      err += dx;
      y0 += stepY;
    }
  }
}

//----MISC----
void about() {
  printf("                 .:Visual T:.\n"
         "         ver ");
  puts("2.0.0  15 December 2019");
  puts("                 GNU LGPL-3.0\n"
       "\n"
       " A text-based graphic library written by Lucide\n"
       "\n"
       "       https://github.com/Lucide/VisualT\n"
       "\n"
       "\n"
       "             In Memory of Simba\n"
  );
}

//----INITIALIZATION----
void initializeCanvas(Canvas *const canvas, const unsigned int width, const unsigned int height) {
  if(width > 0 && height > 0) {
    canvas->border = true;
    canvas->mnaCanvas.chars = NULL;
    canvas->penCanvas.chars = NULL;
    resizeCanvas(canvas, width, height);
  }
}

void initializeArrayObj(Obj *const obj, const uint32_t *v) {
  CharMap *sprite;

  obj->length = v[0];
  obj->sprites = malloc(obj->length*sizeof(CharMap));
  for(unsigned int i = 0, j = 1; i < obj->length; i++) {
    sprite = &obj->sprites[i];
    vtInitializeCharMap(sprite, v[j], v[j+1]);
    j += 2;
    for(unsigned int y = 0; y < sprite->height; y++) {
      for(unsigned int x = 0; x < sprite->width; x++, j++) {
        sprite->chars[x+y*sprite->width] = vt32EndiannessSwap(v[j]);
      }
    }
  }
  vtInitializeObj(obj);
}

void initializeFileObj(Obj *const obj, const char path[const]) {
  unsigned int vLength = 0;
  uint32_t *v;
  FILE *const file = fopen(path, "r");

  if(file) {
    while(fscanf(file, "%*" SCNu32)) {
      vLength++;
    }
    v = malloc(vLength*sizeof(uint32_t));
    rewind(file);
    for(unsigned int i = 0; i < vLength; i++) {
      fscanf(file, "%" SCNu32, &v[i]); // NOLINT(cert-err34-c)
    }
    fclose(file);
    initializeArrayObj(obj, v);
    free(v);
  }
}

void initializeStringObj(Obj *const obj, const uint8_t *const utf8Text) {
  obj->length = 1;
  obj->sprites = malloc(sizeof(CharMap));
  vtInitializeStringCharMap(obj->sprites, utf8Text);
  vtInitializeObj(obj);
}

void deleteCanvas(const Canvas *const canvas) {
  free(canvas->mnaCanvas.chars);
  free(canvas->penCanvas.chars);
}

void deleteObj(const Obj *const obj) {
  for(unsigned int i = 0; i < obj->length; i++) {
    free(obj->sprites[i].chars);
  }
  free(obj->sprites);
}

void cloneCanvas(Canvas *const dest, const Canvas *const src) {
  memcpy(dest, src, sizeof(Canvas));
  vtInitializeCharMap(&dest->mnaCanvas, dest->mnaCanvas.width, dest->mnaCanvas.height);
  vtInitializeCharMap(&dest->penCanvas, dest->penCanvas.width, dest->penCanvas.height);
  memcpy(dest->penCanvas.chars, src->penCanvas.chars, vtSizeofChars(&dest->penCanvas));
}

void cloneObj(Obj *const dest, const Obj *const src) {
  CharMap *sprite;

  memcpy(dest, src, sizeof(Obj));
  dest->sprites = malloc(dest->length*sizeof(CharMap));
  for(unsigned int i = 0; i < dest->length; i++) {
    sprite = &dest->sprites[i];
    vtInitializeCharMap(sprite, src->sprites[i].width, src->sprites[i].height);
    memcpy(sprite->chars, src->sprites[i].chars, vtSizeofChars(sprite));
  }
  dest->currentSprite = &dest->sprites[src->currentSprite-src->sprites];
}

//----CANVAS----
void resizeCanvas(Canvas *const canvas, const unsigned int width, const unsigned int height) {
  if(width > 0 && height > 0) {
    free(canvas->mnaCanvas.chars);
    free(canvas->penCanvas.chars);
    vtInitializeCharMap(&canvas->mnaCanvas, width, height);
    vtInitializeCharMap(&canvas->penCanvas, width, height);
  }
  clearPen(canvas);
}

unsigned int canvasWidth(const Canvas *const canvas) {
  return canvas->mnaCanvas.width;
}

unsigned int canvasHeight(const Canvas *const canvas) {
  return canvas->mnaCanvas.height;
}

void canvasBorder(Canvas *const canvas, const bool border) {
  canvas->border = border;
}

//----REFRESH----
void draw(const Canvas *const canvas, const unsigned int objsLength, const Obj *const *const objs) {
  vtRender(canvas, objsLength, objs);
  vtPrintCharMap(&canvas->mnaCanvas, canvas->border);
}

void drawToString(const Canvas *const canvas, const unsigned int objsLength, const Obj *const *const objs, unsigned int *const stringLength, uint8_t **const utf8Text) {
  vtRender(canvas, objsLength, objs);
  vtPrintStringCharMap(&canvas->mnaCanvas, canvas->border, stringLength, utf8Text);
}

//----TEXT----
void setSpriteText(Obj *const obj, const uint8_t *const utf8Text) {
  free(obj->currentSprite->chars);
  vtInitializeStringCharMap(obj->currentSprite, utf8Text);
}

//----SPRITE----
unsigned int sprites(const Obj *const obj) {
  return obj->length;
}

unsigned int width(const Obj *const obj) {
  return obj->currentSprite->width;
}

unsigned int height(const Obj *const obj) {
  return obj->currentSprite->height;
}

void nextSprite(Obj *const obj) {
  obj->currentSprite = &(obj->sprites[(obj->currentSprite-obj->sprites+1)%obj->length]);
}

void precSprite(Obj *const obj) {
  obj->currentSprite = &(obj->sprites[(obj->currentSprite-obj->sprites+obj->length-1)%obj->length]);
}

void setSprite(Obj *const obj, const unsigned int sprite) {
  if(sprite < obj->length) {
    obj->currentSprite = &obj->sprites[sprite];
  }
}

//----OBJ----
bool visible(const Obj *const obj) {
  return obj->visible;
}

void show(Obj *const obj) {
  obj->visible = true;
}

void hide(Obj *const obj) {
  obj->visible = false;
}

void setVisible(Obj *const obj, const bool visible) {
  obj->visible = visible;
}

//----PEN----
bool isPen(const Obj *const obj) {
  return obj->pen;
}

uint32_t penChar(const Obj *const obj) {
  return obj->penChar;
}

unsigned short penSize(const Obj *const obj) {
  return obj->penSize;
}

void setPenSize(Obj *const obj, const unsigned short size) {
  obj->penSize = size < 7 ? size : (unsigned short)6;
}

void setPenChar(Obj *const obj, const uint32_t penChar) {
  obj->penChar = vt32EndiannessSwap(penChar);
}

void penUp(Obj *const obj) {
  obj->pen = false;
}

void penDown(Obj *const obj) {
  obj->pen = true;
}

void setPen(Obj *const obj, const bool pen) {
  obj->pen = pen;
}

void stamp(const Canvas *const canvas, const Obj *const obj) {
  vtStamp(&canvas->penCanvas, obj->currentSprite, obj->x, obj->y, false);
}

void penFill(const Canvas *const canvas, const uint32_t fillChar) {
  for(unsigned int i = canvas->penCanvas.width*canvas->penCanvas.height; i-- > 0;) {
    canvas->penCanvas.chars[i] = vt32EndiannessSwap(fillChar);
  }
}

void penShift(const Canvas *const canvas, const unsigned char direction) {
  const unsigned int width = canvas->penCanvas.width, height = canvas->penCanvas.height;
  switch(direction) {
    default:
    case 0: //LEFT
      for(unsigned int y = 0; y < height; y++) {
        for(unsigned int x = 0; x < width-1; x++) {
          canvas->penCanvas.chars[x+y*width] = canvas->penCanvas.chars[(x+1)+y*width];
        }
      }
      for(unsigned int y = 0; y < height; y++) {
        canvas->penCanvas.chars[(width-1)+y*width] = ' ';
      }
      break;
    case 1: //RIGHT
      for(unsigned int y = 0; y < height; y++) {
        for(unsigned int x = width-1; x > 0; x--) {
          canvas->penCanvas.chars[x+y*width] = canvas->penCanvas.chars[(x-1)+y*width];
        }
      }
      for(unsigned int y = 0; y < height; y++) {
        canvas->penCanvas.chars[0+y*width] = ' ';
      }
      break;
    case 2: //TOP
      for(unsigned int x = 0; x < width; x++) {
        for(unsigned int y = 0; y < height-1; y++) {
          canvas->penCanvas.chars[x+y*width] = canvas->penCanvas.chars[x+(y+1)*width];
        }
      }
      for(unsigned int x = 0; x < width; x++) {
        canvas->penCanvas.chars[x+(height-1)*width] = ' ';
      }
      break;
    case 3: //BOTTOM
      for(unsigned int x = 0; x < width; x++) {
        for(unsigned int y = height-1; y > 0; y--) {
          canvas->penCanvas.chars[x+y*width] = canvas->penCanvas.chars[x+(y-1)*width];
        }
      }
      for(unsigned int x = 0; x < width; x++) {
        canvas->penCanvas.chars[x+0*width] = ' ';
      }
      break;
  }
}

void printAxes(const Canvas *const canvas) {
  uint32_t *const chars = canvas->penCanvas.chars;
  const unsigned int width = canvas->penCanvas.width, height = canvas->penCanvas.height;

  for(unsigned int i = 0; i < width; i++) {
    chars[(i)+(height/2)*width] = '-';
  }
  for(unsigned int i = 0; i < height; i++) {
    chars[(width/2)+(i)*width] = '|';
  }
  chars[(0)+(height/2)*width] = '-';
  chars[(width/2)+(0)*width] = '+';
  chars[(width-1)+(height/2)*width] = '+';
  chars[(width/2)+(height-1)*width] = '-';
  chars[(width/2)+(height/2)*width] = '0';
}

void clearPen(const Canvas *const canvas) {
  for(unsigned int i = canvas->penCanvas.width*canvas->penCanvas.height; i-- > 0;) {
    canvas->penCanvas.chars[i] = (uint32_t)32;
  }
}

//----MOVE----
int xPostion(const Obj *const obj) {
  return obj->x;
}

int yPosition(const Obj *const obj) {
  return obj->y;
}

void moveTo(const Canvas *const canvas, Obj *const obj, const int x, const int y) {
  if(obj->pen) {
    vtLine(&canvas->penCanvas, obj->penSize, obj->penChar, obj->x, obj->y, x, y);
  }
  obj->x = x;
  obj->y = y;
}

void setX(const Canvas *const canvas, Obj *const obj, int x) {
  if(obj->pen) {
    vtLine(&canvas->penCanvas, obj->penSize, obj->penChar, obj->x, obj->y, x, obj->y);
  }
  obj->x = x;
}

void setY(const Canvas *const canvas, Obj *const obj, int y) {
  if(obj->pen) {
    vtLine(&canvas->penCanvas, obj->penSize, obj->penChar, obj->x, obj->y, obj->x, y);
  }
  obj->y = y;
}

void changeX(const Canvas *const canvas, Obj *const obj, const int x) {
  if(obj->pen) {
    vtLine(&canvas->penCanvas, obj->penSize, obj->penChar, obj->x, obj->y, obj->x+x, obj->y);
  }
  obj->x += x;
}

void changeY(const Canvas *const canvas, Obj *const obj, const int y) {
  if(obj->pen) {
    vtLine(&canvas->penCanvas, obj->penSize, obj->penChar, obj->x, obj->y, obj->x, obj->y-y);
  }
  obj->y -= y;
}

void align(Obj *const obj, const unsigned char position) {
  switch(position) {
    default:
    case 0: //top-left
      obj->x += (int)(obj->currentSprite->width/2);
      obj->y += (int)(obj->currentSprite->height/2);
      break;
    case 1: //top-right
      obj->x -= (int)(obj->currentSprite->width/2);
      obj->y += (int)(obj->currentSprite->height/2);
      if(obj->currentSprite->width%2 == 0) {
        obj->x++;
      }
      break;
    case 2: //bottom-left
      obj->x += (int)(obj->currentSprite->width/2);
      obj->y -= (int)(obj->currentSprite->height/2);
      if(obj->currentSprite->height%2 == 0) {
        obj->y--;
      }
      break;
    case 3: //bottom-right
      obj->x -= (int)(obj->currentSprite->width/2);
      obj->y -= (int)(obj->currentSprite->height/2);
      if(obj->currentSprite->width%2 == 0) {
        obj->x--;
      }
      if(obj->currentSprite->height%2 == 0) {
        obj->y--;
      }
      break;
  }
}

bool isTouching(const Canvas *const canvas, const Obj *const obj, unsigned int objsLength, const struct Obj *const *const objs) {
  const CharMap *const mnaCanvas = &canvas->mnaCanvas, *const sprite = obj->currentSprite;

  if(obj->visible) {
    int spriteX, spriteY;
    memset(mnaCanvas->chars, '\0', mnaCanvas->width*mnaCanvas->height*sizeof(uint32_t));
    for(unsigned int i = 0; i < objsLength; i++) {
      if(objs[i]->visible) {
        vtStamp(mnaCanvas, objs[i]->currentSprite, objs[i]->x, objs[i]->y, true);
      }
    }
    vtNormalizePosition(mnaCanvas, sprite, obj->x, obj->y, spriteX, spriteY);
    for(unsigned int y = 0; y < sprite->height; y++) {
      for(unsigned int x = 0; x < sprite->width; x++) {
        if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < mnaCanvas->width && spriteY+y >= 0 && spriteY+y < mnaCanvas->height && !mnaCanvas->chars[(spriteX+x)+(spriteY+y)*mnaCanvas->width]) {
          return true;
        }
      }
    }
  }
  return false;
}

bool isTouchingChar(const Canvas *const canvas, const Obj *const obj, uint32_t character) {
  const CharMap *const mnaCanvas = &canvas->mnaCanvas, *const sprite = obj->currentSprite;

  if(obj->visible) {
    int spriteX, spriteY;
    character = vt32EndiannessSwap(character);
    memcpy(mnaCanvas->chars, canvas->penCanvas.chars, vtSizeofChars(mnaCanvas));
    vtNormalizePosition(mnaCanvas, sprite, obj->x, obj->y, spriteX, spriteY);
    for(unsigned int y = 0; y < sprite->height; y++) {
      for(unsigned int x = 0; x < sprite->width; x++) {
        if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < mnaCanvas->width && spriteY+y >= 0 && spriteY+y < mnaCanvas->height && mnaCanvas->chars[(spriteX+x)+(spriteY+y)*mnaCanvas->width] == character) {
          return true;
        }
      }
    }
  }
  return false;
}

bool isOutside(const Canvas *const canvas, const Obj *const obj) {
  const CharMap *const mnaCanvas = &canvas->mnaCanvas, *const sprite = obj->currentSprite;

  if(obj->visible) {
    int spriteX, spriteY;
    memcpy(mnaCanvas->chars, canvas->penCanvas.chars, vtSizeofChars(mnaCanvas));
    vtNormalizePosition(mnaCanvas, sprite, obj->x, obj->y, spriteX, spriteY);
    for(unsigned int y = 0; y < sprite->height; y++) {
      for(unsigned int x = 0; x < sprite->width; x++) {
        if(sprite->chars[x+y*sprite->width] && !(spriteX+x >= 0 && spriteX+x < mnaCanvas->width && spriteY+y >= 0 && spriteY+y < mnaCanvas->height)) {
          return true;
        }
      }
    }
  }
  return false;
}
