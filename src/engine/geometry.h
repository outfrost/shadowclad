#ifndef ENGINE_GEOMETRY_H_
#define ENGINE_GEOMETRY_H_

#include <GL/gl.h>

typedef struct Vector3D Vector3D;
typedef struct Transform Transform;

struct Vector3D {
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
void translate(Transform* transform, Vector3D vec);
void rotate(Transform* transform, Vector3D axis, float angle);
Vector3D addVectors(Vector3D v1, Vector3D v2);
Vector3D subtractVectors(Vector3D v1, Vector3D v2);
Vector3D crossProduct(Vector3D v1, Vector3D v2);
float dotProduct(Vector3D v1, Vector3D v2);
Vector3D scaleVector(Vector3D vec, float scale);
Vector3D clampMagnitude(Vector3D vec, float maxMagnitude);
float magnitude(Vector3D vec);
Vector3D applyTransform(Transform transform, Vector3D vec);
Vector3D translationOf(Transform transform);
Vector3D normalized(Vector3D vec);

#endif // ENGINE_GEOMETRY_H_
