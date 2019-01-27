#include <GL/glxew.h>
#include <GL/glut.h>

#include "debugutil.h"
#include "level.h"
#include "logger.h"
#include "performance.h"
#include "render.h"
#include "ui.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	// glutInitContextVersion(4,5); TODO establish correct context
	
	glutInitWindowSize(800, 600);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(NULL);
	
	// glutSetWindowTitle(getGlInfoString());
	glutSetWindowTitle("shadowclad");
	
	logInfo("OpenGL %s", (const char*) glGetString(GL_VERSION));
	logInfo("GLSL %s", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
	logInfo("%s", (const char*) glGetString(GL_RENDERER));
	
	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		logError("GLEW init failed: %s", (const char*) glewGetErrorString(glewInitStatus));
		return 1;
	}
	logInfo("GLEW %s", (const char*) glewGetString(GLEW_VERSION));
	
	if (GLXEW_EXT_swap_control) {
		Display* display = glXGetCurrentDisplay();
		GLXDrawable drawable = glXGetCurrentDrawable();
		if (drawable) {
			glXSwapIntervalEXT(display, drawable, 1);
		}
		else {
			logWarning("Drawable is not here ¯\\_(ツ)_/¯");
			logWarning("Could not enable vsync (GLX_EXT_swap_control)");
		}
	}
	else if (GLXEW_MESA_swap_control) {
		glXSwapIntervalMESA(1);
		log("Vsync enabled with GLX_MESA_swap_control, swap interval %d", glXGetSwapIntervalMESA());
	}
	else {
		logWarning("Could not enable vsync (extensions not supported)");
	}
	
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resizeStage);
	//glutKeyboardFunc(key_pressed);
	//glutMouseFunc(mouse_button_event);
	//glutMotionFunc(mouse_motion_event);
	
	initRender();
	initPerformanceMetering();
	initLevel();
	
	glutMainLoop();
	return 0;
}
