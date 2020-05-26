#ifndef SCENE_H_
#define SCENE_H_

#include "asset.h"

typedef struct Scene Scene;

struct Scene {
	Scene* parent;
	size_t numChildren;
	Scene** children;
	Transform transform;
	const Solid* solid;
};

Scene* currentScene;

Scene* newScene();
void insertChildScene(Scene* scene, Scene* newChild);
void reparentScene(Scene* scene, Scene* newParent);

#endif // SCENE_H_
