#ifndef RENDER_H_
#define RENDER_H_

#include <GLFW/glfw3.h>

#include "scene.h"

extern float viewportAspectRatio;
extern const Scene* cameraAnchor;

void initRender();
void renderFrame(GLFWwindow* window);

#endif // RENDER_H_
