#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h> // TODO remove
#include <assimp/cimport.h>
#include <assimp/scene.h>

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

void import_model(const char* path) {
	const struct aiScene* scene = aiImportFile(path, 0u);
	if (scene == NULL) {
		printf("We have nothing.\n");
		return;
	}
	printf("mFlags = %u\nmRootNode = %p\nmNumMeshes = %u\nmMeshes = %p\nmNumMaterials = %u\nmMaterials = %p\nmNumAnimations = %u\nmAnimations = %p\nmNumTextures = %u\nmTextures = %p\nmNumLights = %u\nmLights = %p\n",
			(*scene).mFlags,
			(*scene).mRootNode,
			(*scene).mNumMeshes,
			(*scene).mMeshes,
			(*scene).mNumMaterials,
			(*scene).mMaterials,
			(*scene).mNumAnimations,
			(*scene).mAnimations,
			(*scene).mNumTextures,
			(*scene).mTextures,
			(*scene).mNumLights,
			(*scene).mLights);
	aiReleaseImport(scene);
}
