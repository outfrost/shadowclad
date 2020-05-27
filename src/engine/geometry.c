#include "geometry.h"

#include <stddef.h>

Transform identity() {
	return (Transform) { .a1 = 1.0f, .a2 = 0.0f, .a3 = 0.0f, .a4 = 0.0f,
	                     .b1 = 0.0f, .b2 = 1.0f, .b3 = 0.0f, .b4 = 0.0f,
	                     .c1 = 0.0f, .c2 = 0.0f, .c3 = 1.0f, .c4 = 0.0f,
	                     .d1 = 0.0f, .d2 = 0.0f, .d3 = 0.0f, .d4 = 1.0f };
}

Transform multiply(Transform t1, Transform t2) {
	GLfloat* a = (GLfloat*) &t1;
	GLfloat* b = (GLfloat*) &t2;

	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			b[(row * 4) + col] =
				a[(row * 4) + 0] * b[(0 * 4) + col]
				+ a[(row * 4) + 1] * b[(1 * 4) + col]
				+ a[(row * 4) + 2] * b[(2 * 4) + col]
				+ a[(row * 4) + 3] * b[(3 * 4) + col];
		}
	}
	return t2;
}

void translate(Transform* transform, Vector3D vec) {
	*transform = multiply(
		(Transform) { .a1 = 1.0f, .a2 = 0.0f, .a3 = 0.0f, .a4 = vec.x,
		              .b1 = 0.0f, .b2 = 1.0f, .b3 = 0.0f, .b4 = vec.y,
		              .c1 = 0.0f, .c2 = 0.0f, .c3 = 1.0f, .c4 = vec.z,
		              .d1 = 0.0f, .d2 = 0.0f, .d3 = 0.0f, .d4 = 1.0f },
		*transform);
}

Vector3D translationOf(Transform transform) {
	return (Vector3D) { transform.a4, transform.b4, transform.c4 };
}
