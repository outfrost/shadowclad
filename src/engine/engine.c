#include "engine.h"

#include <stdlib.h>
#include <assimp/version.h>
#include <GL/glxew.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "performance.h"
#include "render.h"
#include "ui.h"

// static const int EXIT_OK = 0;
static const int EXIT_LIB_FAIL = 1;
static const int EXIT_CTX_FAIL = 2;

static GLFWwindow* window;

static void onGlfwError(int error, const char* description);



void init() {
	if (window) {
		logError("init called more than once");
		return;
	}

	logInfo("Assimp %u.%u", aiGetVersionMajor(), aiGetVersionMinor());
	logInfo("GLEW %s", (const char*) glewGetString(GLEW_VERSION));
	logInfo("GLFW %s", glfwGetVersionString());

	glfwSetErrorCallback(onGlfwError);

	if (!glfwInit()) {
		logError("GLFW init failed");
		exit(EXIT_LIB_FAIL);
	}
	// glutInitContextVersion(4,5); TODO establish correct context
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	window = glfwCreateWindow(1280, 720, "shadowclad", NULL, NULL);
	if (!window) {
		logError("Window or context creation failed");
		glfwTerminate();
		exit(EXIT_CTX_FAIL);
	}

	glfwMakeContextCurrent(window);

	logInfo("OpenGL %s", (const char*) glGetString(GL_VERSION));
	logInfo("GLSL %s", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
	logInfo("%s", (const char*) glGetString(GL_RENDERER));

	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		logError("GLEW init failed: %s", (const char*) glewGetErrorString(glewInitStatus));
		exit(EXIT_LIB_FAIL);
	}

	logInfo("Setting swap interval to 1");
	glfwSwapInterval(1);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	resizeStage(window, width, height);

	glfwSetFramebufferSizeCallback(window, resizeStage);

	initRender();
	//initPerformanceMetering();
}

void run(void (*updateFn) (float)) {
	if (!updateFn) {
		logError("No update function provided");
		return;
	}

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();
		delta = time - lastTime;
		lastTime = time;

		updateFn(delta);

		renderFrame(window);
		glfwPollEvents();
	}
}

void terminate() {
	glfwTerminate();
}

void setKeyboardEventCallback(void (*keyboardEventCallback) (GLFWwindow*, int, int, int, int)) {
	glfwSetKeyCallback(window, keyboardEventCallback);
}

static void onGlfwError(int error, const char* description) {
	logError("GLFW error: %s", description);
}
