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

void playerMovementInput(float x, float y) {
	Transform rotation = identity();
	rotate(&rotation, (Vector3D) { 0.0f, 1.0f, 0.0f }, TAU / 8.0f);
	Vector3D movementDirection = (Vector3D) { x, 0.0f, -y };
	movementDirection = normalized(applyTransform(&rotation, movementDirection));
}
