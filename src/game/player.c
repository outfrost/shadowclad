/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "player.h"

#include "engine/asset.h"
#include "engine/logger.h"
#include "engine/render.h"

#include "level.h"

static const float movementSpeed = 1.5f;
static const float collisionRadius = 0.5f;

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
	Vector direction = zeroVector();
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
	float distance = clamp(delta * movementSpeed, - BLOCKGRID_CELL_SIZE, BLOCKGRID_CELL_SIZE);
	Vector displacement = scaleVector(direction, distance);

	Vector initialPosition = translationOf(playerCharacter->transform);
	Vector position = initialPosition;



	Obstacle o = getObstacles(gridLocationFromPosition(position));



	GridLocation location = gridLocationFromPosition(position);
	bool enteredNewCell = true;

	while (enteredNewCell) {
		enteredNewCell = false;
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
		Vector displacementToLimit = displacement;
		bool reachedXp = false;
		bool reachedXn = false;
		bool reachedZp = false;
		bool reachedZn = false;
		if (displacementToLimit.x > distanceXp) {
			displacementToLimit = scaleVector(
				displacementToLimit, distanceXp / displacementToLimit.x);
			reachedXp = true;
		}
		if (displacementToLimit.x < distanceXn) {
			displacementToLimit = scaleVector(
				displacementToLimit, distanceXn / displacementToLimit.x);
			reachedXn = true;
		}
		if (displacementToLimit.z > distanceZp) {
			displacementToLimit = scaleVector(
				displacementToLimit, distanceZp / displacementToLimit.z);
			reachedZp = true;
		}
		if (displacementToLimit.z < distanceZn) {
			displacementToLimit = scaleVector(
				displacementToLimit, distanceZn / displacementToLimit.z);
			reachedZn = true;
		}
		// This is how far we can move until we collide or leave the grid cell
		position = addVectors(position, displacementToLimit);
		displacement = subtractVectors(displacement, displacementToLimit);
		// Update distances
		distanceXp -= displacementToLimit.x;
		distanceXn -= displacementToLimit.x;
		distanceZp -= displacementToLimit.z;
		distanceZn -= displacementToLimit.z;

		// Slide along obstacles
		if ((reachedXp && obstacle & OBSTACLE_XP)
			|| (reachedXn && obstacle & OBSTACLE_XN))
		{
			float dz = displacement.z;
			if (dz > distanceZp) {
				dz = distanceZp;
				reachedZp = true;
			}
			if (dz < distanceZn) {
				dz = distanceZn;
				reachedZn = true;
			}
			position.z += dz;
			displacement = scaleVector(displacement, 1.0f - (dz / displacement.z));
		}

		if ((reachedZp && obstacle & OBSTACLE_ZP)
			|| (reachedZn && obstacle & OBSTACLE_ZN))
		{
			float dx = displacement.x;
			if (dx > distanceXp) {
				dx = distanceXp;
				reachedXp = true;
			}
			if (dx < distanceXn) {
				dx = distanceXn;
				reachedXn = true;
			}
			position.x += dx;
			displacement = scaleVector(displacement, 1.0f - (dx / displacement.x));
		}

		// Resolve crossing cell boundaries
		// in reverse order to direct movement limits, because
		// we only want to cross the closest cell boundary.
		if (reachedZn && !(obstacle & OBSTACLE_ZN)) {
			// Enter new grid cell
			location.z -= 1;
			enteredNewCell = true;
		}

		if (!enteredNewCell && reachedZp && !(obstacle & OBSTACLE_ZP)) {
			location.z += 1;
			enteredNewCell = true;
		}

		if (!enteredNewCell && reachedXn && !(obstacle & OBSTACLE_XN)) {
			location.x -= 1;
			enteredNewCell = true;
		}

		if (!enteredNewCell && reachedXp && !(obstacle & OBSTACLE_XP)) {
			location.x += 1;
			enteredNewCell = true;
		}
	}

	translate(&playerCharacter->transform, subtractVectors(position, initialPosition));
}

static Vector worldMovementDirection(float x, float y) {
	Vector direction = (Vector) { x, 0.0f, -y };
	direction = normalized(
		applyTransform(screenToWorldMovementTransform, direction));
	return direction;
}
