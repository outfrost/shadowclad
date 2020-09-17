/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui.h"

#include "_prelude.h"
#include "render.h"

void resizeStage(GLFWwindow* window UNUSED, int width, int height) {
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	viewportAspectRatio = (float) width / (float) height;
}
