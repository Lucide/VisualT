#include "visualt/visualt.h"
#include "version.h"
#include "buildDate.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

typedef struct VTGlyphMap {
	VTChar *restrict chars;
	int width, height;
} GlyphMap;

typedef VTObj Obj;

typedef struct BoolMap {
	bool *restrict chars;
	int width, height;
} BoolMap;

//----INTERNALS----
#define lputs(STRING) fputs((STRING), stdout)

#define puts32(VTCHAR, LENGTH) fwrite((VTCHAR), sizeof(VTChar), (LENGTH), stdout)

#define putchar32(VTCHAR) putchar((char)((VTCHAR)>>0u&0xffu));   \
                            putchar((char)((VTCHAR)>>8u&0xffu));   \
                            putchar((char)((VTCHAR)>>16u&0xffu));  \
                            putchar((char)((VTCHAR)>>24u&0xffu))

#define endiannessSwap32(VTCHAR) ((((VTCHAR)>>24u)&0xffu)|((VTCHAR)>>8u&0xff00u)|((VTCHAR)<<8u&0xff0000u)|((VTCHAR)<<24u&0xff000000u))

#define initializeObj(OBJ)  (OBJ)->x = 0;           \
                              (OBJ)->y = 0;           \
                              (OBJ)->visible = true;  \
                              (OBJ)->penSize = 0;     \
                              (OBJ)->penChar = '#';   \
                              (OBJ)->activeSprite = (OBJ)->sprites

#define sizeofGlyphs(GLYPHMAP) ((size_t)(GLYPHMAP)->width*(size_t)(GLYPHMAP)->height*sizeof(VTChar))

#define sizeofBools(BOOLMAP) ((size_t)(BOOLMAP)->width*(size_t)(BOOLMAP)->height*sizeof(bool))

#define clearMap(TYPE, MAP) memset((MAP)->chars, 0, sizeof##TYPE((MAP)))

