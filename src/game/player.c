#include "player.h"

#include "engine/asset.h"
#include "engine/render.h"

#include "level.h"

static const float movementSpeed = 0.5f;
static const float collisionRadius = 0.5f;

Scene* playerCharacter;
Scene* playerProjectedMovement;
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

	playerProjectedMovement = newScene();
}

void spawnPlayer(Transform transform) {
	playerCharacter->transform = transform;
	reparentScene(playerCharacter, currentScene);
	reparentScene(playerProjectedMovement, currentScene);
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

{
Vector displacement = scaleVector(direction, 0.006944f * movementSpeed * 1000.0f);

	playerProjectedMovement->transform = playerCharacter->transform;

	Vector initialPosition = translationOf(playerCharacter->transform);
	Vector position = initialPosition;

	GridLocation location = gridLocationFromPosition(position);
	Obstacle obstacle = getObstacles(location);

	// Eliminate redundant corner checks
	if (obstacle & OBSTACLE_XP) {
		obstacle &= ~(OBSTACLE_XP_ZP | OBSTACLE_XP_ZN);
	}
	if (obstacle & OBSTACLE_XN) {
		obstacle &= ~(OBSTACLE_XN_ZP | OBSTACLE_XN_ZN);
	}
	if (obstacle & OBSTACLE_ZP) {
		obstacle &= ~(OBSTACLE_XP_ZP | OBSTACLE_XN_ZP);
	}
	if (obstacle & OBSTACLE_ZN) {
		obstacle &= ~(OBSTACLE_XP_ZN | OBSTACLE_XN_ZN);
	}

	float edgeXp = cellBoundaryCoord(location.x + 1);
	float edgeXn = cellBoundaryCoord(location.x);
	float edgeZp = cellBoundaryCoord(location.z + 1);
	float edgeZn = cellBoundaryCoord(location.z);
	float distanceXp = edgeXp - position.x;
	if (obstacle & OBSTACLE_XP) distanceXp -= collisionRadius;
	float distanceXn = edgeXn - position.x;
	if (obstacle & OBSTACLE_XN) distanceXn += collisionRadius;
	float distanceZp = edgeZp - position.z;
	if (obstacle & OBSTACLE_ZP) distanceZp -= collisionRadius;
	float distanceZn = edgeZn - position.z;
	if (obstacle & OBSTACLE_ZN) distanceZn += collisionRadius;

	// Check all edges for intersecting already
	if (distanceXp < 0.0f) {
		position.x += distanceXp;
		displacement = growVectorNoFlip(displacement, distanceXp);
		distanceXp = 0.0f;
	}
	if (distanceXn > 0.0f) {
		position.x += distanceXn;
		displacement = growVectorNoFlip(displacement, - distanceXn);
		distanceXn = 0.0f;
	}
	if (distanceZp < 0.0f) {
		position.z += distanceZp;
		displacement = growVectorNoFlip(displacement, distanceZp);
		distanceZp = 0.0f;
	}
	if (distanceZn > 0.0f) {
		position.z += distanceZn;
		displacement = growVectorNoFlip(displacement, - distanceZn);
		distanceZn = 0.0f;
	}

	// Calculate direct movement limits
	float dx = displacement.x;
	float dz = displacement.z;
	if (dx > distanceXp) {
		dz *= distanceXp / dx;
		dx = distanceXp;
		// Might need a flag that we've reached a limit in X+?
	}
	if (dx < distanceXn) {
		dz *= distanceXn / dx;
		dx = distanceXn;
		// ?
	}
	if (dz > distanceZp) {
		dx *= distanceZp / dz;
		dz = distanceZp;
		// ?
	}
	if (dz < distanceZn) {
		dx *= distanceZn / dz;
		dz = distanceZn;
		// ?
	}
	// This is how far we can move until we collide or leave the grid cell
	position = addVectors(position, (Vector) { dx, 0.0f, dz });
	translate(&playerProjectedMovement->transform, subtractVectors(position, initialPosition));
}


	translate(&playerCharacter->transform, displacement);
}

static Vector worldMovementDirection(float x, float y) {
	Vector direction = (Vector) { x, 0.0f, -y };
	direction = normalized(
		applyTransform(screenToWorldMovementTransform, direction));
	return direction;
}
