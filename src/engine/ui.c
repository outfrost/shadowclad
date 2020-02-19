#include <GL/gl.h>

#include "render.h"

void resizeStage(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	
	glViewport(0, 0, width, height);
	
	viewportAspectRatio = (float) width / (float) height;
}
