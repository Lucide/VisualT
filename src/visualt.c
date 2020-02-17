#include "visualt/visualt.h"
#include "visualt/version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

typedef struct vtCharMap CharMap;

typedef struct vtObj Obj;

typedef struct BoolMap {
	bool *chars;
	unsigned int width, height;
} BoolMap;

//----INTERNALs----
#define lputs(STRING) fputs((STRING), stdout)

#define puts32(VTCHAR, LENGTH) fwrite((VTCHAR), sizeof(VTChar), (LENGTH), stdout);

#define putchar32(VTCHAR) putchar((VTCHAR)>>0&0xffu);   \
                            putchar((VTCHAR)>>8&0xffu);   \
                            putchar((VTCHAR)>>16&0xffu);  \
                            putchar((VTCHAR)>>24&0xffu)

#define initializeObj(OBJ)  (OBJ)->x = 0;           \
                              (OBJ)->y = 0;           \
                              (OBJ)->visible = true;  \
                              (OBJ)->penSize = 0;     \
                              (OBJ)->penChar = '#';   \
                              (OBJ)->currentSprite = (OBJ)->sprites

#define initializeCharMap(CHARMAP, WIDTH, HEIGHT) (CHARMAP)->width = (WIDTH);   \
                                                    (CHARMAP)->height = (HEIGHT); \
                                                    (CHARMAP)->chars = malloc(sizeofChars(CHARMAP))

#define freeCharMap(CHARMAP) free((CHARMAP)->chars)

#define endiannessSwap32(VTCHAR) ((((VTCHAR)>>24u)&0xffu)|(((VTCHAR)>>8u)&0xff00u)|(((VTCHAR)<<8u)&0xff0000u)|(((VTCHAR)<<24u)&0xff000000u))

#define sizeofChars(CHARMAP) ((CHARMAP)->width*(CHARMAP)->height*sizeof(VTChar))

#define clearCharMap(CHARMAP) memset((CHARMAP)->chars, 0, sizeofChars((CHARMAP)))

#define normalizePosition(CHARMAP, SPRITE, SPRITEX, SPRITEY, X, Y)  (X) = (int)((CHARMAP)->width/2-(SPRITE)->width/2+(SPRITEX));  \
                                                                      (Y) = (int)((CHARMAP)->height/2-(SPRITE)->height/2-(SPRITEY))

static void initializeStringCharMap(CharMap *const charMap, const uint8_t *const utf8Text) {
	unsigned int charMapWidth = 1, charMapHeight = 1;
	uint8_t *t;

	//count size
	for(unsigned int width = 0, i = 0; utf8Text[i] != '\0'; i++) {
		if((utf8Text[i]&0xc0u) != 0x80u) { // partial code point: 10xx xxxx
			if(utf8Text[i] == '\n') {
				charMapHeight++;
				width = 0;
			} else {
				width++;
				if(width > charMapWidth) {
					charMapWidth = width;
				}
			}
		}
	}
	initializeCharMap(charMap, charMapWidth, charMapHeight);
	//parse
	for(unsigned int i = 0, y = 0; y < charMapHeight; i++, y++) {
		for(unsigned int x = 0; x < charMapWidth; x++) {
			charMap->chars[x+y*charMapWidth] = 0;
			t = (uint8_t *)&charMap->chars[x+y*charMapWidth];
			if(utf8Text[i] >= 0xf0) {         //4B code point: 1111 0xxx
				t[0] = utf8Text[i++];
				t[1] = utf8Text[i++];
				t[2] = utf8Text[i++];
				t[3] = utf8Text[i++];
			} else if(utf8Text[i] >= 0xe0) {  //3B code point: 1110 xxxx
				t[0] = utf8Text[i++];
				t[1] = utf8Text[i++];
				t[2] = utf8Text[i++];
			} else if(utf8Text[i] >= 0xc0) {  //2B code point: 110x xxxx
				t[0] = utf8Text[i++];
				t[1] = utf8Text[i++];
			} else {                          //1B code point: 0xxx xxxx
				if(utf8Text[i] != '\n' && utf8Text[i] != '\0') {
					if(utf8Text[i] != '\v') {
						t[0] = utf8Text[i++];
					} else {
						t[0] = utf8Text[i++]*0;
					}
				} else {
					for(; x < charMapWidth; x++) {
						charMap->chars[x+y*charMapWidth] = 0;
					}
				}
			}
		}
	}
}

