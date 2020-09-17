/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

#include "engine/asset.h"

enum BlockType {
	BLOCKTYPE_SPACE = 0,
	BLOCKTYPE_OBSTACLE_X = 1 << 0,
	BLOCKTYPE_OBSTACLE_Z = 1 << 1
};

enum Obstacle {
	OBSTACLE_NONE = 0,
	OBSTACLE_XP = 1 << 0,
	OBSTACLE_XN = 1 << 1,
	OBSTACLE_ZP = 1 << 2,
	OBSTACLE_ZN = 1 << 3,
	OBSTACLE_XP_ZP = 1 << 4,
	OBSTACLE_XP_ZN = 1 << 5,
	OBSTACLE_XN_ZP = 1 << 6,
	OBSTACLE_XN_ZN = 1 << 7
};

typedef enum BlockType BlockType;
typedef enum Obstacle Obstacle;
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

static const float BLOCKGRID_CELL_SIZE = 2.5f;

extern BlockGrid levelGrid;

void initLevel();
void startLevel();
GridLocation gridLocationFromPosition(Vector pos);
Obstacle getObstacles(GridLocation loc);

static inline Block* getBlockFromGrid(BlockGrid grid, size_t x, size_t z) {
	return grid.blocks[(z * grid.width) + x];
}

static inline void setBlockInGrid(BlockGrid grid, size_t x, size_t z, Block* block) {
	grid.blocks[(z * grid.width) + x] = block;
}

static inline float cellBoundaryCoord(size_t cellIndex) {
	return cellIndex * BLOCKGRID_CELL_SIZE;
}

#endif // LEVEL_H_
