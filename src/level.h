#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include "asset.h"
#include "tga.h"

typedef enum {
	BLOCKTYPE_SPACE,
	BLOCKTYPE_OBSTACLE_X,
	BLOCKTYPE_OBSTACLE_Z,
	BLOCKTYPE_OBSTACLE
} BlockType;

typedef struct {
	const BlockType type;
	const Asset3D* asset3D;
} Block;

typedef struct {
	int width;
	int depth;
	Block** blocks;
} BlockGrid;

#define BLOCKGRID_CELL_SIZE 2.5f

BlockGrid levelGrid;
Vector3D playerSpawnPos;

void initLevel();
void buildLevelFromImage(TgaImage* image);

static inline Block* getBlockFromGrid(BlockGrid grid, int x, int z) {
	return grid.blocks[(z * grid.width) + x];
}

static inline void setBlockInGrid(BlockGrid grid, int x, int z, Block* block) {
	grid.blocks[(z * grid.width) + x] = block;
}

#endif