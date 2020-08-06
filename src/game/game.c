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
