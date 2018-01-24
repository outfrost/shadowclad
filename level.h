#ifndef LEVEL_H
#define LEVEL_H

#include <GL/gl.h>

#include "lib/lib3ds.h"
#include "tga.h"

typedef struct {
	GLuint type;
	Lib3dsMesh* mesh;
	Lib3dsMaterial** materials;
	int material_count;
	unsigned int obstacle;
} Block;

const GLuint BLOCK_EMPTY = 0x000000FF;
const GLuint BLOCK_WALL01 = 0xFF0000FF;

Block* get_block_at(GLushort x, GLushort y);
void set_level_image(TGAimage* image);

#endif
