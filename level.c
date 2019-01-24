#include <GL/gl.h>
#include <assimp/cimport.h>
#include <stdio.h> // TODO remove

#include "level.h"

const AiScene* levelScene = NULL;

static const Block BLOCK_EMPTY = 0;
static const Block BLOCK_WALL01 = 0xFF0000FF; // red

static const AiScene* blockWall01 = NULL;

static TgaImage* levelImage = NULL;

void initLevel() {
	blockWall01 = importScene("out/assets/wall01.3ds");
	levelScene = blockWall01;
}

void setImage(TgaImage* image) {
	levelImage = image;
}

const AiScene* importScene(const char* path) {
	const AiScene* scene = aiImportFile(path, 0u);
	if (scene == NULL) {
		fprintf(stderr, "Asset import failed at file %s\n", path); // TODO factor logging the heck outta here
	}
	return scene;
	// TODO aiReleaseImport(scene);
}
