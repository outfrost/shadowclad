#ifndef ENGINE_ASSET_H_
#define ENGINE_ASSET_H_

#include <stddef.h>
#include <GL/gl.h>

#include "geometry.h"

typedef struct Solid Solid;
typedef struct Mesh Mesh;
typedef struct Face Face;
typedef struct Material Material;

struct Solid {
	size_t numMeshes;
	Mesh* meshes;
	size_t numMaterials;
	Material* materials;
};

struct Mesh {
	size_t numVertices;
	Vector* vertices;
	Vector* normals;
	Vector* textureCoords;
	size_t numFaces;
	Face* faces;
	size_t materialIndex;
};

struct Face {
	size_t numIndices;
	size_t* indices;
	Vector* normals;
};

struct Material {
	GLuint textureId;
};

const Solid* importSolid(const char* path);

#endif // ENGINE_ASSET_H_
