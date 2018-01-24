#include <GL/gl.h>
#include <stdlib.h>

#include "level.h"
#include "tga.h"

const GLuint BLOCK_EMPTY = 0x000000FF;
const GLuint BLOCK_WALL01 = 0xFF0000FF;

TGAimage* level_image = NULL;

Block** blocks;
const int block_count = 1;

void init_blocks() {
	blocks = realloc(blocks, sizeof(Block*) * block_count);
	
	Lib3dsFile* model_file;
	
	model_file = lib3ds_file_open("assets/wall01.3ds");
	Block* wall01block = malloc(sizeof(Block));
	(*wall01block).type = BLOCK_WALL01;
	(*wall01block).mesh = (*model_file).meshes[0];
	(*wall01block).materials = (*model_file).materials;
	(*wall01block).material_count = (*model_file).nmaterials;
	(*wall01block).obstacle = 1;
	
	blocks[0] = wall01block;
}

Block* get_block_at(GLushort x, GLushort y) {
	if (level_image == NULL) {
		return NULL;
	}
	GLuint block_type = ((GLuint*) (*level_image).bytes)[x * (*level_image).header.image_width + y];
	Block* result = NULL;
	for (int i = 0; i < block_count; ++i) {
		if ((*blocks[i]).type == block_type) {
			result = blocks[i];
		}
	}
	return result;
}

void set_level_image(TGAimage* image) {
	level_image = image;
}