#define initializeMap(TYPE, MAP, WIDTH, HEIGHT) (MAP)->width = (int)(WIDTH);   \
                                                    (MAP)->height = (int)(HEIGHT); \
                                                    (MAP)->chars = malloc(sizeof##TYPE(MAP))

#define freeMap(MAP) free((MAP)->chars)

#define normalizePosition(GLYPHMAP, SPRITE, SPRITEX, SPRITEY, X, Y)  (X) = (GLYPHMAP)->width/2-(SPRITE)->width/2+(SPRITEX);  \
                                                                      (Y) = (GLYPHMAP)->height/2-(SPRITE)->height/2-(SPRITEY)

static inline VTChar humanizeSwapChar(VTChar implChar) {
	#ifndef VISUALT_IS_BIG_ENDIAN
	implChar = endiannessSwap32(implChar);
	#endif
	if(implChar&0xffu) {
		return implChar;
	}
	implChar >>= 8u;
	if(implChar&0xffu) {
		return implChar;
	}
	implChar >>= 8u;
	if(implChar&0xffu) {
		return implChar;
	}
	implChar >>= 8u;
	return implChar;
}

static void initializeGlyphMapString(GlyphMap *const sprite, uint8_t const *const utf8Text) {
	int spriteWidth = 1, spriteHeight = 1;

	//calculate dimensions
	for(int width = 0, i = 0; utf8Text[i] != '\0'; i++) {
		if((utf8Text[i]&0xc0u) != 0x80) { // partial code point: 10xx xxxx
			if(utf8Text[i] == '\n') {
				spriteHeight++;
				width = 0;
			} else {
				width++;
				if(width > spriteWidth) {
					spriteWidth = width;
				}
			}
		}
	}
	initializeMap(Glyphs, sprite, spriteWidth, spriteHeight);
	clearMap(Glyphs, sprite);
	//parse
	for(int i = 0, y = 0; y < spriteHeight; y++) {
		for(int x = 0;; x++) {
			if(utf8Text[i] == '\n' || utf8Text[i] == '\0') {
				i++;
				break;
			}
			if(utf8Text[i] == '\v') {  //1B empty
				i++;
				continue;
			}
			uint8_t *const bytes = (uint8_t *)&sprite->chars[x+y*spriteWidth];
			if(utf8Text[i] >= 0xf0) {         //4B code point: 1111 0xxx
				bytes[0] = utf8Text[i++];
				bytes[1] = utf8Text[i++];
				bytes[2] = utf8Text[i++];
				bytes[3] = utf8Text[i++];
			} else if(utf8Text[i] >= 0xe0) {  //3B code point: 1110 xxxx
				bytes[0] = utf8Text[i++];
				bytes[1] = utf8Text[i++];
				bytes[2] = utf8Text[i++];
			} else if(utf8Text[i] >= 0xc2) {  //2B code point: 110x xxxx
				bytes[0] = utf8Text[i++];
				bytes[1] = utf8Text[i++];
			} else {                          //1B code point: 0xxx xxxx
				bytes[0] = utf8Text[i++];
			}
		}
	}
}

static void printGlyphMap(GlyphMap const *const sprite, bool const border) {
	int const width = sprite->width, height = sprite->height;
	uint8_t *const buffer = malloc((size_t)width*sizeof(VTChar)+sizeof(uint8_t)*7); //2*3B '|' + 1B '\0'

	if(border) {
		int i = 0;
		buffer[i++] = 0xe2;  //┌
		buffer[i++] = 0x94;  //┌
		buffer[i++] = 0x8c;  //┌
		while(i/3 <= width) {
			buffer[i++] = 0xe2;  //─
			buffer[i++] = 0x94;  //─
			buffer[i++] = 0x80;  //─
		}
		buffer[i++] = 0xe2;    //┐
		buffer[i++] = 0x94;    //┐
		buffer[i++] = 0x90;    //┐
		buffer[i] = '\0';
		puts((char *)buffer);
	}
	for(int y = 0; y < height; y++) {
		int i = 0;
		if(border) {
			buffer[i++] = 0xe2;  //│
			buffer[i++] = 0x94;  //│
			buffer[i++] = 0x82;  //│
		}
		for(int x = 0; x < width; x++) {
			uint8_t const *const bytes = (uint8_t *)&sprite->chars[x+y*width];
			if(bytes[0] >= 0xf0) {        //4B code point: 1111 0xxx
				buffer[i++] = bytes[0];
				buffer[i++] = bytes[1];
				buffer[i++] = bytes[2];
				buffer[i++] = bytes[3];
			} else if(bytes[0] >= 0xe0) {  //3B code point: 1110 xxxx
				buffer[i++] = bytes[0];
				buffer[i++] = bytes[1];
				buffer[i++] = bytes[2];
			} else if(bytes[0] >= 0xc2) {  //2B code point: 110x xxxx
				buffer[i++] = bytes[0];
				buffer[i++] = bytes[1];
			} else if(bytes[0]) {          //1B code point: 0xxx xxxx
				buffer[i++] = bytes[0];
			} else {                      //1B empty
				buffer[i++] = ' ';
			}
		}
		if(border) {
			buffer[i++] = 0xe2;  //│
			buffer[i++] = 0x94;  //│
			buffer[i++] = 0x82;  //│
		}
		buffer[i] = '\0';
		puts((char *)buffer);
	}
	if(border) {
		int i = 0;
		buffer[i++] = 0xe2;  //└
		buffer[i++] = 0x94;  //└
		buffer[i++] = 0x94;  //└
		while(i/3 <= width) {
			buffer[i++] = 0xe2;  //─
			buffer[i++] = 0x94;  //─
			buffer[i++] = 0x80;  //─
		}
		buffer[i++] = 0xe2;  //┘
		buffer[i++] = 0x94;  //┘
		buffer[i++] = 0x98;  //┘
		buffer[i] = '\0';
		puts((char *)buffer);
	}
	free(buffer);
}

static size_t stringifiedGlyphMapSize(GlyphMap const *const sprite, bool const border) {
	int const width = sprite->width, height = sprite->height;
	int length = 0;

	for(int y = 0; y < height; length++, y++) {
		for(int x = 0; x < width; x++) {
			uint8_t const *const bytes = (uint8_t *)&sprite->chars[x+y*width];
			if(bytes[0] >= 0xf0) {        //4B code point: 1111 0xxx
				length += 4;
			} else if(bytes[0] >= 0xe0) { //3B code point: 1110 xxxx
				length += 3;
			} else if(bytes[0] >= 0xc2) { //2B code point: 110x xxxx
				length += 2;
			} else {                      //1B code point: 0xxx xxxx
				length += 1;
			}
		}
	}
	if(border) {
		length += 3*((width+2)*2+(2*height)) //first and last row + '│' for every line
				+2; //first '\n' and last '\0'
	}
	return (size_t)length*sizeof(uint8_t);
}

static size_t stringifyGlyphMap(GlyphMap const *const sprite, bool const border, uint8_t **const utf8String) {
	int const width = sprite->width, height = sprite->height;
	VTChar const *const chars = sprite->chars;
	size_t const size = stringifiedGlyphMapSize(sprite, border);
	if(!*utf8String) {
		*utf8String = malloc(size);
	}
	uint8_t *const string = *utf8String;
	int i = 0;

	if(border) {
		string[i++] = 0xe2;  //┌
		string[i++] = 0x94;  //┌
		string[i++] = 0x8c;  //┌
		for(int j = 0; j < width; j++) {
			string[i++] = 0xe2;  //─
			string[i++] = 0x94;  //─
			string[i++] = 0x80;  //─
		}
		string[i++] = 0xe2;  //┐
		string[i++] = 0x94;  //┐
		string[i++] = 0x90;  //┐
		string[i++] = '\n';
	}
	for(int y = 0; y < height; y++) {
		if(border) {
			string[i++] = 0xe2;  //│
			string[i++] = 0x94;  //│
			string[i++] = 0x82;  //│
		}
		for(int x = 0; x < width; x++) {
			uint8_t const *const bytes = (uint8_t *)&chars[x+y*width];
			if(bytes[0] >= 0xf0) {        //4B code point: 1111 0xxx
				string[i++] = bytes[0];
				string[i++] = bytes[1];
				string[i++] = bytes[2];
				string[i++] = bytes[3];
			} else if(bytes[0] >= 0xe0) {  //3B code point: 1110 xxxx
				string[i++] = bytes[0];
				string[i++] = bytes[1];
				string[i++] = bytes[2];
			} else if(bytes[0] >= 0xc2) {  //2B code point: 110x xxxx
				string[i++] = bytes[0];
				string[i++] = bytes[1];
			} else {
				if(bytes[0]) {
					string[i++] = bytes[0];    //1B code point: 0xxx xxxx
				} else {
					string[i++] = ' ';         //1B empty
				}
			}
		}
		if(border) {
			string[i++] = 0xe2;  //│
			string[i++] = 0x94;  //│
			string[i++] = 0x82;  //│
		}
		string[i++] = '\n';
	}
	if(border) {
		string[i++] = 0xe2;  //└
		string[i++] = 0x94;  //└
		string[i++] = 0x94;  //└
		for(int j = 0; j < width; j++) {
			string[i++] = 0xe2;  //─
			string[i++] = 0x94;  //─
			string[i++] = 0x80;  //─
		}
		string[i++] = 0xe2;  //┘
		string[i++] = 0x94;  //┘
		string[i++] = 0x98;  //┘
		i++; //\n
	}
	string[i-1] = '\0';
	return size;
}

static void stamp(GlyphMap const *const restrict canvas, GlyphMap const *const restrict sprite, int spriteX, int spriteY, bool const reversed) {
	normalizePosition(canvas, sprite, spriteX, spriteY, spriteX, spriteY);
	if(reversed) {
		for(int y = 0; y < sprite->height; y++) {
			for(int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < canvas->width && spriteY+y >= 0 && spriteY+y < canvas->height) {
					if(!canvas->chars[(spriteX+x)+(spriteY+y)*canvas->width]) {
						canvas->chars[(spriteX+x)+(spriteY+y)*canvas->width] = sprite->chars[x+y*sprite->width];
					}
				}
			}
		}
	} else {
		for(int y = 0; y < sprite->height; y++) {
			for(int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < canvas->width && spriteY+y >= 0 && spriteY+y < canvas->height) {
					canvas->chars[(spriteX+x)+(spriteY+y)*canvas->width] = sprite->chars[x+y*sprite->width];
				}
			}
		}
	}
}

