#include <GL/gl.h>
#include <stdlib.h>

#include "engine/logger.h"
#include "engine/scene.h"

#include "level.h"
#include "player.h"

static Block blockEmpty = { .type = BLOCKTYPE_SPACE,
                            .solid = NULL };
static Block blockWall01 = { .type = BLOCKTYPE_OBSTACLE,
                             .solid = NULL };

static Block* testBlocks[9] = { &blockWall01, &blockWall01, &blockWall01,
                                &blockEmpty, &blockEmpty, &blockEmpty,
                                &blockWall01, &blockEmpty, &blockWall01 };

BlockGrid levelGrid = { .width = 3,
                        .depth = 3,
                        .blocks = testBlocks };

#define DEFAULT_PLAYER_SPAWN_POS { -BLOCKGRID_CELL_SIZE, 0.0f, -BLOCKGRID_CELL_SIZE }
Vector3D playerSpawnPos = DEFAULT_PLAYER_SPAWN_POS;



void initLevel() {
	blockWall01.solid = importSolid("assets/wall01.3ds");
	
	buildLevelFromImage(readTga("assets/level01.tga"));
	
	Scene* levelScene = newScene();
	
	for (size_t z = 0; z < levelGrid.depth; ++z) {
		for (size_t x = 0; x < levelGrid.depth; ++x) {
			Scene* blockScene = newScene();
			translate(&blockScene->transform, (Vector3D) { .x = x * BLOCKGRID_CELL_SIZE,
			                                               .y = 0.0f,
			                                               .z = z * BLOCKGRID_CELL_SIZE });
			blockScene->solid = getBlockFromGrid(levelGrid, x, z)->solid;
			insertChildScene(levelScene, blockScene);
		}
	}

	currentScene = levelScene;
}

void buildLevelFromImage(TgaImage* image) {
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
	playerSpawnPos = (Vector3D) DEFAULT_PLAYER_SPAWN_POS;
	
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
					playerSpawnPos = (Vector3D) { x * BLOCKGRID_CELL_SIZE, 0.0f, z * BLOCKGRID_CELL_SIZE };
					break;
				default:
					block = &blockEmpty;
					break;
			}
			setBlockInGrid(newGrid, x, z, block);
		}
	}
	
	levelGrid = newGrid;
	spawnPlayer();
}
