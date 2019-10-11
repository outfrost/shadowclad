#ifndef PLAYER_H_
#define PLAYER_H_

#include <GL/gl.h>

#include "asset.h"

typedef struct {
	const Asset3D* asset3D;
} Character;

Character playerCharacter;
Vector3D playerPos;

void spawnPlayer();

#endif