#ifdef VISUALT_UNBUFFERED_PRINT

static void printCharMap(const CharMap *const charMap, const bool border) {
	const unsigned int width = charMap->width, height = charMap->height;
	const VTChar *const chars = charMap->chars;

	if(border) {
		lputs("┌");
		for(unsigned int x = 0; x < width-1; x++) {
			lputs("─");
		}
		lputs("─┐\n");
		for(unsigned int y = 0; y < width*height; y += width) {
			lputs("│");
			vt32Puts(&chars[0+y], width);
			lputs("│\n");
		}
		lputs("└");
		for(unsigned int x = 0; x < width-1; x++) {
			lputs("─");
		}
		lputs("─┘\n");
	} else {
		for(unsigned int y = 0; y < width*height; y += width) {
			lputs("│");
			vt32Puts(&chars[0+y], width);
			lputs("│\n");
		}
	}
}

#else

static void printCharMap(const CharMap *const charMap, const bool border) {
	const unsigned int width = charMap->width, height = charMap->height;
	const VTChar *const chars = charMap->chars;
	const uint8_t *t;
	uint8_t *const buffer = malloc(sizeof(uint8_t)+width*sizeof(VTChar));

	if(border) {
		lputs("┌");
		for(unsigned int x = 0; x < width-1; x++) {
			lputs("─");
		}
		lputs("─┐\n");
		for(unsigned y = 0; y < height; y++) {
			for(unsigned int i = 0, x = 0; x < width; x++) {
				t = (uint8_t *)&chars[x+y*width];
				if(t[0] >= 0xf0) {  //4B code point: 1111 0xxx
					buffer[i++] = t[0];
					buffer[i++] = t[1];
					buffer[i++] = t[2];
					buffer[i++] = t[3];
				} else if(t[0] >= 0xe0) { //3B code point: 1110 xxxx
					buffer[i++] = t[0];
					buffer[i++] = t[1];
					buffer[i++] = t[2];
				} else if(t[0] >= 0xc0) { //2B code point: 110x xxxx
					buffer[i++] = t[0];
					buffer[i++] = t[1];
				} else {  //1B code point: 0xxx xxxx
					if(t[0] != 0) {
						buffer[i++] = t[0];
					} else {
						buffer[i++] = ' ';
					}
				}
				buffer[i] = '\0';
			}
			lputs("│");
			lputs((char *)buffer);
			lputs("│\n");
		}
		lputs("└");
		for(unsigned int x = 0; x < width-1; x++) {
			lputs("─");
		}
		lputs("─┘\n");
	} else {
		for(unsigned int y = 0; y < height; y++) {
			for(unsigned int i = 0, x = 0; x < width; x++) {
				t = (uint8_t *)&chars[x+y*width];
				if(t[0] >= 0xf0) {  //4B code point: 1111 0xxx
					buffer[i++] = t[0];
					buffer[i++] = t[1];
					buffer[i++] = t[2];
					buffer[i++] = t[3];
				} else if(t[0] >= 0xe0) { //3B code point: 1110 xxxx
					buffer[i++] = t[0];
					buffer[i++] = t[1];
					buffer[i++] = t[2];
				} else if(t[0] >= 0xc0) { //2B code point: 110x xxxx
					buffer[i++] = t[0];
					buffer[i++] = t[1];
				} else {  //1B code point: 0xxx xxxx
					buffer[i++] = t[0];
				}
				buffer[i] = '\0';
			}
			puts((char *)buffer);
		}
	}
	free(buffer);
}

#endif

