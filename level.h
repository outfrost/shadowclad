#ifndef LEVEL_H_
#define LEVEL_H_

#include <GL/gl.h>

#include "tga.h"

typedef GLuint Block;

const Block BLOCK_EMPTY = 0;
const Block BLOCK_WALL01 = 1;

Block get_block(GLushort x, GLushort y);
void set_image(TGAimage* image);

#endif
