#include <GL/gl.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <stdlib.h>
#include <stdio.h> // TODO remove

#include "level.h"
#include "tga.h"

const Block BLOCK_EMPTY = 0;
const Block BLOCK_WALL01 = 1;

TgaImage* levelImage = NULL;

Block getBlock(GLushort x, GLushort y) {
	if (levelImage == NULL) {
		return BLOCK_EMPTY;
	}
	return ((Block*) (*levelImage).bytes)[x * (*levelImage).header.imageWidth + y];
}

void setImage(TgaImage* image) {
	levelImage = image;
}

const struct aiScene* importModel(const char* path) {
	const struct aiScene* scene = aiImportFile(path, 0u);
	if (scene == NULL) {
		fprintf(stderr, "Asset import failed at file %s\n", path); // TODO factor logging the heck outta here
	}
	return scene;
	// TODO aiReleaseImport(scene);
}
