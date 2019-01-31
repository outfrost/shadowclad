#include "asset.h"
#include "level.h"
#include "logger.h"
#include "player.h"

PlayerCharacter playerCharacter = { .sceneData = NULL,
                                    .textureIds = NULL };

static AiVector3D playerPos;



void initPlayer() {
	playerCharacter.sceneData = importScene("out/assets/playercharacter.3ds");
	// TODO import textures
}

void spawnPlayer() {
	playerPos = playerSpawnPos;
}