static unsigned int printStringCharMap(const CharMap *const charMap, const bool border, uint8_t **const utf8String) {
	const unsigned int width = charMap->width, height = charMap->height;
	const VTChar *const chars = charMap->chars;
	const uint8_t *t;
	unsigned int length = 0, i = 0;

	for(unsigned int y = 0; y < height; length++, y++) {
		for(unsigned int x = 0; x < width; x++) {
			t = (uint8_t *)&chars[x+y*width];
			if(t[0] >= 0xf0) {        //4B code point: 1111 0xxx
				length += 4;
			} else if(t[0] >= 0xe0) { //3B code point: 1110 xxxx
				length += 3;
			} else if(t[0] >= 0xc0) { //2B code point: 110x xxxx
				length += 2;
			} else {               //1B code point: 0xxx xxxx
				length += 1;
			}
		}
	}
	if(border) {
		length += (3*((width+2)*2+(2*height)) //first and last row + │ for every line
				+2 //first \n and last \0
		)*sizeof(uint8_t);
	}

	*utf8String = malloc(length*sizeof(uint8_t));
	uint8_t *const string = *utf8String;

	if(border) {
		i = length-2;
		string[i--] = 0x98u;  //┘
		string[i--] = 0x94u;  //┘
		string[i--] = 0xe2u;  //┘
		for(unsigned int j = 0; j < width; j++) {
			string[i--] = 0x80u;  //─
			string[i--] = 0x94u;  //─
			string[i--] = 0xe2u;  //─
		}
		string[i--] = 0x94u;  //└
		string[i--] = 0x94u;  //└
		string[i] = 0xe2u;    //└
		i = 0;
		string[i++] = 0xe2u;  //┌
		string[i++] = 0x94u;  //┌
		string[i++] = 0x8cu;  //┌
		for(unsigned int j = 0; j < width; j++) {
			string[i++] = 0xe2u;  //─
			string[i++] = 0x94u;  //─
			string[i++] = 0x80u;  //─
		}
		string[i++] = 0xe2u;  //┐
		string[i++] = 0x94u;  //┐
		string[i++] = 0x90u;  //┐
		string[i++] = '\n';
	}
	for(unsigned int y = 0; y < height; y++) {
		if(border) {
			string[i++] = 0xe2u;  //│
			string[i++] = 0x94u;  //│
			string[i++] = 0x82u;  //│
		}
		for(unsigned int x = 0; x < width; x++) {
			t = (uint8_t *)&chars[x+y*width]; // t = chars[x+y*width];
			if(t[0] >= 0xf0) { //4B code point: 1111 0xxx
				string[i++] = t[0];
				string[i++] = t[1];
				string[i++] = t[2];
				string[i++] = t[3];
			} else if(t[0] >= 0xe0) { //3B code point: 1110 xxxx
				string[i++] = t[0];
				string[i++] = t[1];
				string[i++] = t[2];
			} else if(t[0] >= 0xc0) { //2B code point: 110x xxxx
				string[i++] = t[0];
				string[i++] = t[1];
			} else { //1B code point: 0xxx xxxx
				if(t[0] != 0) {
					string[i++] = t[0];
				} else {
					string[i++] = ' ';
				}
			}
		}
		if(border) {
			string[i++] = 0xe2u;  //│
			string[i++] = 0x94u;  //│
			string[i++] = 0x82u;  //│
		}
		string[i++] = '\n';
	}
	string[length-1] = '\0';
	return length;
}

static void stamp(const CharMap *const charMap, const CharMap *const sprite, int spriteX, int spriteY) {
	normalizePosition(charMap, sprite, spriteX, spriteY, spriteX, spriteY);
	for(unsigned int y = 0; y < sprite->height; y++) {
		for(unsigned int x = 0; x < sprite->width; x++) {
			if(sprite->chars[x+y*sprite->width] && spriteX+(int)x >= 0 && spriteX+(int)x < (int)charMap->width && spriteY+(int)y >= 0 && spriteY+(int)y < (int)charMap->height) {
				charMap->chars[(spriteX+x)+(spriteY+y)*charMap->width] = sprite->chars[x+y*sprite->width];
			}
		}
	}
}

