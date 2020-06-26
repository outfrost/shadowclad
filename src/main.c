#include "engine/engine.h"

#include "game/game.h"

int main(/*int argc, char** argv*/) {
	// Engine startup
	init();

	initGame();

	// Main update loop
	run(update);

	// Shutdown
	terminate();
	return 0;
}
