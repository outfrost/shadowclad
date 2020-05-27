#ifndef GEOMETRY_H_
#define GEOMETRY_H_

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

Transform identity();
void translate(Transform* transform, Vector3D vec);
Vector3D translationOf(Transform transform);

#endif // GEOMETRY_H_
