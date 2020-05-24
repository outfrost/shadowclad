#ifndef SCENE_H_
#define SCENE_H_

#include "asset.h"

typedef struct Scene Scene;
typedef struct Transform Transform;

struct Transform {
	GLfloat a1, a2, a3, a4;
	GLfloat b1, b2, b3, b4;
	GLfloat c1, c2, c3, c4;
	GLfloat d1, d2, d3, d4;
};

struct Scene {
	Scene* parent;
	size_t numChildren;
	Scene** children;
	Transform transform;
	const Solid* solid;
};

Scene* currentScene;

void translate(Transform* transform, Vector3D vec);
Scene* newScene();
void insertChildScene(Scene* scene, Scene* newChild);

#endif // SCENE_H_
