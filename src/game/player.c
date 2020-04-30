#include "engine/logger.h"

#include "level.h"
#include "player.h"

Character playerCharacter = { .solid = NULL };



void initPlayer() {
	playerCharacter.solid = importSolid("assets/playercharacter.3ds");
}

void spawnPlayer() {
	playerPos = playerSpawnPos;
}
