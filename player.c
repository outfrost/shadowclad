#include "level.h"
#include "logger.h"
#include "player.h"

PlayerCharacter playerCharacter = { .asset3D = NULL };

static Vector3D playerPos;



void initPlayer() {
	playerCharacter.asset3D = importAsset("assets/playercharacter.3ds");
	// TODO import textures
}

void spawnPlayer() {
	playerPos = playerSpawnPos;
}
