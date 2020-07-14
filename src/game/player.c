#include "player.h"

#include "engine/asset.h"
#include "engine/render.h"

static const float movementSpeed = 2.5f;

Scene* playerCharacter;
static Transform screenToWorldMovementTransform;
static Vector worldMovementUp;
static Vector worldMovementDown;
static Vector worldMovementLeft;
static Vector worldMovementRight;
static Direction movementDirection;

static void movePlayer(Vector direction, float delta);
static Vector worldMovementDirection(float x, float y);



void initPlayer() {
	screenToWorldMovementTransform = identity();
	rotate(&screenToWorldMovementTransform, (Vector) { 0.0f, 1.0f, 0.0f }, - TAU / 8.0f);

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
	Vector direction = { 0.0f, 0.0f, 0.0f };
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

static void movePlayer(Vector direction, float delta) {
	direction = clampMagnitude(direction, 1.0f);
	Vector displacement = scaleVector(direction, delta * movementSpeed);
	translate(&playerCharacter->transform, displacement);
}

static Vector worldMovementDirection(float x, float y) {
	Vector direction = (Vector) { x, 0.0f, -y };
	direction = normalized(
		applyTransform(screenToWorldMovementTransform, direction));
	return direction;
}
