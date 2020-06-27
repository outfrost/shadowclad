#include "input.h"

#include <stdbool.h>

static void (*keyboardInputCallback) (int, int, int, int);



void onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool handled = false;

	if (!handled && keyboardInputCallback) {
		keyboardInputCallback(key, scancode, action, mods);
	}
}

void setKeyboardInputCallback(void (*callback) (int, int, int, int)) {
	keyboardInputCallback = callback;
}
