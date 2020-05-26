#ifndef RENDER_H_
#define RENDER_H_

#include "scene.h"

float viewportAspectRatio;
const Scene* cameraAnchor;

void initRender();
void renderFrame();

#endif
