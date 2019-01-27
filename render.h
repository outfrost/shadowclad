#ifndef RENDER_H_
#define RENDER_H_

#include "assimp_types.h"

void initRender();
void renderScene();
void drawAxes();
void drawSceneRecursive(const AiScene* scene, const AiNode* node);

#endif
