#ifndef PLAYER_H_
#define PLAYER_H_

#include <GL/gl.h>

#include "engine/scene.h"

extern Scene* playerCharacter;

void initPlayer();
void spawnPlayer(Transform transform);

#endif // PLAYER_H_
