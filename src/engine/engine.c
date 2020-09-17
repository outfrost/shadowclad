/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "engine.h"

#include <stdlib.h>
#include <assimp/version.h>
#include <GL/glxew.h>
#include <GLFW/glfw3.h>

#include "_prelude.h"
#include "input.h"
#include "logger.h"
#include "performance.h"
#include "render.h"
#include "ui.h"

// static const int EXIT_OK = 0;
static const int EXIT_LIB_FAIL = 1;
static const int EXIT_CTX_FAIL = 2;

static GLFWwindow* window;

static void onGlfwError(int error, const char* description);



void init(EngineConfig config) {
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

	window = glfwCreateWindow(config.windowWidth,
	                          config.windowHeight,
	                          config.windowTitle.cstr,
	                          NULL,
	                          NULL);

	if (!window) {
		logError("Window or context creation failed");
		glfwTerminate();
		exit(EXIT_CTX_FAIL);
	}

	glfwMakeContextCurrent(window);

	logInfo("OpenGL %s", (const char*) glGetString(GL_VERSION));
	logInfo("GLSL %s", (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
	logInfo("Renderer: %s", (const char*) glGetString(GL_RENDERER));

	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK) {
		logError("GLEW init failed: %s", (const char*) glewGetErrorString(glewInitStatus));
		exit(EXIT_LIB_FAIL);
	}

	logInfo("Setting swap interval: %d", config.swapInterval);
	glfwSwapInterval(config.swapInterval);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	resizeStage(window, width, height);

	glfwSetFramebufferSizeCallback(window, resizeStage);
	glfwSetKeyCallback(window, onKeyboardEvent);

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

EngineConfig defaultConfig() {
	return (EngineConfig) { .windowWidth = 800,
	                        .windowHeight = 600,
	                        .windowTitle = newString(NULL),
	                        .swapInterval = 1 };
}

static void onGlfwError(int error UNUSED, const char* description) {
	logError("GLFW error: %s", description);
}
