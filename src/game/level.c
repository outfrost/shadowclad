/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "level.h"

#include <stdlib.h>
#include <GL/gl.h>

#include "engine/logger.h"
#include "engine/scene.h"
#include "engine/tga.h"

#include "player.h"

BlockGrid levelGrid;

static Block blockEmpty = { .type = BLOCKTYPE_SPACE,
                            .solid = NULL };
static Block blockWall01 = { .type = BLOCKTYPE_OBSTACLE_X | BLOCKTYPE_OBSTACLE_Z,
                             .solid = NULL };

static Transform playerSpawnTransform;

static void buildLevelFromImage(const TgaImage* image);



void initLevel() {
	playerSpawnTransform = identity();
	translate(&playerSpawnTransform, (Vector) { -BLOCKGRID_CELL_SIZE,
	                                            0.0f,
	                                            -BLOCKGRID_CELL_SIZE });

	blockWall01.solid = importSolid("assets/wall01.3ds");

	buildLevelFromImage(readTga("assets/level01.tga"));

	Scene* levelScene = newScene();

	for (size_t z = 0; z < levelGrid.depth; ++z) {
		for (size_t x = 0; x < levelGrid.width; ++x) {
			Scene* blockScene = newScene();
			translate(&blockScene->transform, (Vector) {
				(x * BLOCKGRID_CELL_SIZE) + (BLOCKGRID_CELL_SIZE * 0.5f),
				0.0f,
				(z * BLOCKGRID_CELL_SIZE) + (BLOCKGRID_CELL_SIZE * 0.5f) });
			blockScene->solid = getBlockFromGrid(levelGrid, x, z)->solid;
			insertChildScene(levelScene, blockScene);
		}
	}

	currentScene = levelScene;
}

void startLevel() {
	spawnPlayer(playerSpawnTransform);
}

static void buildLevelFromImage(const TgaImage* image) {
	if (image == NULL) {
		logError("Null image received, cannot build level");
		return;
	}

	if (image->header.imageBpp != 32) {
		logError("Invalid level image format (%d bpp)", image->header.imageBpp);
		return;
	}

	BlockGrid newGrid = { .width = image->header.imageWidth,
	                      .depth = image->header.imageHeight,
	                      .blocks = malloc(image->header.imageWidth
	                                       * image->header.imageHeight
	                                       * sizeof(Block*)) };

	for (size_t row = 0; row < newGrid.depth; ++row) {
		for (size_t x = 0; x < newGrid.width; ++x) {
			// Flip the image vertically due to (0, 0) being bottom left
			size_t z = newGrid.depth - row - 1;

			uint32_t pixelColorARGB = ((uint32_t*) image->bytes)[(row * newGrid.width) + x];
			Block* block;
			switch (pixelColorARGB) {
				case 0xFFFF0000:
					block = &blockWall01;
					break;
				case 0xFF00FFFF:
					block = &blockEmpty;
					playerSpawnTransform = identity();
					translate(&playerSpawnTransform, (Vector) {
						(x * BLOCKGRID_CELL_SIZE) + (BLOCKGRID_CELL_SIZE * 0.5f),
						0.0f,
						(z * BLOCKGRID_CELL_SIZE) + (BLOCKGRID_CELL_SIZE * 0.5f) });
					break;
				default:
					block = &blockEmpty;
					break;
			}
			setBlockInGrid(newGrid, x, z, block);
		}
	}

	levelGrid = newGrid;
}

static inline size_t nonNegative(long n) {
	return n < 0 ? 0u : n;
}

GridLocation gridLocationFromPosition(Vector pos) {
	Vector scaledPos = scaleVector(pos, 1.0f / BLOCKGRID_CELL_SIZE);
	return (GridLocation) { .x = nonNegative(scaledPos.x),
	                        .z = nonNegative(scaledPos.z) };
}

Obstacle getObstacles(GridLocation loc) {
	Obstacle result = OBSTACLE_NONE;
	if (loc.x == 0) {
		result |= OBSTACLE_XN | OBSTACLE_XN_ZP | OBSTACLE_XN_ZN;
	}
	if (loc.x >= levelGrid.width - 1) {
		result |= OBSTACLE_XP | OBSTACLE_XP_ZP | OBSTACLE_XP_ZN;
	}
	if (loc.z == 0) {
		result |= OBSTACLE_ZN | OBSTACLE_XP_ZN | OBSTACLE_XN_ZN;
	}
	if (loc.z >= levelGrid.depth - 1) {
		result |= OBSTACLE_ZP | OBSTACLE_XP_ZP | OBSTACLE_XN_ZP;
	}
	if (!(result & OBSTACLE_XP)
	    && getBlockFromGrid(levelGrid, loc.x + 1, loc.z)->type & BLOCKTYPE_OBSTACLE_X) {
		result |= OBSTACLE_XP;
	}
	if (!(result & OBSTACLE_XN)
	    && getBlockFromGrid(levelGrid, loc.x - 1, loc.z)->type & BLOCKTYPE_OBSTACLE_X) {
		result |= OBSTACLE_XN;
	}
	if (!(result & OBSTACLE_ZP)
	    && getBlockFromGrid(levelGrid, loc.x, loc.z + 1)->type & BLOCKTYPE_OBSTACLE_Z) {
		result |= OBSTACLE_ZP;
	}
	if (!(result & OBSTACLE_ZN)
	    && getBlockFromGrid(levelGrid, loc.x, loc.z - 1)->type & BLOCKTYPE_OBSTACLE_Z) {
		result |= OBSTACLE_ZN;
	}
	if (!(result & OBSTACLE_XP_ZP)
	    && getBlockFromGrid(levelGrid, loc.x + 1, loc.z + 1)->type
	       & (BLOCKTYPE_OBSTACLE_X | BLOCKTYPE_OBSTACLE_Z)) {
		result |= OBSTACLE_XP_ZP;
	}
	if (!(result & OBSTACLE_XP_ZN)
	    && getBlockFromGrid(levelGrid, loc.x + 1, loc.z - 1)->type
	       & (BLOCKTYPE_OBSTACLE_X | BLOCKTYPE_OBSTACLE_Z)) {
		result |= OBSTACLE_XP_ZN;
	}
	if (!(result & OBSTACLE_XN_ZP)
	    && getBlockFromGrid(levelGrid, loc.x - 1, loc.z + 1)->type
	       & (BLOCKTYPE_OBSTACLE_X | BLOCKTYPE_OBSTACLE_Z)) {
		result |= OBSTACLE_XN_ZP;
	}
	if (!(result & OBSTACLE_XN_ZN)
	    && getBlockFromGrid(levelGrid, loc.x - 1, loc.z - 1)->type
	       & (BLOCKTYPE_OBSTACLE_X | BLOCKTYPE_OBSTACLE_Z)) {
		result |= OBSTACLE_XN_ZN;
	}
	return result;
}
