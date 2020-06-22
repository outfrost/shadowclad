#include "input.h"

#include "player.h"

void onKeyPressed(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			playerMovementInput(0.0f, 1.0f);
			break;
		case 's':
			playerMovementInput(0.0f, -1.0f);
			break;
		case 'a':
			playerMovementInput(-1.0f, 0.0f);
			break;
		case 'd':
			playerMovementInput(1.0f, 0.0f);
			break;
		default:
			break;
	}
}

void onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_W:
				playerMovementInput(0.0f, 1.0f);
				break;
			case GLFW_KEY_S:
				playerMovementInput(0.0f, -1.0f);
				break;
			case GLFW_KEY_A:
				playerMovementInput(-1.0f, 0.0f);
				break;
			case GLFW_KEY_D:
				playerMovementInput(1.0f, 0.0f);
				break;
			default:
				break;
		}
	}
}
