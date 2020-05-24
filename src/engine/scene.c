#include <stdlib.h>

#include "engine/logger.h"

#include "scene.h"

Scene* currentScene = NULL;

static Transform identity = { .a1 = 1.0f, .a2 = 0.0f, .a3 = 0.0f, .a4 = 0.0f,
                              .b1 = 0.0f, .b2 = 1.0f, .b3 = 0.0f, .b4 = 0.0f,
                              .c1 = 0.0f, .c2 = 0.0f, .c3 = 1.0f, .c4 = 0.0f,
                              .d1 = 0.0f, .d2 = 0.0f, .d3 = 0.0f, .d4 = 1.0f };

void multiply(Transform* transform, Transform by) {
	GLfloat* a = (GLfloat*) &by;
	GLfloat* b = (GLfloat*) transform;

	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			b[(row * 4) + col] =
				a[(row * 4) + 0] * b[(0 * 4) + col]
				+ a[(row * 4) + 1] * b[(1 * 4) + col]
				+ a[(row * 4) + 2] * b[(2 * 4) + col]
				+ a[(row * 4) + 3] * b[(3 * 4) + col];
		}
	}
}

void translate(Transform* transform, Vector3D vec) {
	multiply(transform, (Transform) { .a1 = 1.0f, .a2 = 0.0f, .a3 = 0.0f, .a4 = vec.x,
                                      .b1 = 0.0f, .b2 = 1.0f, .b3 = 0.0f, .b4 = vec.y,
                                      .c1 = 0.0f, .c2 = 0.0f, .c3 = 1.0f, .c4 = vec.z,
                                      .d1 = 0.0f, .d2 = 0.0f, .d3 = 0.0f, .d4 = 1.0f });
}

Scene* newScene() {
	Scene* scene = malloc(sizeof(Scene));
	*scene = (Scene) { .parent = NULL,
	                   .numChildren = 0u,
	                   .children = NULL,
	                   .transform = identity,
	                   .solid = NULL };
	return scene;
}

void insertChildScene(Scene* scene, Scene* newChild) {
	if (!scene || !newChild) {
		return;
	}
	if (newChild->parent) {
		logError(
			"Cannot insert scene 0x%p as child of 0x%p, because it is already child of 0x%p",
			newChild,
			scene,
			newChild->parent);
		return;
	}

	scene->children = realloc(scene->children, ++(scene->numChildren) * sizeof(Scene*));
	scene->children[scene->numChildren - 1] = newChild;

	newChild->parent = scene;
}
