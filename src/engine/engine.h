#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

#include "string.h"

typedef struct EngineConfig EngineConfig;

struct EngineConfig {
	int windowWidth;
	int windowHeight;
	String windowTitle;
	int swapInterval;
};

void init(EngineConfig);
void run(void (*updateFn) (float));
void terminate();

EngineConfig defaultConfig();

#endif // ENGINE_ENGINE_H_
