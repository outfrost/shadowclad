/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <GL/gl.h>

#include "engine/scene.h"

enum Direction {
	DIRECTION_UP = 1 << 0,
	DIRECTION_DOWN = 1 << 1,
	DIRECTION_LEFT = 1 << 2,
	DIRECTION_RIGHT = 1 << 3,
};

typedef enum Direction Direction;

extern Scene* playerCharacter;

void initPlayer();
void spawnPlayer(Transform transform);
void updatePlayer(float delta);
void startMovement(Direction direction);
void stopMovement(Direction direction);

#endif // PLAYER_H_
