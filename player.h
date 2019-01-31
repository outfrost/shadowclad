#ifndef PLAYER_H_
#define PLAYER_H_

#include <GL/gl.h>

#include "assimp_types.h"

typedef struct {
	const AiScene* sceneData;
	GLuint* textureIds;
} PlayerCharacter;

void spawnPlayer();

#endif
