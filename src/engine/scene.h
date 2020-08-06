#ifndef ENGINE_SCENE_H_
#define ENGINE_SCENE_H_

#include "asset.h"

typedef struct Scene Scene;

struct Scene {
	Scene* parent;
	size_t numChildren;
	Scene** children;
	Transform transform;
	const Solid* solid;
};

extern Scene* currentScene;

Scene* newScene();
void insertChildScene(Scene* scene, Scene* newChild);
void reparentScene(Scene* scene, Scene* newParent);
Transform worldTransform(const Scene* scene);

#endif // ENGINE_SCENE_H_
