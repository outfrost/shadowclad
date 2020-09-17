/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "input.h"

#include <GLFW/glfw3.h>

#include "engine/_prelude.h"

#include "player.h"

void keyboardInput(int key, int scancode UNUSED, int action, int mods UNUSED) {
	switch (key) {
		case GLFW_KEY_W:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_UP);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_UP);
			}
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_DOWN);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_DOWN);
			}
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_LEFT);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_LEFT);
			}
			break;
		case GLFW_KEY_D:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_RIGHT);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_RIGHT);
			}
			break;
		default:
			break;
	}
}
