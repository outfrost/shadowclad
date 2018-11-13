#ifndef LEVEL_H_
#define LEVEL_H_

#include <GL/gl.h>

#include "tga.h"

typedef GLuint Block;

const Block BLOCK_EMPTY;
const Block BLOCK_WALL01;

Block getBlock(GLushort x, GLushort y);
void setImage(TgaImage* image);
const struct aiScene* importModel(const char* path);

#endif
