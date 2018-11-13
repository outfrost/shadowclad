#include <GL/gl.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <stdlib.h>
#include <stdio.h> // TODO remove

#include "level.h"
#include "tga.h"

const Block BLOCK_EMPTY = 0;
const Block BLOCK_WALL01 = 1;

TGAimage* level_image = NULL;

Block get_block(GLushort x, GLushort y) {
	if (level_image == NULL) {
		return BLOCK_EMPTY;
	}
	return ((Block*) (*level_image).bytes)[x * (*level_image).header.image_width + y];
}

void set_image(TGAimage* image) {
	level_image = image;
}

const struct aiScene* import_model(const char* path) {
	const struct aiScene* scene = aiImportFile(path, 0u);
	if (scene == NULL) {
		fprintf(stderr, "Asset import failed at file %s\n", path); // TODO factor logging the heck outta here
	}
	return scene;
	// TODO aiReleaseImport(scene);
}
