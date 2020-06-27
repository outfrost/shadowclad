#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

void init();
void run(void (*updateFn) (float));
void terminate();

#endif // ENGINE_ENGINE_H_
