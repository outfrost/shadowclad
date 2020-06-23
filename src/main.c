#include <assimp/version.h>
#include <GL/glxew.h>
#include <GLFW/glfw3.h>

#include "engine/logger.h"
#include "engine/performance.h"
#include "engine/render.h"
#include "engine/ui.h"

#include "game/input.h"
#include "game/level.h"
#include "game/player.h"

void onGlfwError(int error, const char* description);

int main(/*int argc, char** argv*/) {
	logInfo("Assimp %u.%u", aiGetVersionMajor(), aiGetVersionMinor());
	logInfo("GLEW %s", (const char*) glewGetString(GLEW_VERSION));
	logInfo("GLFW %s", glfwGetVersionString());

	glfwSetErrorCallback(onGlfwError);

	if (!glfwInit()) {
		logError("GLFW init failed");
		return 1;
	}
	// glutInitContextVersion(4,5); TODO establish correct context
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "shadowclad", NULL, NULL);
	if (!window) {
		logError("Window or context creation failed");
		glfwTerminate();
		return 2;
	}

	glfwMakeContextCurrent(window);
	//glutInitDisplayMode(//glut_DOUBLE | //glut_RGBA | //glut_DEPTH);

	logInfo("OpenGL %s", (const char*) glGetString(GL_VERSION));
	logInfo("GLSL %s", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
	logInfo("%s", (const char*) glGetString(GL_RENDERER));

	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		logError("GLEW init failed: %s", (const char*) glewGetErrorString(glewInitStatus));
		return 1;
	}
/*
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
		logDebug("Vsync enabled with GLX_MESA_swap_control, swap interval %d", glXGetSwapIntervalMESA());
	}
	else {
		logWarning("Could not enable vsync (extensions not supported)");
	}
*/
	logInfo("Setting swap interval to 1");
	glfwSwapInterval(1);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	resizeStage(window, width, height);

	glfwSetFramebufferSizeCallback(window, resizeStage);
	glfwSetKeyCallback(window, onKeyboardEvent);

	initRender();
	//initPerformanceMetering();
	initLevel();
	initPlayer();
	startLevel();

	while (!glfwWindowShouldClose(window)) {
		renderFrame(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void onGlfwError(int error, const char* description) {
	logError("GLFW error: %s", description);
}
