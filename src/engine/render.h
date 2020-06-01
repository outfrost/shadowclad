#ifndef RENDER_H_
#define RENDER_H_

#include "scene.h"

extern float viewportAspectRatio;
extern const Scene* cameraAnchor;

void initRender();
void renderFrame();

#endif // RENDER_H_
