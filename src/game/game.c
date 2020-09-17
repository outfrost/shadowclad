/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "game.h"

#include "engine/input.h"

#include "input.h"
#include "level.h"
#include "player.h"

void initGame() {
	initLevel();
	initPlayer();
	startLevel();

	setKeyboardInputCallback(keyboardInput);
}

void update(float delta) {
	updatePlayer(delta);
}
