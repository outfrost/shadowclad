#include <GL/gl.h>
#include <stdlib.h>

#include "level.h"
#include "tga.h"

TGAimage* level_image = NULL;

Block get_block(GLushort x, GLushort y) {
	if (level_image == NULL) {
		return BLOCK_EMPTY;
	}
	return ((Block*) (*level_image).bytes)[x * (*level_image).header.image_width + y];
}

void set_image(TGAimage* image) {
	level_image = image;
}
