#ifndef RENDER_H_
#define RENDER_H_

#include "assimp_types.h"

void initRender(void);
void renderScene(void);
void drawAxes(void);
void drawSceneRecursive(const AiScene* scene, const AiNode* node);

#endif
