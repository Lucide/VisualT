#include "visualt/import-xp.h"
#include "visualt-internals.h"
#include "zlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef WIN32
	#include <io.h>

#else
	#include <unistd.h>
#endif

#define ERROR_HEADER "VT .xp loader error: "

extern VTChar translate(uint32_t from);

static int readInts(gzFile file, uint32_t *const buffer, unsigned int const bufferLength) {
	if(gzfread(buffer, sizeof(uint32_t), bufferLength, file) < bufferLength) {
		int errnum;
		char const *const zErrorMsg = gzerror(file, &errnum);
		if(errnum == Z_ERRNO) {
			perror(ERROR_HEADER);
		} else {
			printf(ERROR_HEADER "%s", zErrorMsg);
		}
		return -1;
	}
	return 0;
}

static int readHeader(gzFile file) {
	uint32_t buffer[2];

	if(readInts(file, buffer, 2) < 0) {
		return -1;
	}
	if((int32_t)buffer[0] != -1) {
		fprintf(stderr, ERROR_HEADER "unsupported .xp version (was %d, expected -1).\n", buffer[0]);
		return -1;
	}
	return (int)buffer[1];
}

static int isTransparent(gzFile file) {
	uint8_t buffer[6];

	if(gzfread(buffer, sizeof(uint8_t), 6, file) < 6) {
		int errnum;
		char const *const zErrorMsg = gzerror(file, &errnum);
		if(errnum == Z_ERRNO) {
			perror(ERROR_HEADER);
		} else {
			printf(ERROR_HEADER "%s", zErrorMsg);
		}
		return -1;
	}
	if(buffer[3] == 255 && buffer[4] == 0 && buffer[5] == 255) {
		return 1;
	}
	return 0;
}

static void allocateSpritesOnce(Obj *const obj, int length) {
	if(obj->sprites == NULL) {
		obj->length = (unsigned)length;
		obj->sprites = malloc(obj->length*sizeof(GlyphMap));
	}
}

static int loadSpritesFromLayers(Obj *const obj, gzFile file, int *const allocatedSprites) {
	int width, height;
	for(; *allocatedSprites < (int)obj->length;) {
		{
			uint32_t buffer[2];
			if(readInts(file, buffer, 2) < 0) {
				return -1;
			}
			width = (int)buffer[0], height = (int)buffer[1];
		}
		GlyphMap *const sprite = &obj->sprites[*allocatedSprites];
		initializeMap(Glyphs, sprite, width, height);
		(*allocatedSprites)++;
		for(int x = 0; x < width; x++) {
			for(int y = 0; y < height; y++) {
				VTChar c;
				if(readInts(file, &c, 1) < 0) {
					return -1;
				}
				int transparent = isTransparent(file);
				if(transparent < 0) {
					return -1;
				}
				if(transparent) {
					c = 0;
				} else {
					c = translate(c);
				}
				sprite->chars[x+y*width] = c;
			}
		}
	}
	return 0;
}

static int loadSpritesFromFiles(Obj *const obj, gzFile file, int layerCount, int *const allocatedSprites) {
	int width, height;
	{
		uint32_t buffer[2];
		if(readInts(file, buffer, 2) < 0) {
			return -1;
		}
		width = (int)buffer[0], height = (int)buffer[1];
	}
	GlyphMap *const sprite = &obj->sprites[*allocatedSprites];
	initializeMap(Glyphs, sprite, width, height);
	(*allocatedSprites)++;
	for(int k = 0; k < layerCount; k++) {
		for(int x = 0; x < width; x++) {
			for(int y = 0; y < height; y++) {
				VTChar c;
				if(readInts(file, &c, 1) < 0) {
					return -1;
				}
				int transparent = isTransparent(file);
				if(transparent < 0) {
					return -1;
				}
				if(transparent) {
					c = 0;
				} else {
					c = translate(c);
				}
				sprite->chars[x+y*width] = c;
			}
		}
		// skip next layer's header
		if(gzseek(file, 2*sizeof(uint32_t), SEEK_CUR) < 0) {
			return -1;
		}
	}
	return 0;
}

static int openFile(struct gzFile_s **const file, FILE *const xpFile) {
	int fd = dup(fileno(xpFile));
	if(fd < 0) {
		return -1;
	}
	*file = gzdopen(fd, "rb");
	if(*file == NULL) {
		fputs(ERROR_HEADER "file open failed.", stderr);
		if(close(fd) < 0) {
			perror(ERROR_HEADER);
		}
		return -1;
	}
	return fd;
}

static int closeFile(gzFile file, int fd) {
	int error = gzclose_r(file);
	if(error != Z_OK) {
		if(error == Z_ERRNO) {
			perror(ERROR_HEADER);
		} else {
			if(error == Z_STREAM_ERROR) {
				if(close(fd) < 0) {
					perror(ERROR_HEADER);
				}
			}
			fputs(ERROR_HEADER "file close failed (compression error).", stderr);
		}
		return -1;
	}
	return 0;
}

int loadXp(Obj *const obj, VTXpLoadMode const loadMode, FILE *const restrict *const xpFiles, int const filesLength) {
	int error = false;
	obj->sprites = NULL;

	for(int allocatedSprites = 0, i = 0; i < filesLength; i++) {
		// open gzFile
		gzFile file;
		int fd = openFile(&file, xpFiles[i]);
		if(fd < 0) {
			flagErrorAndGoto(error);
		}
		// check header
		int layerCount = readHeader(file);
		if(layerCount < 0) {
			flagErrorAndGoto(closeXpFile);
		}
		// allocate sprite array once, allocate new sprites
		if(loadMode == VT_XP_LAYERS) {
			allocateSpritesOnce(obj, layerCount);
			error = loadSpritesFromLayers(obj, file, &allocatedSprites);
		} else {
			allocateSpritesOnce(obj, filesLength);
			error = loadSpritesFromFiles(obj, file, layerCount, &allocatedSprites);
		}
		error = error < 0 ? true : false;
		// close gzFile
		closeXpFile:
		if(closeFile(file, fd) < 0) {
			flagErrorAndGoto(error);
		}
		//continue;
		error:
		if(error) {
			// free already allocated sprites
			for(int k = 0; k < allocatedSprites; k++) {
				free(obj->sprites[k].chars);
			}
			// free sprites array, if allocated
			if(obj->sprites) {
				free(obj->sprites);
			}
			break;
		}
	}
	// close original files
	for(int i = 0; i < filesLength; i++) {
		fclose(xpFiles[i]);
	}
	if(error) {
		return -1;
	}
	return 0;
}
