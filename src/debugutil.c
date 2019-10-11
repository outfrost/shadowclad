#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getGlInfoString() {
	const char* glVersion = (const char*) glGetString(GL_VERSION);
	const char* glslVersion = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
	const char* glRenderer = (const char*) glGetString(GL_RENDERER);
	
	size_t glInfoLength = strlen("OpenGL  - GLSL  - ")
	                      + strlen(glVersion)
	                      + strlen(glslVersion)
	                      + strlen(glRenderer);
	
	char* glInfoString = malloc(glInfoLength + sizeof(char));
	sprintf(glInfoString,
		    "OpenGL %s - GLSL %s - %s",
			glVersion,
			glslVersion,
			glRenderer);
	
	return glInfoString;
}