static void mask(BoolMap const *const mask, GlyphMap const *const sprite, int spriteX, int spriteY) {
	normalizePosition(mask, sprite, spriteX, spriteY, spriteX, spriteY);
	for(int y = 0; y < sprite->height; y++) {
		for(int x = 0; x < sprite->width; x++) {
			if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < mask->width && spriteY+y >= 0 && spriteY+y < mask->height) {
				mask->chars[(spriteX+x)+(spriteY+y)*mask->width] = true;
			}
		}
	}
}

static void render(GlyphMap const *const canvas, unsigned int const objsLength, Obj const *const *const objs, bool const clearGlyphMap) {
	if(clearGlyphMap) {
		clearMap(Glyphs, canvas);
		for(unsigned int i = objsLength; i-- > 0;) {
			if(objs[i]->visible) {
				stamp(canvas, objs[i]->activeSprite, objs[i]->x, objs[i]->y, true);
			}
		}
	} else {
		for(unsigned int i = 0; i < objsLength; i++) {
			if(objs[i]->visible) {
				stamp(canvas, objs[i]->activeSprite, objs[i]->x, objs[i]->y, false);
			}
		}
	}
}

static void line(GlyphMap const *const canvas, unsigned short const penSize, VTChar const penChar, int x0, int y0, int const x1, int const y1) {
	GlyphMap stroke;
	switch(penSize) {
		default:
		case 1:
		initializeMap(Glyphs, &stroke, 1, 1);
			memcpy(stroke.chars, (VTChar[1*1]){penChar}, sizeofGlyphs(&stroke));
			break;
		case 2:
		initializeMap(Glyphs, &stroke, 2, 2);
			memcpy(stroke.chars, (VTChar[2*2]){penChar, penChar, penChar, penChar}, sizeofGlyphs(&stroke));
			break;
		case 3:
		initializeMap(Glyphs, &stroke, 4, 3);
			memcpy(stroke.chars, (VTChar[4*3]){0, penChar, penChar, 0, penChar, penChar, penChar, penChar, 0, penChar, penChar, 0}, sizeofGlyphs(&stroke));
			break;
		case 4:
		initializeMap(Glyphs, &stroke, 5, 3);
			memcpy(stroke.chars, (VTChar[5*3]){0, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, 0}, sizeofGlyphs(&stroke));
			break;
		case 5:
		initializeMap(Glyphs, &stroke, 7, 4);
			memcpy(stroke.chars, (VTChar[7*4]){0, 0, penChar, penChar, penChar, 0, 0, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, 0, 0, penChar, penChar, penChar, 0, 0}, sizeofGlyphs(&stroke));
			break;
		case 6:
		initializeMap(Glyphs, &stroke, 8, 4);
			memcpy(stroke.chars, (VTChar[8*4]){0, penChar, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, penChar, 0, penChar, penChar, penChar, penChar, penChar, penChar, 0}, sizeofGlyphs(&stroke));
			break;
	}
	int const dx = abs(x1-x0), dy = abs(y1-y0);
	int err = (dx > dy ? dx : -dy)/2;
	short const stepX = x0 < x1 ? 1 : -1, stepY = y0 < y1 ? 1 : -1;
	while(1) {
		stamp(canvas, &stroke, x0, y0, false);
		if(x0 == x1 && y0 == y1) {
			break;
		}
		int t = err;
		if(t > -dx) {
			err -= dy;
			x0 += stepX;
		}
		if(t < dy) {
			err += dx;
			y0 += stepY;
		}
	}
	freeMap(&stroke);
}

