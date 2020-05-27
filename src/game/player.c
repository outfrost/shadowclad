#include "player.h"

#include "engine/asset.h"
#include "engine/render.h"

Scene* playerCharacter;



void initPlayer() {
	playerCharacter = newScene();
	cameraAnchor = playerCharacter;
	playerCharacter->solid = importSolid("assets/playercharacter.3ds");
}

void spawnPlayer(Transform transform) {
	playerCharacter->transform = transform;
	reparentScene(playerCharacter, currentScene);
}
