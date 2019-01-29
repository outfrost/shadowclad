#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include "assimp_types.h"

#include "tga.h"

typedef enum {
	BLOCKTYPE_SPACE,
	BLOCKTYPE_OBSTACLE_X,
	BLOCKTYPE_OBSTACLE_Z,
	BLOCKTYPE_OBSTACLE
} BlockType;

typedef struct {
	const BlockType type;
	const AiScene* sceneData;
	GLuint* textureIds;
} Block;

typedef struct {
	int width;
	int height;
	Block* blocks;
} BlockGrid;

BlockGrid levelGrid;

void initLevel();
void buildLevelFromImage(TgaImage* image);
const AiScene* importScene(const char* path);

#endif
