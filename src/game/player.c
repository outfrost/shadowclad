#include "engine/logger.h"

#include "level.h"
#include "player.h"

Character playerCharacter = { .asset3D = NULL };



void initPlayer() {
	playerCharacter.asset3D = importAsset("assets/playercharacter.3ds");
}

void spawnPlayer() {
	playerPos = playerSpawnPos;
}
