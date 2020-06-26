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

	logInfo("OpenGL %s", (const char*) glGetString(GL_VERSION));
	logInfo("GLSL %s", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
	logInfo("%s", (const char*) glGetString(GL_RENDERER));

	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		logError("GLEW init failed: %s", (const char*) glewGetErrorString(glewInitStatus));
		return 1;
	}

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

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();
		delta = time - lastTime;
		lastTime = time;

		updatePlayer(delta);
		renderFrame(window);
		glfwPollEvents();

		int wState = glfwGetKey(window, GLFW_KEY_W);
		int sState = glfwGetKey(window, GLFW_KEY_S);
		int aState = glfwGetKey(window, GLFW_KEY_A);
		int dState = glfwGetKey(window, GLFW_KEY_D);

		logDebug("%d %d %d %d", wState, sState, aState, dState);
	}

	glfwTerminate();
	return 0;
}

void onGlfwError(int error, const char* description) {
	logError("GLFW error: %s", description);
}
