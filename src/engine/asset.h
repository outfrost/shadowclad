/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
