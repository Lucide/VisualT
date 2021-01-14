#pragma once
#ifndef VISUALT_VISUALT_INTERNALS_H
#define VISUALT_VISUALT_INTERNALS_H

#include "visualt/visualt.h"

#define initializeObj(OBJ) (OBJ)->x = 0;          \
                           (OBJ)->y = 0;          \
                           (OBJ)->visible = true;  \
                           (OBJ)->penSize = 0;    \
                           (OBJ)->penChar = '#';  \
                           (OBJ)->activeSprite = (OBJ)->sprites

#define sizeofGlyphs(GLYPHMAP) ((size_t)(GLYPHMAP)->width*(size_t)(GLYPHMAP)->height*sizeof(VTChar))

#define sizeofBools(BOOLMAP) ((size_t)(BOOLMAP)->width*(size_t)(BOOLMAP)->height*sizeof(bool))

#define clearMap(TYPE, MAP) memset((MAP)->chars, 0, sizeof##TYPE((MAP)))

#define initializeMap(TYPE, MAP, WIDTH, HEIGHT) (MAP)->width = (int)(WIDTH);        \
                                                    (MAP)->height = (int)(HEIGHT);  \
                                                    (MAP)->chars = malloc(sizeof##TYPE(MAP))

#define freeMap(MAP) free((MAP)->chars)

#define flagErrorAndGoto(LABEL) error = true;  \
                                goto LABEL

typedef VTObj Obj;

typedef struct VTGlyphMap {
	VTChar *restrict chars;
	int width, height;
} GlyphMap;

typedef struct {
	bool *restrict chars;
	int width, height;
} BoolMap;

static inline VTChar codeToVTChar(uint32_t const code) {
	VTChar vtChar = 0;
	if(code <= 0x7F) {
		vtChar = code;                        // 0xxxxxxx
	} else if(code <= 0x7FF) {
		vtChar |= 0xC0|(code&0x7C0)>>6;       // 110xxxxx
		vtChar |= 0x8000|(code&0x3F)<<8;      // 10xxxxxx
	} else if(code <= 0xFFFF) {
		vtChar |= 0xE0|(code&0xF000)>>12;     // 1110xxxx
		vtChar |= 0x8000|(code&0xFC0)<<2;     // 10xxxxxx
		vtChar |= 0x800000|(code&0x3F)<<16;   // 10xxxxxx
	} else {
		vtChar |= 0xF0|(code&0x1C0000)>>18;   // 11110xxx
		vtChar |= 0x8000|(code&0x3F000)>>4;   // 10xxxxxx
		vtChar |= 0x800000|(code&0xFC0)<<10;  // 10xxxxxx
		vtChar |= 0x80000000|(code&0x3F)<<24; // 10xxxxxx
	}
	return vtChar;
}

static inline uint32_t VTCharToCode(VTChar const vtChar) {
	VTChar code = 0;
	if((vtChar&0xFF) <= 0x7F) {					//1B code point: 0xxx xxxx
		code = vtChar;                    // 0xxxxxxx
	} else if((vtChar&0xFF) <= 0xDF) {	//2B code point: 110x xxxx
		code |= (vtChar&0x3F00)>>8;       // 110xxxxx
		code |= (vtChar&0xF)<<6;          // 10xxxxxx
	} else if((vtChar&0xFF) <= 0xEF) {	//3B code point: 1110 xxxx
		code |= (vtChar&0x3F0000)>>16;    // 1110xxxx
		code |= (vtChar&0x3F00)>>2;       // 10xxxxxx
		code |= (vtChar&0xF)<<12;         // 10xxxxxx
	} else {														//4B code point: 1111 0xxx
		code |= (vtChar&0x3F000000)>>24;  // 11110xxx
		code |= (vtChar&0x3F0000)>>10;    // 10xxxxxx
		code |= (vtChar&0x3F00)<<4;       // 10xxxxxx
		code |= (vtChar&0x7)<<18;         // 10xxxxxx
	}
	return code;
}

#endif //VISUALT_VISUALT_INTERNALS_H