//----MISC----
void vtAbout(void) {
	puts("                 .:Visual T:.\n"
			 "     ver " VISUALT_VERSION "  " VISUALT_BUILD_DATE
			 "\n"
			 "                     MIT      \n"
			 "\n"
			 " A text-based graphic library written by Lucide\n"
			 "       https://github.com/Lucide/VisualT\n"
			 "\n"
			 "                                \\    /\\\n"
			 "       In Memory of Simba        )  ( ')\n"
			 "                                (  /  )\n"
			 "                           jgs   \\(__)|\n");
}

VTChar vtChar(char const *const ltChar) {
	VTStr const utf8Text = (VTStr)ltChar;
	VTChar vtChar = 0;
	uint8_t *const bytes = (uint8_t *)&vtChar;
	if(utf8Text[0] >= 0xf0) {        //4B code point: 1111 0xxx
		bytes[0] = utf8Text[0];
		bytes[1] = utf8Text[1];
		bytes[2] = utf8Text[2];
		bytes[3] = utf8Text[3];
	} else if(utf8Text[0] >= 0xe0) {  //3B code point: 1110 xxxx
		bytes[0] = utf8Text[0];
		bytes[1] = utf8Text[1];
		bytes[2] = utf8Text[2];
	} else if(utf8Text[0] >= 0xc2) {  //2B code point: 110x xxxx
		bytes[0] = utf8Text[0];
		bytes[1] = utf8Text[1];
	} else if(utf8Text[0] != '\v') {  //1B code point: 0xxx xxxx
		bytes[0] = utf8Text[0];
	}
	return vtChar;
}

//----INITIALIZATION----
void vtInitializeBlank(Obj *const obj, unsigned int const sizesLength, VTSizes const sizes) {
	assert(obj);
	assert(sizesLength > 0);
	assert(sizes);

	obj->length = sizesLength;
	obj->sprites = malloc(sizesLength*sizeof(GlyphMap));
	for(unsigned int i = 0; i < sizesLength; i++) {
		assert(sizes[i][0] > 0);
		assert(sizes[i][1] > 0);
		initializeMap(Glyphs, &obj->sprites[i], sizes[i][0], sizes[i][1]);
		clearMap(Glyphs, &obj->sprites[i]);
	}
	initializeObj(obj);
}

void vtInitializeArray(Obj *const obj, VTChar const *const v) {
	assert(obj);
	assert(v);

	obj->length = v[0];
	obj->sprites = malloc(obj->length*sizeof(GlyphMap));
	for(unsigned int i = 0, j = 1; i < obj->length; i++) {
		GlyphMap *const sprite = &obj->sprites[i];
		initializeMap(Glyphs, sprite, v[j], v[j+1]);
		j += 2;
		for(int y = 0; y < sprite->height; y++) {
			for(int x = 0; x < sprite->width; x++, j++) {
				sprite->chars[x+y*sprite->width] = humanizeSwapChar(v[j]);
			}
		}
	}
	initializeObj(obj);
}

void vtInitializeFile(Obj *const obj, FILE *const file) {
	assert(obj);
	assert(file);
	int vLength = 0;

	while(fscanf(file, "%*" SCNu32) != EOF) { // NOLINT(cert-err34-c)
		vLength++;
	}
	VTChar *const v = malloc((size_t)vLength*sizeof(VTChar));
	rewind(file);
	for(int i = 0; i < vLength; i++) {
		if(!fscanf(file, "%" SCNu32, &v[i])) { // NOLINT(cert-err34-c)
			perror("serialized Object file parsing failed");
			return;
		}
	}
	fclose(file);
	vtInitializeArray(obj, v);
	free(v);
}

void vtInitializeStrings(Obj *obj, unsigned int utf8StringsLength, VTStrs utf8Strings) {
	assert(obj);
	assert(utf8StringsLength > 0);
	assert(utf8Strings);

	obj->length = utf8StringsLength;
	obj->sprites = malloc(utf8StringsLength*sizeof(GlyphMap));
	for(unsigned int i = 0; i < utf8StringsLength; i++) {
		initializeGlyphMapString(&obj->sprites[i], utf8Strings[i]);
	}
	initializeObj(obj);
}

void vtInitializeObj(Obj *restrict const obj, Obj const *restrict const src) {
	assert(obj);
	assert(src);
	assert(obj != src);

	*obj = *src;
	obj->sprites = malloc(obj->length*sizeof(GlyphMap));
	for(unsigned int i = 0; i < obj->length; i++) {
		GlyphMap *const sprite = &obj->sprites[i];
		initializeMap(Glyphs, sprite, src->sprites[i].width, src->sprites[i].height);
		memcpy(sprite->chars, src->sprites[i].chars, sizeofGlyphs(sprite));
	}
	obj->activeSprite = &obj->sprites[src->activeSprite-src->sprites];
}

