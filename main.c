#include <GL/glxew.h>
#include <GL/glut.h>
#include <stdio.h>

#include "debugutil.h"
#include "glut_janitor.h"
#include "render.h"
#include "level.h"
#include "performance.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	// glutInitContextVersion(4,5); TODO establish correct context
	
	glutInitWindowSize(800, 600);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(NULL);
	
	glutSetWindowTitle(getGlInfoString());
	
	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(glewInitStatus));
		return 1;
	}
	printf("GLEW %s\n", glewGetString(GLEW_VERSION));
	
	if (GLXEW_EXT_swap_control) {
		Display* display = glXGetCurrentDisplay();
		GLXDrawable drawable = glXGetCurrentDrawable();
		if (drawable) {
			glXSwapIntervalEXT(display, drawable, 1);
		}
		else {
			fprintf(stderr, "Drawable is not here\n");
		}
	}
	else if (GLXEW_MESA_swap_control) {
		glXSwapIntervalMESA(1);
		printf("Swap interval %d\n", glXGetSwapIntervalMESA());
	}
	else {
		fprintf(stderr, "Could not enable vsync\n");
	}
	
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resizeStage);
	//glutKeyboardFunc(key_pressed);
	//glutMouseFunc(mouse_button_event);
	//glutMotionFunc(mouse_motion_event);
	
	initRender();
	initPerformanceMetering();
	initLevel();
	
	/*
	fprintf(stderr, "*model = ");
	print_struct_aiScene(stderr, model);
	fprintf(stderr, "\n*(*model).mRootNode = ");
	print_struct_aiNode(stderr, (*model).mRootNode);
	fprintf(stderr, "\n");
	*/
	glutMainLoop();
	return 0;
}
