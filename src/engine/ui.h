/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_UI_H_
#define ENGINE_UI_H_

#include <GL/gl.h>

typedef struct GLFWwindow GLFWwindow;

void resizeStage(GLFWwindow* window, int width, int height);

#endif // ENGINE_UI_H_