void vtRelease(unsigned int const objsLength, VTObjs const objs) {
	assert(objsLength > 0);
	assert(objs);

	for(unsigned int i = 0; i < objsLength; i++) {
		for(unsigned int j = 0; j < objs[i]->length; j++) {
			freeMap(&objs[i]->sprites[j]);
		}
		free(objs[i]->sprites);
	}
}

void vtCloneSprite(Obj const *const dest, unsigned int const spriteDest, Obj const *const src, unsigned int const spriteSrc) {
	assert(dest);
	assert(spriteDest < dest->length);
	assert(src);
	assert(spriteSrc < src->length);
	assert(&dest->sprites[spriteDest] != &src->sprites[spriteSrc]);

	freeMap(&dest->sprites[spriteDest]);
	dest->sprites[spriteDest] = src->sprites[spriteSrc];
	initializeMap(Glyphs, &dest->sprites[spriteDest], dest->sprites[spriteDest].width, dest->sprites[spriteDest].height);
	memcpy(dest->sprites[spriteDest].chars, src->sprites[spriteSrc].chars, sizeofGlyphs(&dest->sprites[spriteDest]));
}

void vtResize(Obj *const obj, unsigned int const width, unsigned int const height) {
	assert(obj);
	assert(width > 0);
	assert(height > 0);

	freeMap(obj->activeSprite);
	initializeMap(Glyphs, obj->activeSprite, width, height);
	clearMap(Glyphs, obj->activeSprite);
}

//----REFRESH----
void vtRender(Obj const *const restrict canvas, unsigned int const objsLength, VTObjs const objs) {
	assert(canvas);
	assert(objsLength > 0);
	assert(objs);

	render(canvas->activeSprite, objsLength, objs, true);
}

void vtStamp(Obj const *const restrict canvas, unsigned int const objsLength, VTObjs const objs) {
	assert(canvas);
	assert(objsLength > 0);
	assert(objs);

	render(canvas->activeSprite, objsLength, objs, false);
}

void vtPrint(Obj const *const obj, bool const border) {
	assert(obj);

	printGlyphMap(obj->activeSprite, border);
}

size_t vtPrintToString(Obj const *const obj, bool const border, uint8_t **const utf8Buffer) {
	assert(obj);
	assert(utf8Buffer);

	return stringifyGlyphMap(obj->activeSprite, border, utf8Buffer);
}

size_t vtPrintStringSize(Obj const *obj, bool border) {
	assert(obj);

	return stringifiedGlyphMapSize(obj->activeSprite, border);
}

//----SPRITE----
unsigned int vtSprites(Obj const *const obj) {
	assert(obj);

	return obj->length;
}

unsigned int vtSpriteInd(Obj const *const obj) {
	assert(obj);

	return (unsigned int)(obj->activeSprite-obj->sprites);
}

void vtNextSprite(Obj *const obj) {
	assert(obj);

	obj->activeSprite = &(obj->sprites[(obj->activeSprite-obj->sprites+1)%obj->length]);
}

void vtPrecSprite(Obj *const obj) {
	assert(obj);

	obj->activeSprite = &(obj->sprites[(obj->activeSprite-obj->sprites+obj->length-1)%obj->length]);
}

void vtSetSprite(Obj *obj, unsigned int const index) {
	assert(obj);
	assert(index < obj->length);

	obj->activeSprite = &obj->sprites[index];
}

int vtWidth(Obj const *const obj) {
	assert(obj);

	return obj->activeSprite->width;
}

int vtHeight(Obj const *const obj) {
	assert(obj);

	return obj->activeSprite->height;
}

int vtExtremum(Obj const *const obj, VTDirection const direction) {
	int const width = obj->activeSprite->width, height = obj->activeSprite->height;
	switch(direction) {
		default:
		case VT_LEFT:
			return -width/2;
		case VT_RIGHT:
			return width-width/2-1;
		case VT_TOP:
			return height/2;
		case VT_BOTTOM:
			return -height+height/2+1;
	}
}

void vtSetText(Obj *const obj, bool fitToContent, VTStr const utf8String) {
	assert(obj);
	assert(utf8String);

	if(fitToContent) {
		freeMap(obj->activeSprite);
		initializeGlyphMapString(obj->activeSprite, utf8String);
	} else {
		GlyphMap const *const sprite = obj->activeSprite;
		clearMap(Glyphs, sprite);

		for(int y = 0, x = 0, i = 0; utf8String[i] != 0;) {
			if(utf8String[i] == '\n') {
				x = 0;
				y++;
				i++;
				continue;
			}
			VTChar t = 0;
			uint8_t *const bytes = (uint8_t *)&t;
			if(utf8String[i] == '\v') {         //1B empty
				i++;
			} else if(utf8String[i] >= 0xf0) {  //4B code point: 1111 0xxx
				bytes[0] = utf8String[i++];
				bytes[1] = utf8String[i++];
				bytes[2] = utf8String[i++];
				bytes[3] = utf8String[i++];
			} else if(utf8String[i] >= 0xe0) {  //3B code point: 1110 xxxx
				bytes[0] = utf8String[i++];
				bytes[1] = utf8String[i++];
				bytes[2] = utf8String[i++];
			} else if(utf8String[i] >= 0xc2) {  //2B code point: 110x xxxx
				bytes[0] = utf8String[i++];
				bytes[1] = utf8String[i++];
			} else {                            //1B code point: 0xxx xxxx
				bytes[0] = utf8String[i++];
			}
			if(x < sprite->width && y < sprite->height) {
				sprite->chars[x+y*sprite->width] = t;
			}
			x++;
		}
	}
}

