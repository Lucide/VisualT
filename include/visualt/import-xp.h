#pragma once
#ifndef VISUALT_IMPORT_XP_H
#define VISUALT_IMPORT_XP_H

#include "visualt/visualt.h"
#include "import_xp_export.h"

typedef enum VTXpLoadMode {
	VT_XP_FILES, VT_XP_LAYERS
} VTXpLoadMode;

VISUALT_IMPORT_XP_EXPORT int vtInitializeXp(VTObj *obj, VTXpLoadMode loadMode, FILE *restrict mapFile, FILE *const restrict *files, unsigned int filesLength);

#endif //VISUALT_IMPORT_XP_H
