#ifndef LEVEL_H_
#define LEVEL_H_

#include <GL/gl.h>

#include "tga.h"

typedef GLuint Block;

const Block BLOCK_EMPTY;
const Block BLOCK_WALL01;

Block get_block(GLushort x, GLushort y);
void set_image(TGAimage* image);
const struct aiScene* import_model(const char* path);

#endif
