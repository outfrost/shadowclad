#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <stdio.h>
#include <assimp/scene.h>

char * get_gl_info();
void print_struct_aiScene(FILE* stream, const struct aiScene* scene);
void print_struct_aiNode(FILE* stream, const struct aiNode* node);

#endif
