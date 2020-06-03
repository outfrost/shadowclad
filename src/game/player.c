#include "player.h"

#include "engine/asset.h"
#include "engine/render.h"

Scene* playerCharacter;
static Transform movementDirectionTransform;



void initPlayer() {
	movementDirectionTransform = identity();
	rotate(&movementDirectionTransform, (Vector3D) { 0.0f, 1.0f, 0.0f }, - TAU / 8.0f);

	playerCharacter = newScene();
	cameraAnchor = playerCharacter;
	playerCharacter->solid = importSolid("assets/playercharacter.3ds");
}

void spawnPlayer(Transform transform) {
	playerCharacter->transform = transform;
	reparentScene(playerCharacter, currentScene);
}

void playerMovementInput(float x, float y) {
	if (!playerCharacter) {
		return;
	}

	Vector3D direction = (Vector3D) { x, 0.0f, -y };
	direction = normalized(
		applyTransform(movementDirectionTransform, direction));
	float velocity = 1.0f;
	Vector3D movement = { direction.x * velocity,
	                      direction.y * velocity,
	                      direction.z * velocity };

	translate(&(playerCharacter->transform), movement);
}
