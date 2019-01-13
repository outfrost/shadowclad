#include <GL/gl.h>

void initRender() {
	// Set the clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	GLfloat light0_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_position[] = {5.0f, 10.0f, 10.0f, 1.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);
}

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
	
	glOrtho(-8.0, 8.0, -8.0/aspectRatio, 8.0/aspectRatio, 128.0, -128.0);
	
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	
	glMatrixMode(GL_MODELVIEW);
}
