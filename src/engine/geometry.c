#include "geometry.h"

#include <math.h>
#include <stddef.h>

const float TAU = 6.28318530718f;



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

void rotate(Transform* transform, Vector3D axis, float angle) {
	axis = normalized(axis);
	float l = axis.x;
	float m = axis.y;
	float n = axis.z;
	float sinA = sinf(angle);
	float cosA = cosf(angle);
	float omcA = 1 - cosA;
	*transform = multiply(
		(Transform) { l*l*omcA + cosA, m*l*omcA - n*sinA, n*l*omcA + m*sinA, 0.0f,
		              l*m*omcA + n*sinA, m*m*omcA + cosA, n*m*omcA - l*sinA, 0.0f,
		              l*n*omcA - m*sinA, m*n*omcA + l*sinA, n*n*omcA + cosA, 0.0f,
		              0.0f, 0.0f, 0.0f, 1.0f },
		*transform);
}

Vector3D applyTransform(Transform* transform, Vector3D vec) {
	GLfloat* a = (GLfloat*) &transform;
	GLfloat b[4] = { vec.x, vec.y, vec.z, 1.0f };
	GLfloat c[4];

	for (size_t row = 0; row < 4; ++row) {
		c[row] =
			a[(row * 4) + 0] * b[0]
			+ a[(row * 4) + 1] * b[1]
			+ a[(row * 4) + 2] * b[2]
			+ a[(row * 4) + 3] * b[3];
	}
	return (Vector3D) { c[0], c[1], c[2] };
}

Vector3D translationOf(Transform transform) {
	return (Vector3D) { transform.a4, transform.b4, transform.c4 };
}

Vector3D normalized(Vector3D vec) {
	float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return (Vector3D) {vec.x / magnitude, vec.y / magnitude, vec.z / magnitude};
}
