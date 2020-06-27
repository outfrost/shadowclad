#ifndef ENGINE_RENDER_H_
#define ENGINE_RENDER_H_

#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "scene.h"

extern float viewportAspectRatio;
extern const Scene* cameraAnchor;
extern bool debugScene;
extern bool debugRender;

void initRender();
void renderFrame(GLFWwindow* window);

#endif // ENGINE_RENDER_H_
