#ifndef INPUT_H_
#define INPUT_H_

#include <GLFW/glfw3.h>

void onKeyPressed(unsigned char key, int x, int y);
void onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif // INPUT_H_
