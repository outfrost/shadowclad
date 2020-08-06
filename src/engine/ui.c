#include "ui.h"

#include "_prelude.h"
#include "render.h"

void resizeStage(GLFWwindow* window UNUSED, int width, int height) {
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	viewportAspectRatio = (float) width / (float) height;
}
