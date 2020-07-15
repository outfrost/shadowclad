#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include "engine/asset.h"

enum BlockType {
	BLOCKTYPE_SPACE,
	BLOCKTYPE_OBSTACLE_X,
	BLOCKTYPE_OBSTACLE_Z,
	BLOCKTYPE_OBSTACLE
};

typedef enum BlockType BlockType;
typedef struct Block Block;
typedef struct BlockGrid BlockGrid;
typedef struct GridLocation GridLocation;

struct Block {
	BlockType type;
	const Solid* solid;
};

struct BlockGrid {
	size_t width;
	size_t depth;
	Block** blocks;
};

struct GridLocation {
	size_t x;
	size_t z;
};

extern BlockGrid levelGrid;

void initLevel();
void startLevel();
GridLocation gridLocationFromTransform(Transform transform);

static inline Block* getBlockFromGrid(BlockGrid grid, size_t x, size_t z) {
	return grid.blocks[(z * grid.width) + x];
}

static inline void setBlockInGrid(BlockGrid grid, size_t x, size_t z, Block* block) {
	grid.blocks[(z * grid.width) + x] = block;
}

#endif // LEVEL_H_
