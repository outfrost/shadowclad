#include <GL/glut.h>

#include "debugutil.h"
#include "glut_janitor.h"
#include "render.h"
#include "level.h"
#include "performance.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitWindowSize(800, 600);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(NULL);
	
	glutSetWindowTitle(getGlInfoString());
	
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resizeStage);
	//glutKeyboardFunc(key_pressed);
	//glutMouseFunc(mouse_button_event);
	//glutMotionFunc(mouse_motion_event);
	
	initRender();
	initPerformanceMetering();
	
	model = importModel("out/assets/wall01.3ds");
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
