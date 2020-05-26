#ifndef PLAYER_H_
#define PLAYER_H_

#include <GL/gl.h>

#include "engine/scene.h"

Scene* playerCharacter;

void initPlayer();
void spawnPlayer(Transform transform);

#endif
