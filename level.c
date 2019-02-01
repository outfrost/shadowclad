#include <GL/gl.h>
#include <stdlib.h>

#include "asset.h"
#include "level.h"
#include "logger.h"
#include "player.h"

static Block blockEmpty = { .type = BLOCKTYPE_SPACE,
                            .sceneData = NULL,
                            .textureIds = NULL };
static Block blockWall01 = { .type = BLOCKTYPE_OBSTACLE,
                             .sceneData = NULL,
                             .textureIds = NULL };

static Block* testBlocks[9] = { &blockWall01, &blockWall01, &blockWall01,
                                &blockEmpty, &blockEmpty, &blockEmpty,
                                &blockWall01, &blockEmpty, &blockWall01 };

BlockGrid levelGrid = { .width = 3,
                        .depth = 3,
                        .blocks = testBlocks };

#define DEFAULT_PLAYER_SPAWN_POS { -BLOCKGRID_CELL_SIZE, 0.0f, -BLOCKGRID_CELL_SIZE }
AiVector3D playerSpawnPos = DEFAULT_PLAYER_SPAWN_POS;

static const char* replaceFileExtension(const AiString path, const char* ext);



void initLevel() {
	const AiScene* sceneData = importScene("assets/wall01.3ds");
	blockWall01.sceneData = sceneData;
	if (sceneData != NULL) {
		const unsigned int numTextures = sceneData->mNumMeshes;
		
		blockWall01.textureIds = malloc(numTextures * sizeof(GLuint));
		glGenTextures(numTextures, blockWall01.textureIds);
		
		for (unsigned int i = 0; i < numTextures; ++i) {
			glBindTexture(GL_TEXTURE_2D, blockWall01.textureIds[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			AiString originalTexturePath;
			if (aiGetMaterialTexture(sceneData->mMaterials[sceneData->mMeshes[i]->mMaterialIndex],
			                         aiTextureType_DIFFUSE,
			                         0,
			                         &originalTexturePath,
			                         NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				const char* textureFile = replaceFileExtension(originalTexturePath, ".tga");
				size_t textureFileLength = strlen(textureFile);
				char* texturePath = malloc(strlen("assets/") + textureFileLength + 1);
				strcpy(texturePath, "assets/");
				strncat(texturePath, textureFile, textureFileLength);
				TgaImage* textureImage = readTga(texturePath);
				if (textureImage == NULL) {
					logError("Asset texture file not found: %s", texturePath);
				}
				else {
					glTexImage2D(GL_TEXTURE_2D,
					             0,
					             textureImage->imageComponents,
					             textureImage->header.imageWidth,
					             textureImage->header.imageHeight,
					             0,
					             textureImage->imageFormat,
					             GL_UNSIGNED_BYTE,
					             textureImage->bytes);
					free(textureImage->bytes);
				}
			}
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	buildLevelFromImage(readTga("assets/level01.tga"));
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
	playerSpawnPos = (AiVector3D) DEFAULT_PLAYER_SPAWN_POS;
	
	for (int row = 0; row < newGrid.depth; ++row) {
		for (int x = 0; x < newGrid.width; ++x) {
			// Flip the image vertically due to (0, 0) being bottom left
			int z = newGrid.depth - row - 1;
			
			uint32_t pixelColorARGB = ((uint32_t*) image->bytes)[(row * newGrid.width) + x];
			Block* block;
			switch (pixelColorARGB) {
				case 0xFFFF0000:
					block = &blockWall01;
					break;
				case 0xFF00FFFF:
					block = &blockEmpty;
					playerSpawnPos = (AiVector3D) { x * BLOCKGRID_CELL_SIZE, 0.0f, z * BLOCKGRID_CELL_SIZE };
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

/** BUGS
 * The following function will not work properly with texture
 * file names (excluding directory part) beginning with '.'
 */
static const char* replaceFileExtension(const AiString path, const char* ext) {
		size_t lengthToCopy = path.length;
		
		char* lastDotSubstr = strrchr(path.data, '.');
		if (lastDotSubstr != NULL) {
			if (strpbrk(lastDotSubstr, "\\/") == NULL) {
				lengthToCopy = lastDotSubstr - path.data;
			}
		}
		
		size_t extLength = strlen(ext) + 1;
		char* newPath = malloc(lengthToCopy + extLength);
		strncpy(newPath, path.data, lengthToCopy);
		strncpy(newPath + lengthToCopy, ext, extLength);
		
		return newPath;
}
