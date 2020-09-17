/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
