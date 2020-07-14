#ifndef ENGINE_GEOMETRY_H_
#define ENGINE_GEOMETRY_H_

#include <GL/gl.h>

typedef struct Vector Vector;
typedef struct Transform Transform;

struct Vector {
	float x;
	float y;
	float z;
};

struct Transform {
	GLfloat a1, a2, a3, a4;
	GLfloat b1, b2, b3, b4;
	GLfloat c1, c2, c3, c4;
	GLfloat d1, d2, d3, d4;
};

static const float TAU = 6.28318530718f;

Transform identity();
Transform multiply(Transform t1, Transform t2);
void translate(Transform* transform, Vector vec);
void rotate(Transform* transform, Vector axis, float angle);
Vector addVectors(Vector v1, Vector v2);
Vector subtractVectors(Vector v1, Vector v2);
Vector crossProduct(Vector v1, Vector v2);
float dotProduct(Vector v1, Vector v2);
Vector scaleVector(Vector vec, float scale);
Vector clampMagnitude(Vector vec, float maxMagnitude);
float magnitude(Vector vec);
Vector applyTransform(Transform transform, Vector vec);
Vector translationOf(Transform transform);
Vector normalized(Vector vec);

#endif // ENGINE_GEOMETRY_H_
