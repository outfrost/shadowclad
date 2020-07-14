#include "level.h"

#include <stdlib.h>
#include <GL/gl.h>

#include "engine/logger.h"
#include "engine/scene.h"
#include "engine/tga.h"

#include "player.h"

BlockGrid levelGrid;

static const float BLOCKGRID_CELL_SIZE = 2.5f;

static Block blockEmpty = { .type = BLOCKTYPE_SPACE,
                            .solid = NULL };
static Block blockWall01 = { .type = BLOCKTYPE_OBSTACLE,
                             .solid = NULL };

static Transform playerSpawnTransform;

static void buildLevelFromImage(const TgaImage* image);
static inline Block* getBlockFromGrid(BlockGrid grid, size_t x, size_t z);
static inline void setBlockInGrid(BlockGrid grid, size_t x, size_t z, Block* block);



void initLevel() {
	playerSpawnTransform = identity();
	translate(&playerSpawnTransform, (Vector) { .x = -BLOCKGRID_CELL_SIZE,
	                                            .y = 0.0f,
	                                            .z = -BLOCKGRID_CELL_SIZE });

	blockWall01.solid = importSolid("assets/wall01.3ds");

	buildLevelFromImage(readTga("assets/level01.tga"));

	Scene* levelScene = newScene();

	for (size_t z = 0; z < levelGrid.depth; ++z) {
		for (size_t x = 0; x < levelGrid.width; ++x) {
			Scene* blockScene = newScene();
			translate(&blockScene->transform, (Vector) { .x = x * BLOCKGRID_CELL_SIZE,
			                                             .y = 0.0f,
			                                             .z = z * BLOCKGRID_CELL_SIZE });
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
					translate(&playerSpawnTransform, (Vector) { .x = x * BLOCKGRID_CELL_SIZE,
					                                            .y = 0.0f,
					                                            .z = z * BLOCKGRID_CELL_SIZE });
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

static inline Block* getBlockFromGrid(BlockGrid grid, size_t x, size_t z) {
	return grid.blocks[(z * grid.width) + x];
}

static inline void setBlockInGrid(BlockGrid grid, size_t x, size_t z, Block* block) {
	grid.blocks[(z * grid.width) + x] = block;
}
