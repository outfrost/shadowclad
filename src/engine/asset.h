#ifndef ASSET_H_
#define ASSET_H_

#include <GL/gl.h>

#include "assimp_types.h"

#include "geometry.h"

typedef struct Asset3D Asset3D;
typedef struct Mesh Mesh;
typedef struct Face Face;
typedef struct Material Material;

struct Asset3D {
	unsigned int numMeshes;
	Mesh* meshes;
	unsigned int numMaterials;
	Material* materials;
};

struct Mesh {
	unsigned int numVertices;
	Vector3D* vertices;
	Vector3D* normals;
	Vector3D* textureCoords;
	unsigned int numFaces;
	Face* faces;
	unsigned int materialIndex;
};

struct Face {
	unsigned int numIndices;
	unsigned int* indices;
};

struct Material {
	GLuint textureId;
};

const Asset3D* importAsset(const char* path);

#endif
