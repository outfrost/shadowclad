/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "scene.h"

#include <stdlib.h>

#include "engine/logger.h"

Scene* currentScene = NULL;



Scene* newScene() {
	Scene* scene = malloc(sizeof(Scene));
	*scene = (Scene) { .parent = NULL,
	                   .numChildren = 0u,
	                   .children = NULL,
	                   .transform = identity(),
	                   .solid = NULL };
	return scene;
}

void insertChildScene(Scene* scene, Scene* newChild) {
	if (!scene || !newChild) {
		return;
	}
	if (newChild->parent) {
		logError(
			"Cannot insert Scene 0x%p as child of 0x%p, because it is already child of 0x%p",
			newChild,
			scene,
			newChild->parent);
		return;
	}

	scene->children = realloc(scene->children, ++(scene->numChildren) * sizeof(Scene*));
	scene->children[scene->numChildren - 1] = newChild;

	newChild->parent = scene;
}

void reparentScene(Scene* scene, Scene* newParent) {
	if (!scene) {
		return;
	}

	if (scene->parent) {
		Scene* parent = scene->parent;
		size_t indexInParent = parent->numChildren;
		for (size_t i = 0; i < parent->numChildren; ++i) {
			if (parent->children[i] == scene) {
				indexInParent = i;
				break;
			}
		}
		// This includes the hopefully impossible case where parent->numChildren == 0
		if (indexInParent == parent->numChildren) {
			logError("Scene %p not found in children of parent %p", scene, parent);
		}
		else {
			for (size_t i = indexInParent; i < parent->numChildren - 1; ++i) {
				parent->children[i] = parent->children[i + 1];
			}
			parent->children = realloc(parent->children, --(parent->numChildren) * sizeof(Scene*));
		}
	}

	scene->parent = NULL;

	insertChildScene(newParent, scene);
}

Transform worldTransform(const Scene* scene) {
	if (!scene) {
		logError("Cannot compute world space Transform of null Scene");
		return identity();
	}

	if (scene->parent) {
		return multiply(scene->transform, worldTransform(scene->parent));
	}
	else {
		return scene->transform;
	}
}

