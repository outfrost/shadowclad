#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <stdio.h>

#include "assimp_types.h"

char* getGlInfoString(void);
void dumpScene(FILE* stream, const AiScene* scene);
void dumpNode(FILE* stream, const AiNode* node);

#endif