void vtClear(Obj const *const obj) {
	assert(obj);

	clearMap(Glyphs, obj->activeSprite);
}

void vtFill(Obj const *const obj, VTChar const fillChar) {
	assert(obj);

	for(int i = 0; i < obj->activeSprite->width*obj->activeSprite->height; i++) {
		obj->activeSprite->chars[i] = fillChar;
	}
}

void vtReplace(Obj const *const obj, VTChar const oldChar, VTChar const newChar) {
	assert(obj);

	for(int i = 0; i < obj->activeSprite->width*obj->activeSprite->height; i++) {
		if(obj->activeSprite->chars[i] == oldChar) {
			obj->activeSprite->chars[i] = newChar;
		}
	}
}

void vtShift(Obj const *const obj, VTDirection direction, int amount) {
	assert(obj);

	if(amount < 0) {
		amount = -amount;
		if(direction <= 1) {
			direction = 1-direction;
		} else {
			direction = 1-(direction-2)+2;
		}
	}
	GlyphMap const *const sprite = obj->activeSprite;
	switch(direction) {
		default:
		case VT_LEFT:
			for(int y = 0; y < sprite->height; y++) {
				int x = 0;
				for(; x+amount < sprite->width; x++) {
					sprite->chars[x+y*sprite->width] = sprite->chars[x+amount+y*sprite->width];
				}
				for(; x < sprite->width; x++) {
					sprite->chars[x+y*sprite->width] = 0;
				}
			}
			break;
		case VT_RIGHT:
			for(int y = 0; y < sprite->height; y++) {
				int x = sprite->width-1;
				for(; x-amount >= 0; x--) {
					sprite->chars[x+y*sprite->width] = sprite->chars[x-amount+y*sprite->width];
				}
				for(; x >= 0; x--) {
					sprite->chars[x+y*sprite->width] = 0;
				}
			}
			break;
		case VT_TOP:
			for(int x = 0; x < sprite->width; x++) {
				int y = 0;
				for(; y+amount < sprite->height; y++) {
					sprite->chars[x+y*sprite->width] = sprite->chars[x+(y+amount)*sprite->width];
				}
				for(; y < sprite->height; y++) {
					sprite->chars[x+y*sprite->width] = 0;
				}
			}
			break;
		case VT_BOTTOM:
			for(int x = 0; x < sprite->width; x++) {
				int y = sprite->height-1;
				for(; y-amount >= 0; y--) {
					sprite->chars[x+y*sprite->width] = sprite->chars[x+(y-amount)*sprite->width];
				}
				for(; y >= 0; y--) {
					sprite->chars[x+y*sprite->width] = 0;
				}
			}
			break;
	}
}

void vtRotate(Obj const *const obj, VTDirection direction, int amount) {
	assert(obj);

	GlyphMap const *const sprite = obj->activeSprite;
	if(amount < 0) {
		amount = -amount;
		if(direction <= 1) {
			amount %= sprite->width;
			direction = 1-direction;
		} else {
			amount %= sprite->height;
			direction = 1-(direction-2)+2;
		}
	}
	switch(direction) {
		default:
		case VT_LEFT:
			for(int y = 0; y < sprite->height; y++) {
				for(int k = 0; k < amount; k++) {
					int x = 0;
					VTChar t = sprite->chars[x+y*sprite->width];
					for(; x+1 < sprite->width; x++) {
						sprite->chars[x+y*sprite->width] = sprite->chars[x+1+y*sprite->width];
					}
					sprite->chars[x+y*sprite->width] = t;
				}
			}
			break;
		case VT_RIGHT:
			for(int y = 0; y < sprite->height; y++) {
				for(int k = 0; k < amount; k++) {
					int x = sprite->width-1;
					VTChar t = sprite->chars[x+y*sprite->width];
					for(; x-1 >= 0; x--) {
						sprite->chars[x+y*sprite->width] = sprite->chars[x-1+y*sprite->width];
					}
					sprite->chars[x+y*sprite->width] = t;
				}
			}
			break;
		case VT_TOP:
			for(int x = 0; x < sprite->width; x++) {
				for(int k = 0; k < amount; k++) {
					int y = 0;
					VTChar t = sprite->chars[x+y*sprite->width];
					for(; y+1 < sprite->height; y++) {
						sprite->chars[x+y*sprite->width] = sprite->chars[x+(y+1)*sprite->width];
					}
					sprite->chars[x+y*sprite->width] = t;
				}
			}
			break;
		case VT_BOTTOM:
			for(int x = 0; x < sprite->width; x++) {
				for(int k = 0; k < amount; k++) {
					int y = sprite->height-1;
					VTChar t = sprite->chars[x+y*sprite->width];
					for(; y-1 >= 0; y--) {
						sprite->chars[x+y*sprite->width] = sprite->chars[x+(y-1)*sprite->width];
					}
					sprite->chars[x+y*sprite->width] = t;
				}
			}
			break;
	}
}

