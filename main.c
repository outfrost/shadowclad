#include <GL/glut.h>

#include "debugutil.h"
#include "glut_janitor.h"
#include "render.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitWindowSize(640, 480);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(NULL);
	
	glutSetWindowTitle(get_gl_info());
	
	glutDisplayFunc(render_scene);
	glutReshapeFunc(resize_stage);
	//glutKeyboardFunc(key_pressed);
	//glutMouseFunc(mouse_button_event);
	//glutMotionFunc(mouse_motion_event);
	
	init_render();
	
	glutMainLoop();
	return 0;
}
