/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
