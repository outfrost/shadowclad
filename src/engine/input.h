/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <GLFW/glfw3.h>

void onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
void setKeyboardInputCallback(void (*) (int, int, int, int));

#endif // ENGINE_INPUT_H