void vtOverlay(Obj const *const dest, unsigned int const spriteDest, Obj const *const src, unsigned int const spriteSrc) {
	assert(dest);
	assert(spriteDest < dest->length);
	assert(src);
	assert(spriteSrc < src->length);
	assert(src->sprites[spriteSrc].width == dest->sprites[spriteDest].width);
	assert(src->sprites[spriteSrc].height == dest->sprites[spriteDest].height);

	for(int i = src->sprites[spriteSrc].width*src->sprites[spriteSrc].height; --i >= 0;) {
		if(src->sprites[spriteSrc].chars[i]) {
			dest->sprites[spriteDest].chars[i] = src->sprites[spriteSrc].chars[i];
		}
	}
}

void vtDrawAxes(Obj const *obj) {
	assert(obj);
	VTChar *const chars = obj->activeSprite->chars;
	int const width = obj->activeSprite->width, height = obj->activeSprite->height;
	int const centerX = width/2, centerY = height/2;
	char value[12];
	Obj label;

	for(int i = 0; i < centerX; i++) {
		chars[i+centerY*width] = vtChar("┄");
	}
	for(int i = centerX+1; i < width; i++) {
		chars[i+centerY*width] = vtChar("─");
	}
	for(int i = 0; i < centerY; i++) {
		chars[centerX+i*width] = vtChar("│");
	}
	for(int i = centerY+1; i < height; i++) {
		chars[centerX+i*width] = vtChar("┊");
	}
	sprintf(value, "%+d", -centerX);
	vtInitializeStrings(&label, 1, LTSTRS{value});
	stamp(obj->activeSprite, label.activeSprite, (-centerX)+(label.activeSprite->width/2), 0, false);

	sprintf(value, "%+d", width-centerX-1);
	vtSetText(&label, true, (VTStr)value);
	stamp(obj->activeSprite, label.activeSprite, (width-centerX-1)-(label.activeSprite->width-label.activeSprite->width/2-1), 0, false);

	sprintf(value, "%+d", centerY);
	vtSetText(&label, true, (VTStr)value);
	stamp(obj->activeSprite, label.activeSprite, 0, centerY, false);

	sprintf(value, "%+d", -height+centerY+1);
	vtSetText(&label, true, (VTStr)value);
	stamp(obj->activeSprite, label.activeSprite, 0, -height+centerY+1, false);

	vtRelease(1, LTOBJS{&label});
	chars[centerX+centerY*width] = '0';
}

//----OBJ----
bool vtVisible(Obj const *const obj) {
	assert(obj);
	return obj->visible;
}

void vtShow(Obj *const obj) {
	assert(obj);
	obj->visible = true;
}

void vtHide(Obj *const obj) {
	assert(obj);
	obj->visible = false;
}

void vtSetVisibility(Obj *const obj, bool const visible) {
	assert(obj);
	obj->visible = visible;
}

void vtSerialize(Obj const *const obj, VTChar *const v) {
	assert(obj);

	if(v) {
		v[0] = (VTChar)obj->length;
		for(unsigned int i = 0, j = 1; i < obj->length; i++) {
			GlyphMap *const sprite = &obj->sprites[i];
			v[j++] = (VTChar)sprite->width;
			v[j++] = (VTChar)sprite->height;
			for(int y = 0; y < sprite->height; y++) {
				for(int x = 0; x < sprite->width; x++, j++) {
					v[j] = humanizeSwapChar(sprite->chars[x+y*sprite->width]);
				}
			}
		}
	} else {
		printf("%"PRIu32" ", obj->length);
		for(unsigned int i = 0; i < obj->length; i++) {
			GlyphMap *const sprite = &obj->sprites[i];
			printf("%"PRIu32" %"PRIu32" ", sprite->width, sprite->height);
			for(int y = 0; y < sprite->height; y++) {
				for(int x = 0; x < sprite->width; x++) {
					printf("%"PRIu32" ", humanizeSwapChar(sprite->chars[x+y*sprite->width]));
				}
			}
		}
	}
}

size_t vtSerializedArraySize(Obj const *const obj) {
	assert(obj);
	int length = 1; //sprites length

	for(unsigned int i = 0; i < obj->length; i++) {
		length += 2 //sprite dimensions
				+obj->sprites[i].width*obj->sprites[i].height;
	}
	return (size_t)length*sizeof(VTChar);
}

//----PEN----
VTChar vtPenGlyph(Obj const *obj) {
	assert(obj);
	return obj->penChar;
}

unsigned short vtPenSize(Obj const *const obj) {
	assert(obj);
	return obj->penSize;
}

void vtSetPenSize(Obj *const obj, unsigned short const size) {
	assert(obj);
	obj->penSize = (size >= 1 && size <= 6) ? size : (unsigned short)1;
}

void vtSetPenGlyph(Obj *obj, VTChar penChar) {
	assert(obj);
	obj->penChar = penChar;
}

