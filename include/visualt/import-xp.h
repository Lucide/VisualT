/**
 * @file
 * The VisualT REXPaint import module header.
 */

#pragma once
#ifndef VISUALT_IMPORT_XP_H
#define VISUALT_IMPORT_XP_H

#include "visualt/visualt.h"
#include "import_xp_export.h"

/**
 * This enumerator is meant to be used with @func{vtInitializeXp}.
 *
 * .xp files are made up of @e n layers of equal size, which together form an image. This structure is similar to the
 * one used by Objects, where each layer corresponds to a Sprite. The only difference is that Sprites can be of
 * different sizes, while layers cannot. This is why the module can work in two modes:
 */
typedef enum VTXpLoadMode {
	VT_XP_FILES, /**<
 * @b{One Sprite per file:} each Sprite is constructed from a different .xp file. This mode allows you to use layers
 * normally in REXPaint, and also to have differently sized Sprites. The downside is that it requires one .xp file per
 * Sprite.
 */
	VT_XP_LAYERS /**<
 * @b{One Sprite per layer:} each Sprite is constructed from a different layer of the same .xp file. This mode allows
 * you to use a single .xp file. The downside is that editing in REXPaint can be less convenient and all Sprites will be
 * the same size.
 */
} VTXpLoadMode;

/**
 * Initializes @c *obj from one or more .xp files.
 *
 * .xp files are created by the REXPaint text-art editor. You may pass one or more files depending on @c loadMode.
 * You also need to pass a @e{charset file} @c mapFile.
 * @param obj a pointer the Object to initialize
 * @param loadMode selects the conversion mode
 * @param mapFile a pointer to the @e{charset file} that will be used to map tiles indices to their corresponding
 * character
 * @param filesLength the length of @c files. In @enumerator{VT_XP_LAYERS} mode must be 1.
 * @param files an array of pointers to .xp files
 * @return A value < 0 if the operation failed. An attempt is always made to close @c mapFile and all @c files, even in
 * case of failure, where @c{*obj} is left uninitialized.
 */
VISUALT_IMPORT_XP_EXPORT int vtInitializeXp(VTObj *obj, VTXpLoadMode loadMode, FILE *restrict mapFile, unsigned int filesLength, FILE *const restrict *files);

#endif //VISUALT_IMPORT_XP_H
