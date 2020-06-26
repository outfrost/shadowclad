#include "game.h"

#include "engine/engine.h"

#include "input.h"
#include "level.h"
#include "player.h"

void initGame() {
	initLevel();
	initPlayer();
	startLevel();

	setKeyboardEventCallback(onKeyboardEvent);
}

void update(float delta) {
	updatePlayer(delta);
}