//----MOVE----
int vtXPosition(Obj const *const obj) {
	assert(obj);
	return obj->x;
}

int vtYPosition(Obj const *const obj) {
	assert(obj);
	return obj->y;
}

void vtGotoXY(Obj const *const canvas, Obj *const obj, int const x, int const y) {
	assert(obj);

	if(canvas) {
		line(canvas->activeSprite, obj->penSize, obj->penChar, obj->x, obj->y, x, y);
	}
	obj->x = x;
	obj->y = y;
}

void vtGotoX(Obj const *const canvas, Obj *const obj, int const x) {
	assert(obj);

	if(canvas) {
		line(canvas->activeSprite, obj->penSize, obj->penChar, obj->x, obj->y, x, obj->y);
	}
	obj->x = x;
}

void vtGotoY(Obj const *const canvas, Obj *const obj, int const y) {
	assert(obj);

	if(canvas) {
		line(canvas->activeSprite, obj->penSize, obj->penChar, obj->x, obj->y, obj->x, y);
	}
	obj->y = y;
}

void vtChangeX(Obj const *const canvas, Obj *const obj, int const x) {
	assert(obj);

	if(canvas) {
		line(canvas->activeSprite, obj->penSize, obj->penChar, obj->x, obj->y, obj->x+x, obj->y);
	}
	obj->x += x;
}

void vtChangeY(Obj const *const canvas, Obj *const obj, int const y) {
	assert(obj);

	if(canvas) {
		line(canvas->activeSprite, obj->penSize, obj->penChar, obj->x, obj->y, obj->x, obj->y-y);
	}
	obj->y += y;
}

void vtAlign(Obj *const obj, VTAlign const position) {
	assert(obj);
	int const centerX = obj->activeSprite->width/2, centerY = obj->activeSprite->height/2;
	switch(position) {
		default:
			return;
		case VT_TOP_LEFT:
			obj->x += centerX;
			obj->y -= centerY;
			break;
		case VT_TOP_RIGHT:
			obj->x -= obj->activeSprite->width-centerX-1;
			obj->y -= centerY;
			break;
		case VT_BOTTOM_LEFT:
			obj->x += centerX;
			obj->y += obj->activeSprite->height-centerY-1;
			break;
		case VT_BOTTOM_RIGHT:
			obj->x -= obj->activeSprite->width-centerX-1;
			obj->y += obj->activeSprite->height-centerY-1;
			break;
	}
}

//----SENSORS----
bool vtIsTouching(Obj const *const canvas, Obj const *const obj, unsigned int objsLength, VTObjs const objs) {
	assert(canvas);
	assert(obj);
	assert(objsLength > 0);
	assert(objs);

	if(obj->visible) {
		BoolMap boolMap;
		initializeMap(Bools, &boolMap, canvas->activeSprite->width, canvas->activeSprite->height);
		clearMap(Bools, &boolMap);
		for(unsigned int i = 0; i < objsLength; i++) {
			if(objs[i]->visible) {
				mask(&boolMap, objs[i]->activeSprite, objs[i]->x, objs[i]->y);
			}
		}
		GlyphMap const *const sprite = obj->activeSprite;
		int spriteX, spriteY;
		normalizePosition(&boolMap, sprite, obj->x, obj->y, spriteX, spriteY);
		for(int y = 0; y < sprite->height; y++) {
			for(int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < boolMap.width && spriteY+y >= 0 && spriteY+y < boolMap.height) {
					if(boolMap.chars[(spriteX+x)+(spriteY+y)*boolMap.width]) {
						freeMap(&boolMap);
						return true;
					}
				}
			}
		}
		freeMap(&boolMap);
	}

	return false;
}

bool vtIsTouchingGlyph(Obj const *const canvas, Obj const *const obj, VTChar testChar) {
	assert(canvas);
	assert(obj);

	if(obj->visible) {
		GlyphMap const *const canvasSprite = canvas->activeSprite, *const sprite = obj->activeSprite;
		int spriteX, spriteY;
		normalizePosition(canvasSprite, sprite, obj->x, obj->y, spriteX, spriteY);
		for(int y = 0; y < sprite->height; y++) {
			for(int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && spriteX+x >= 0 && spriteX+x < canvasSprite->width && spriteY+y >= 0 && spriteY+y < canvasSprite->height) {
					if(canvasSprite->chars[(spriteX+x)+(spriteY+y)*canvasSprite->width] == testChar) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool vtIsOutside(Obj const *const canvas, Obj const *const obj) {
	assert(canvas);
	assert(obj);

	if(obj->visible) {
		GlyphMap const *const canvasSprite = canvas->activeSprite, *const sprite = obj->activeSprite;
		int spriteX, spriteY;
		normalizePosition(canvasSprite, sprite, obj->x, obj->y, spriteX, spriteY);
		for(int y = 0; y < sprite->height; y++) {
			for(int x = 0; x < sprite->width; x++) {
				if(sprite->chars[x+y*sprite->width] && !(spriteX+x >= 0 && spriteX+x < canvasSprite->width && spriteY+y >= 0 && spriteY+y < canvasSprite->height)) {
					return true;
				}
			}
		}
	}
	return false;
}
