#include "input.h"

#include <GLFW/glfw3.h>

#include "player.h"

void keyboardInput(int key, int scancode, int action, int mods) {
	switch (key) {
		case GLFW_KEY_W:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_UP);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_UP);
			}
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_DOWN);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_DOWN);
			}
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_LEFT);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_LEFT);
			}
			break;
		case GLFW_KEY_D:
			if (action == GLFW_PRESS) {
				startMovement(DIRECTION_RIGHT);
			}
			else if (action == GLFW_RELEASE) {
				stopMovement(DIRECTION_RIGHT);
			}
			break;
		default:
			break;
	}
}
