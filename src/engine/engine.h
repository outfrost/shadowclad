#ifndef ENGINE_H_
#define ENGINE_H_

typedef struct GLFWwindow GLFWwindow;

void init();
void run(void (*updateFn) (float));
void terminate();
void setKeyboardEventCallback(void (*) (GLFWwindow*, int, int, int, int));

#endif // ENGINE_H_
