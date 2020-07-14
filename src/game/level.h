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

struct Block {
	BlockType type;
	const Solid* solid;
};

struct BlockGrid {
	size_t width;
	size_t depth;
	Block** blocks;
};

extern BlockGrid levelGrid;

void initLevel();
void startLevel();

#endif // LEVEL_H_
