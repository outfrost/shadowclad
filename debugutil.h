#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <stdio.h>
#include <assimp/scene.h>

char* getGlInfoString();
void dumpScene(FILE* stream, const struct aiScene* scene);
void dumpNode(FILE* stream, const struct aiNode* node);

#endif
