#include <GL/gl.h>

void resizeStage(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	
	// Set device viewport dimensions
	glViewport(0, 0, width, height);
	// Switch to projection matrix
	glMatrixMode(GL_PROJECTION);
	// Reset the projection matrix
	glLoadIdentity();
	
	GLfloat aspectRatio = (GLfloat) width / (GLfloat) height;
	
	glOrtho(-8.0, 24.0, -16.0/aspectRatio, 16.0/aspectRatio, -128.0, 128.0);
	
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	
	glMatrixMode(GL_MODELVIEW);
}
