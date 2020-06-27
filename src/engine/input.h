#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <GLFW/glfw3.h>

void onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
void setKeyboardInputCallback(void (*) (int, int, int, int));

#endif // ENGINE_INPUT_H
