#include "visualt/import-xp.h"
#include "visualt-internals.h"
#include <stdio.h>
#include <assert.h>

extern int loadXp(Obj *obj, VTXpLoadMode loadMode, FILE *const restrict *xpFiles, unsigned int filesLength);
extern int loadDictionary(FILE *mapFile);
extern void freeDictionary();

int vtInitializeXp(Obj *const obj, VTXpLoadMode const loadMode, FILE *const restrict mapFile, unsigned int const filesLength, FILE *const restrict *const files) {
	assert(obj);
	assert(mapFile);
	assert(files);
	assert(filesLength > 0);
	assert(loadMode != VT_XP_LAYERS || filesLength == 1);

	int error = false;
	if(loadDictionary(mapFile) < 0) {
		for(int i = 0; i < filesLength; ++i) {
			fclose(files[i])
		}
		flagErrorAndGoto(error);
	}
	if(loadXp(obj, loadMode, files, filesLength) < 0) {
		flagErrorAndGoto(freeDictionary);
	}
	initializeObj(obj);
	freeDictionary:
	freeDictionary();
	if(error) {
		error:
		return -1;
	}
	return 0;
}

