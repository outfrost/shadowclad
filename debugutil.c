#include <GL/gl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char * get_gl_info() {
	const char * gl_version = (const char *) glGetString(GL_VERSION);
	const char * gl_sl_version = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
	const char * gl_renderer = (const char *) glGetString(GL_RENDERER);
	
	size_t gl_info_length = strlen("OpenGL  - GLSL  - ")
				+ strlen(gl_version)
				+ strlen(gl_sl_version)
				+ strlen(gl_renderer);
	
	char * gl_info = malloc(gl_info_length + sizeof(char));
	sprintf(gl_info,
		    "OpenGL %s - GLSL %s - %s",
			gl_version,
			gl_sl_version,
			gl_renderer);
	
	return gl_info;
}
