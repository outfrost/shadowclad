#ifndef PLAYER_H_
#define PLAYER_H_

#include <GL/gl.h>

#include "engine/asset.h"

typedef struct {
	const Solid* solid;
} Character;

Character playerCharacter;
Vector3D playerPos;

void initPlayer();
void spawnPlayer();

#endif