static void mask(const BoolMap *const boolMap, const CharMap *const sprite, int spriteX, int spriteY) {
	normalizePosition(boolMap, sprite, spriteX, spriteY, spriteX, spriteY);
	for(unsigned int y = 0; y < sprite->height; y++) {
		for(unsigned int x = 0; x < sprite->width; x++) {
			if(sprite->chars[x+y*sprite->width] && spriteX+(int)x >= 0 && spriteX+(int)x < (int)boolMap->width && spriteY+(int)y >= 0 && spriteY+(int)y < (int)boolMap->height) {
				boolMap->chars[(spriteX+x)+(spriteY+y)*boolMap->width] = true;
			}
		}
	}
}

static void render(const CharMap *const charMap, const unsigned int objsLength, const Obj *const *const objs, bool clearCharmap) {
	if(clearCharmap) {
		clearCharMap(charMap);
	}
	for(unsigned int i = 0; i < objsLength; i++) {
		if(objs[i]->visible) {
			stamp(charMap, objs[i]->currentSprite, objs[i]->x, objs[i]->y);
		}
	}
}

static void line(const CharMap *const canvas, const unsigned short penSize, const VTChar penChar, int x0, int y0, const int x1, const int y1) {
	CharMap stroke;
	switch(penSize) {
		default:
		case 0:
			stroke = (CharMap){.width=1, .height=1, .chars=(VTChar[1*1]){penChar}};
			break;
		case 1:
			stroke = (CharMap){.width=2, .height=2, .chars=(VTChar[2*2]){penChar, penChar, penChar, penChar}};
			break;
		case 2:
			stroke = (CharMap){.width=4, .height=3, .chars=(VTChar[4*3]){0, penChar, penChar, 0, penChar, penChar, penChar, penChar, 0, penChar, penChar, 0}};
			break;
		case 3:
			stroke = (CharMap){.width=5, .height=3, .chars=(VTChar[5*3]){0, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, 0}};
			break;
		case 4:
			stroke = (CharMap){.width=7, .height=4, .chars=(VTChar[7*4]){0, 0, penChar, penChar, penChar, 0, 0, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, 0, 0, penChar, penChar, penChar, 0, 0}};
			break;
		case 5:
			stroke = (CharMap){.width=8, .height=4, .chars=(VTChar[8*4]){0, penChar, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, penChar, 0}};
			break;
	}
	int dx = abs(x1-x0), dy = abs(y1-y0);
	int t, err = (dx > dy ? dx : -dy)/2;
	const short stepX = x0 < x1 ? (short)1 : (short)-1, stepY = y0 < y1 ? (short)1 : (short)-1;
	while(1) {
		stamp(canvas, &stroke, x0, y0);
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
void vtAbout() {
	puts("                 .:Visual T:.\n"
			 "         ver " VISUALT_VERSION "  15 December 2019\n"
			 "                 GNU LGPL-3.0\n"
			 "\n"
			 " A text-based graphic library written by Lucide\n"
			 "       https://github.com/Lucide/VisualT\n"
			 "\n"
			 "                                \\    /\\\n"
			 "       In Memory of Simba        )  ( ')\n"
			 "                                (  /  )\n"
			 "                           jgs   \\(__)|\n");
}

//----INITIALIZATION----
void vtInitializeBlank(Obj *const obj, const unsigned int sizesLength, const VTSizes sizes) {
	assert(sizesLength > 0);

	obj->length = sizesLength;
	obj->sprites = malloc(sizesLength*sizeof(CharMap));
	for(unsigned int i = 0; i < sizesLength; i++) {
		assert(sizes[i][0] > 0);
		assert(sizes[i][1] > 0);
		initializeCharMap(&obj->sprites[i], sizes[i][0], sizes[i][1]);
		clearCharMap(&obj->sprites[i]);
	}
	initializeObj(obj);
}

void vtInitializeArray(Obj *const obj, const VTChar *const v) {
	CharMap *sprite;

	obj->length = v[0];
	obj->sprites = malloc(obj->length*sizeof(CharMap));
	for(unsigned int i = 0, j = 1; i < obj->length; i++) {
		sprite = &obj->sprites[i];
		initializeCharMap(sprite, v[j], v[j+1]);
		j += 2;
		for(unsigned int y = 0; y < sprite->height; y++) {
			for(unsigned int x = 0; x < sprite->width; x++, j++) {
				sprite->chars[x+y*sprite->width] = v[j]; // vt32EndiannessSwap(v[j]);
			}
		}
	}
	initializeObj(obj);
}

void vtInitializeFile(Obj *const obj, const char path[const]) {
	unsigned int vLength = 0;
	VTChar *v;
	FILE *const file = fopen(path, "r");

	if(file) {
		while(fscanf(file, "%*" SCNu32) != EOF) { // NOLINT(cert-err34-c)
			vLength++;
		}
		v = malloc(vLength*sizeof(VTChar));
		rewind(file);
		for(unsigned int i = 0; i < vLength; i++) {
			fscanf(file, "%" SCNu32, &v[i]); // NOLINT(cert-err34-c)
		}
		fclose(file);

		// for(unsigned int i = 0; i < vLength; i++) {
		// 	printf("%" PRIu32 " ", vt32EndiannessSwap(v[i]));
		// }
		// putchar('\n');

		vtInitializeArray(obj, v);
		free(v);
	}
}

void vtInitializeString(Obj *const obj, unsigned int utf8StringsLength, const VTStrs utf8Strings) {
	assert(utf8StringsLength > 0);

	obj->length = utf8StringsLength;
	obj->sprites = malloc(utf8StringsLength*sizeof(CharMap));
	for(unsigned int i = 0; i < utf8StringsLength; i++) {
		initializeStringCharMap(&obj->sprites[i], utf8Strings[i]);
	}
	initializeObj(obj);
}

void vtInitializeObj(Obj *const obj, const Obj *const src) {
	CharMap *sprite;

	*obj = *src;
	obj->sprites = malloc(obj->length*sizeof(CharMap));
	for(unsigned int i = 0; i < obj->length; i++) {
		sprite = &obj->sprites[i];
		initializeCharMap(sprite, src->sprites[i].width, src->sprites[i].height);
		memcpy(sprite->chars, src->sprites[i].chars, sizeofChars(sprite));
	}
	obj->currentSprite = &obj->sprites[src->currentSprite-src->sprites];
}

void vtRelease(const unsigned int objsLength, const VTObjs objs) {
	for(unsigned int i = 0; i < objsLength; i++) {
		for(unsigned int j = 0; j < objs[i]->length; j++) {
			freeCharMap(&objs[i]->sprites[j]);
		}
		free(objs[i]->sprites);
	}
}

void vtCloneSprite(const Obj *const dest, const unsigned int spriteDest, const Obj *const src, const unsigned int spriteSrc) {
	assert(spriteSrc < src->length);
	assert(spriteDest < dest->length);

	freeCharMap(&dest->sprites[spriteDest]);
	dest->sprites[spriteDest] = src->sprites[spriteSrc];
	initializeCharMap(&dest->sprites[spriteDest], dest->sprites[spriteDest].width, dest->sprites[spriteDest].height);
	memcpy(dest->sprites[spriteDest].chars, src->sprites[spriteSrc].chars, sizeofChars(&dest->sprites[spriteDest]));
}

void vtResize(Obj *const obj, const unsigned int width, const unsigned int height) {
	assert(width > 0);
	assert(height > 0);

	freeCharMap(obj->currentSprite);
	initializeCharMap(obj->currentSprite, width, height);
	vtClear(obj);
}

//----REFRESH----
void vtRender(const Obj *const canvas, const unsigned int objsLength, const VTObjs objs) {
	render(canvas->currentSprite, objsLength, objs, true);
}

void vtStamp(const Obj *const canvas, const unsigned int objsLength, const VTObjs objs) {
	render(canvas->currentSprite, objsLength, objs, false);
}

void vtPrint(const Obj *const canvas, const bool border) {
	printCharMap(canvas->currentSprite, border);
}

unsigned int vtPrintToString(const Obj *const canvas, const bool border, uint8_t **const utf8String) {
	return printStringCharMap(canvas->currentSprite, border, utf8String);
}

//----SPRITE----
unsigned int vtSprites(const Obj *const obj) {
	return obj->length;
}

unsigned int vtSpriteInd(const Obj *const obj) {
	return (unsigned int)(obj->currentSprite-obj->sprites);
}

void vtNextSprite(Obj *const obj) {
	obj->currentSprite = &(obj->sprites[(obj->currentSprite-obj->sprites+1)%obj->length]);
}

void vtPrecSprite(Obj *const obj) {
	obj->currentSprite = &(obj->sprites[(obj->currentSprite-obj->sprites+obj->length-1)%obj->length]);
}

void vtSetSprite(Obj *const obj, const unsigned int sprite) {
	assert(sprite < obj->length);

	obj->currentSprite = &obj->sprites[sprite];
}

unsigned int vtWidth(const Obj *const obj) {
	return obj->currentSprite->width;
}

unsigned int vtHeight(const Obj *const obj) {
	return obj->currentSprite->height;
}

void vtSetText(Obj *const obj, const VTStr utf8Text) {
	freeCharMap(obj->currentSprite);
	initializeStringCharMap(obj->currentSprite, utf8Text);
}

void vtClear(const Obj *const canvas) {
	clearCharMap(canvas->currentSprite);
}

void vtFill(const Obj *canvas, const VTChar fillChar) {
	for(unsigned int i = canvas->currentSprite->width*canvas->currentSprite->height; i-- > 0;) {
		canvas->currentSprite->chars[i] = fillChar;
	}
}

void vtOverlay(const Obj *const dest, const unsigned int spriteDest, const Obj *const src, const unsigned int spriteSrc) {
	assert(spriteSrc < src->length);
	assert(spriteDest < dest->length);
	assert(src->sprites[spriteSrc].width == dest->sprites[spriteDest].width);
	assert(src->sprites[spriteSrc].height == dest->sprites[spriteDest].height);

	for(unsigned int i = src->sprites[spriteSrc].width*src->sprites[spriteSrc].height; i-- > 0;) {
		if(src->sprites[spriteSrc].chars[i]) {
			dest->sprites[spriteDest].chars[i] = src->sprites[spriteSrc].chars[i];
		}
	}
}

void vtPrintAxes(const Obj *const canvas) {
	VTChar *const chars = canvas->currentSprite->chars;
	const unsigned int width = canvas->currentSprite->width, height = canvas->currentSprite->height;

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

//----OBJ----
bool vtVisible(const Obj *const obj) {
	return obj->visible;
}

void vtShow(Obj *const obj) {
	obj->visible = true;
}

void vtHide(Obj *const obj) {
	obj->visible = false;
}

void vtSetVisibility(Obj *const obj, const bool visible) {
	obj->visible = visible;
}

//----PEN----
VTChar vtPenChar(const Obj *const obj) {
	return obj->penChar;
}

unsigned short vtPenSize(const Obj *const obj) {
	return obj->penSize;
}

void vtSetPenSize(Obj *const obj, const unsigned short size) {
	obj->penSize = size <= 5 ? size : (unsigned short)5;
}

void vtSetPenChar(Obj *const obj, const VTChar penChar) {
	obj->penChar = penChar;
}

//----MOVE----
int vtXPosition(const Obj *const obj) {
	return obj->x;
}

int vtYPosition(const Obj *const obj) {
	return obj->y;
}

void vtGotoXY(const Obj *const canvas, Obj *const obj, const int x, const int y) {
	if(canvas) {
		line(canvas->currentSprite, obj->penSize, obj->penChar, obj->x, obj->y, x, y);
	}
	obj->x = x;
	obj->y = y;
}

void vtGotoX(const Obj *const canvas, Obj *const obj, const int x) {
	if(canvas) {
		line(canvas->currentSprite, obj->penSize, obj->penChar, obj->x, obj->y, x, obj->y);
	}
	obj->x = x;
}

void vtGotoY(const Obj *const canvas, Obj *const obj, const int y) {
	if(canvas) {
		line(canvas->currentSprite, obj->penSize, obj->penChar, obj->x, obj->y, obj->x, y);
	}
	obj->y = y;
}

void vtChangeX(const Obj *const canvas, Obj *const obj, const int x) {
	if(canvas) {
		line(canvas->currentSprite, obj->penSize, obj->penChar, obj->x, obj->y, obj->x+x, obj->y);
	}
	obj->x += x;
}

void vtChangeY(const Obj *const canvas, Obj *const obj, const int y) {
	if(canvas) {
		line(canvas->currentSprite, obj->penSize, obj->penChar, obj->x, obj->y, obj->x, obj->y-y);
	}
	obj->y += y;
}

void vtAlign(Obj *const obj, const unsigned char position) {
	switch(position) {
		default:
			return;
		case 0: //top-left
			obj->x += (int)(obj->currentSprite->width/2);
			obj->y -= (int)(obj->currentSprite->height/2);
			break;
		case 1: //top-right
			obj->x -= (int)(obj->currentSprite->width/2);
			obj->y -= (int)(obj->currentSprite->height/2);
			if(obj->currentSprite->width%2 == 0) {
				obj->x++;
			}
			break;
		case 2: //bottom-left
			obj->x += (int)(obj->currentSprite->width/2);
			obj->y += (int)(obj->currentSprite->height/2);
			if(obj->currentSprite->height%2 == 0) {
				obj->y--;
			}
			break;
		case 3: //bottom-right
			obj->x -= (int)(obj->currentSprite->width/2);
			obj->y += (int)(obj->currentSprite->height/2);
			if(obj->currentSprite->width%2 == 0) {
				obj->x++;
			}
			if(obj->currentSprite->height%2 == 0) {
				obj->y--;
			}
			break;
	}
}

bool vtIsTouching(const Obj *const canvas, const Obj *const obj, unsigned int objsLength, const VTObjs objs) {
	const CharMap *const sprite = obj->currentSprite;
	int spriteX, spriteY;

	if(obj->visible) {
		const BoolMap boolMap = (BoolMap){malloc(canvas->currentSprite->width*canvas->currentSprite->height*sizeof(bool)), canvas->currentSprite->width, canvas->currentSprite->height};
		memset(boolMap.chars, false, sizeofChars(&boolMap)/sizeof(VTChar)*sizeof(bool));
		for(unsigned int i = 0; i < objsLength; i++) {
			if(objs[i]->visible) {
				mask(&boolMap, objs[i]->currentSprite, objs[i]->x, objs[i]->y);
			}
		}
		normalizePosition(&boolMap, sprite, obj->x, obj->y, spriteX, spriteY);
		for(unsigned int y = 0; y < sprite->height; y++) {
			for(unsigned int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && spriteX+(int)x >= 0 && spriteX+(int)x < (int)boolMap.width && spriteY+(int)y >= 0 && spriteY+(int)y < (int)boolMap.height && boolMap.chars[(spriteX+x)+(spriteY+y)*boolMap.width]) {
					freeCharMap(&boolMap);
					return true;
				}
			}
		}
		freeCharMap(&boolMap);
	}
	return false;
}

bool vtIsTouchingChar(const Obj *const canvas, const Obj *const obj, const VTChar character) {
	const CharMap *const charMap = canvas->currentSprite, *const sprite = obj->currentSprite;
	int spriteX, spriteY;

	if(obj->visible) {
		normalizePosition(charMap, sprite, obj->x, obj->y, spriteX, spriteY);
		for(unsigned int y = 0; y < sprite->height; y++) {
			for(unsigned int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && spriteX+(int)x >= 0 && spriteX+(int)x < (int)charMap->width && spriteY+(int)y >= 0 && spriteY+(int)y < (int)charMap->height && charMap->chars[(spriteX+x)+(spriteY+y)*charMap->width] == character) {
					return true;
				}
			}
		}
	}
	return false;
}

bool vtIsOutside(const Obj *const canvas, const Obj *const obj) {
	const CharMap *const charMap = canvas->currentSprite, *const sprite = obj->currentSprite;
	int spriteX, spriteY;

	if(obj->visible) {
		normalizePosition(charMap, sprite, obj->x, obj->y, spriteX, spriteY);
		for(unsigned int y = 0; y < sprite->height; y++) {
			for(unsigned int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && !(spriteX+(int)x >= 0 && spriteX+(int)x < (int)charMap->width && spriteY+(int)y >= 0 && spriteY+(int)y < (int)charMap->height)) {
					return true;
				}
			}
		}
	}
	return false;
}
