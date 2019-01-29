#include <GL/gl.h>
#include <assimp/cimport.h>
#include <stdlib.h>

#include "level.h"
#include "logger.h"

BlockGrid levelGrid;

static Block blockEmpty = { .type = BLOCKTYPE_SPACE,
                            .sceneData = NULL,
                            .textureIds = NULL };
static Block blockWall01 = { .type = BLOCKTYPE_OBSTACLE,
                             .sceneData = NULL,
                             .textureIds = NULL };

//static TgaImage* levelImage = NULL;

static const char* replaceFileExtension(const AiString path, const char* ext);

void initLevel() {
	const AiScene* sceneData = importScene("out/assets/wall01.3ds");
	blockWall01.sceneData = sceneData;
	if (sceneData != NULL) {
		const unsigned int numTextures = sceneData->mNumMeshes;
		
		blockWall01.textureIds = malloc(numTextures * sizeof(GLuint));
		glGenTextures(numTextures, blockWall01.textureIds);
		
		for (unsigned int i = 0; i < numTextures; ++i) {
			glBindTexture(GL_TEXTURE_2D, blockWall01.textureIds[i]);
			
			AiString originalTexturePath;
			if (aiGetMaterialTexture(sceneData->mMaterials[sceneData->mMeshes[i]->mMaterialIndex],
			                         aiTextureType_DIFFUSE,
			                         0,
			                         &originalTexturePath,
			                         NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				const char* textureFile = replaceFileExtension(originalTexturePath, ".tga");
				size_t textureFileLength = strlen(textureFile);
				char* texturePath = malloc(strlen("out/assets/") + textureFileLength + 1);
				strcpy(texturePath, "out/assets/");
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
}

void buildLevelFromImage(TgaImage* image) {
	if (image->header.imageBpp != 32) {
		logError("Invalid level image format (%d bpp)", image->header.imageBpp);
		return;
	}
}

const AiScene* importScene(const char* path) {
	const AiScene* scene = aiImportFile(path, 0u);
	if (scene == NULL) {
		logError("Failed to import asset from %s", path);
	}
	else if ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE) {
		logError("Incomplete scene imported from %s", path);
	}
	return scene;
	// TODO aiReleaseImport(scene);
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
