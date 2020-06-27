#include "input.h"

#include <stdbool.h>

#include "render.h"

static void (*keyboardInputCallback) (int, int, int, int);



void onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!(mods & GLFW_MOD_CONTROL)) {
		if (keyboardInputCallback) {
			keyboardInputCallback(key, scancode, action, mods);
		}
		return;
	}

	switch (key) {
		case GLFW_KEY_1:
			if (action == GLFW_PRESS) {
				debugScene = !debugScene;
			}
			break;
		case GLFW_KEY_2:
			if (action == GLFW_PRESS) {
				debugRender = !debugRender;
			}
			break;
		default:
			break;
	}
}

void setKeyboardInputCallback(void (*callback) (int, int, int, int)) {
	keyboardInputCallback = callback;
}
