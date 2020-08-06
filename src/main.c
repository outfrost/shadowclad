#include "engine/engine.h"

#include "game/game.h"

int main(/*int argc, char** argv*/) {
	EngineConfig cfg = { .windowWidth = 1280,
	                     .windowHeight = 720,
	                     .windowTitle = newString("shadowclad"),
	                     .swapInterval = 1 };

	// Engine startup
	init(cfg);

	initGame();

	// Main update loop
	run(update);

	// Shutdown
	terminate();
	return 0;
}
