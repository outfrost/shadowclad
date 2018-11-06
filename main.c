#include <GL/glut.h>

#include "debugutil.h"
#include "glut_janitor.h"
#include "render.h"
#include "level.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitWindowSize(640, 480);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(NULL);
	
	glutSetWindowTitle(getGlInfoString());
	
	glutDisplayFunc(render_scene);
	glutReshapeFunc(resize_stage);
	//glutKeyboardFunc(key_pressed);
	//glutMouseFunc(mouse_button_event);
	//glutMotionFunc(mouse_motion_event);
	
	init_render();
	
	model = import_model("out/assets/wall01.3ds");
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
