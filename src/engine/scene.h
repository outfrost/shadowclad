/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
