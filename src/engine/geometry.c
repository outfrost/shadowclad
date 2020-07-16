#include "geometry.h"

#include <math.h>
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
	Transform result;
	GLfloat* c = (GLfloat*) &result;

	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			c[(row * 4) + col] =
				a[(row * 4) + 0] * b[(0 * 4) + col]
				+ a[(row * 4) + 1] * b[(1 * 4) + col]
				+ a[(row * 4) + 2] * b[(2 * 4) + col]
				+ a[(row * 4) + 3] * b[(3 * 4) + col];
		}
	}
	return result;
}

void translate(Transform* transform, Vector vec) {
	*transform = multiply(
		(Transform) { .a1 = 1.0f, .a2 = 0.0f, .a3 = 0.0f, .a4 = vec.x,
		              .b1 = 0.0f, .b2 = 1.0f, .b3 = 0.0f, .b4 = vec.y,
		              .c1 = 0.0f, .c2 = 0.0f, .c3 = 1.0f, .c4 = vec.z,
		              .d1 = 0.0f, .d2 = 0.0f, .d3 = 0.0f, .d4 = 1.0f },
		*transform);
}

void rotate(Transform* transform, Vector axis, float angle) {
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

Vector addVectors(Vector v1, Vector v2){
	return (Vector) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector subtractVectors(Vector v1, Vector v2) {
	return (Vector) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vector crossProduct(Vector v1, Vector v2) {
	return (Vector) { .x = (v1.y * v2.z) - (v1.z * v2.y),
	                  .y = (v1.z * v2.x) - (v1.x * v2.z),
	                  .z = (v1.x * v2.y) - (v1.y * v2.x) };
}

float dotProduct(Vector v1, Vector v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector scaleVector(Vector vec, float scale) {
	return (Vector) { vec.x * scale,
	                  vec.y * scale,
	                  vec.z * scale };
}

Vector growVectorNoFlip(Vector vec, float amount) {
	float mag = magnitude(vec);
	float factor = (mag + amount) / mag;
	if (factor < 0.0f) {
		factor = 0.0f;
	}
	return scaleVector(vec, factor);
}

Vector clampMagnitude(Vector vec, float maxMagnitude) {
	float m = magnitude(vec);
	if (m > maxMagnitude) {
		vec = scaleVector(vec, maxMagnitude / m);
	}
	return vec;
}

float magnitude(Vector vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vector applyTransform(Transform transform, Vector vec) {
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
	return (Vector) { c[0], c[1], c[2] };
}

Vector translationOf(Transform transform) {
	return (Vector) { transform.a4, transform.b4, transform.c4 };
}

Vector normalized(Vector vec) {
	float m = magnitude(vec);
	return (Vector) { vec.x / m, vec.y / m, vec.z / m };
}
