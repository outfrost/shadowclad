#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include "assimp_types.h"

#include "tga.h"

typedef uint32_t Block;

const AiScene* levelScene;

void initLevel();
void setImage(TgaImage* image);
const AiScene* importScene(const char* path);

#endif
