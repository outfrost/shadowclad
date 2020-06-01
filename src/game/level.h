#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include "engine/asset.h"
#include "engine/tga.h"

typedef enum {
	BLOCKTYPE_SPACE,
	BLOCKTYPE_OBSTACLE_X,
	BLOCKTYPE_OBSTACLE_Z,
	BLOCKTYPE_OBSTACLE
} BlockType;

typedef struct {
	const BlockType type;
	const Solid* solid;
} Block;

typedef struct {
	size_t width;
	size_t depth;
	Block** blocks;
} BlockGrid;

#define BLOCKGRID_CELL_SIZE 2.5f

extern BlockGrid levelGrid;

void initLevel();
void startLevel();
void buildLevelFromImage(TgaImage* image);

static inline Block* getBlockFromGrid(BlockGrid grid, size_t x, size_t z) {
	return grid.blocks[(z * grid.width) + x];
}

static inline void setBlockInGrid(BlockGrid grid, size_t x, size_t z, Block* block) {
	grid.blocks[(z * grid.width) + x] = block;
}

#endif // LEVEL_H_
