#include "player.h"

#include "engine/asset.h"
#include "engine/render.h"

static const float movementSpeed = 2.5f;

Scene* playerCharacter;
static Transform screenToWorldMovementTransform;
static Vector3D worldMovementUp;
static Vector3D worldMovementDown;
static Vector3D worldMovementLeft;
static Vector3D worldMovementRight;
static Direction movementDirection;

static void movePlayer(Vector3D direction, float delta);
static Vector3D worldMovementDirection(float x, float y);



void initPlayer() {
	screenToWorldMovementTransform = identity();
	rotate(&screenToWorldMovementTransform, (Vector3D) { 0.0f, 1.0f, 0.0f }, - TAU / 8.0f);

	worldMovementUp = worldMovementDirection(0.0f, 1.0f);
	worldMovementDown = worldMovementDirection(0.0f, -1.0f);
	worldMovementLeft = worldMovementDirection(-1.0f, 0.0f);
	worldMovementRight = worldMovementDirection(1.0f, 0.0f);

	playerCharacter = newScene();
	cameraAnchor = playerCharacter;
	playerCharacter->solid = importSolid("assets/playercharacter.3ds");
}

void spawnPlayer(Transform transform) {
	playerCharacter->transform = transform;
	reparentScene(playerCharacter, currentScene);
}

void updatePlayer(float delta) {
	Vector3D direction = { 0.0f, 0.0f, 0.0f };
	if (movementDirection & DIRECTION_UP) {
		direction = addVectors(direction, worldMovementUp);
	}
	if (movementDirection & DIRECTION_DOWN) {
		direction = addVectors(direction, worldMovementDown);
	}
	if (movementDirection & DIRECTION_LEFT) {
		direction = addVectors(direction, worldMovementLeft);
	}
	if (movementDirection & DIRECTION_RIGHT) {
		direction = addVectors(direction, worldMovementRight);
	}
	movePlayer(direction, delta);
}

void startMovement(Direction direction) {
	movementDirection |= direction;
}

void stopMovement(Direction direction) {
	movementDirection &= ~direction;
}

static void movePlayer(Vector3D direction, float delta) {
	direction = clampMagnitude(direction, 1.0f);
	Vector3D displacement = scaleVector(direction, delta * movementSpeed);
	translate(&playerCharacter->transform, displacement);
}

static Vector3D worldMovementDirection(float x, float y) {
	Vector3D direction = (Vector3D) { x, 0.0f, -y };
	direction = normalized(
		applyTransform(screenToWorldMovementTransform, direction));
	return direction;
}
