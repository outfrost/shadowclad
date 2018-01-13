#include <GL/gl.h>

void init_render() {
	// Set the clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize_stage(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	
	// Set device viewport dimensions
	glViewport(0, 0, width, height);
	// Switch to projection matrix
	glMatrixMode(GL_PROJECTION);
	// Reset the projection matrix
	glLoadIdentity();
	
	GLfloat aspect_ratio = (GLfloat)width / (GLfloat)height;
	
	glOrtho(-10.0f, 10.0f, -10.0f/aspect_ratio, 10.0f/aspect_ratio, 128.0, -128.0);
	
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	
	glMatrixMode(GL_MODELVIEW);
}
