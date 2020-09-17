/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "asset.h"

#include <math.h>
#include <stdlib.h>
#include <assimp/cimport.h>
//#include <assimp/metadata.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "logger.h"
#include "string.h"
#include "tga.h"

#define IMPORT_DEBUG_ 0

static const float smoothingThresholdAngle = TAU / 14.0f;

static const struct aiScene* importScene(const char* path);
static Vector triangleNormal(Vector v1, Vector v2, Vector v3);
static Vector convertAiVector3D(struct aiVector3D vect);
static const char* replaceFileExtension(const struct aiString path, const char* ext);



#if IMPORT_DEBUG_
static void printMetadata(const struct aiMetadata* meta) {
	if (meta) {
		for (size_t i = 0; i < meta->mNumProperties; ++i) {
			String key = stringFromAiString(meta->mKeys[i]);
			const struct aiMetadataEntry value = meta->mValues[i];
			switch (value.mType) {
				case AI_BOOL:
					logDebug("\"%s\": (bool) %d", key.cstr, *((int*) value.mData));
					break;
				case AI_INT32:
					logDebug("\"%s\": (int32) %d", key.cstr, *((int32_t*) value.mData));
					break;
				case AI_UINT64:
					logDebug("\"%s\": (uint64) %llu", key.cstr, *((uint64_t*) value.mData));
					break;
				case AI_FLOAT:
					logDebug("\"%s\": (float) %f", key.cstr, *((float*) value.mData));
					break;
				case AI_DOUBLE:
					logDebug("\"%s\": (double) %f", key.cstr, *((double*) value.mData));
					break;
				case AI_AISTRING: {
					String str = stringFromAiString(*((struct aiString*) value.mData));
					logDebug("\"%s\": (string) %s", key.cstr, str.cstr);
					dropString(str);
					break; }
				case AI_AIVECTOR3D: {
					struct aiVector3D vec = *((struct aiVector3D*) value.mData);
					logDebug("\"%s\": (vector3d) { %f, %f, %f }", key.cstr, vec.x, vec.y, vec.z);
					break; }
				case AI_META_MAX:
				default:
					logDebug("\"%s\": (unrecognized type)", key.cstr);
					break;
			}
			dropString(key);
		}
	}
}

void printAiNodeMetadata(const struct aiNode* node) {
	if (!node) {
		return;
	}

	String name = stringFromAiString(node->mName);
	logDebug("Metadata from node \"%s\": %p", name.cstr, node->mMetaData);
	printMetadata(node->mMetaData);

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		printAiNodeMetadata(node->mChildren[i]);
	}

	dropString(name);
}
#endif // IMPORT_DEBUG_

const Solid* importSolid(const char* path) {
	const struct aiScene* scene = importScene(path);
	if (scene == NULL) {
		logError("Failed to import solid from %s", path);
		return NULL;
	}

#if IMPORT_DEBUG_
	const struct aiMetadata* meta = scene->mMetaData;
	logDebug("Metadata from %s: %p", path, meta);
	printMetadata(meta);
	printAiNodeMetadata(scene->mRootNode);
#endif // IMPORT_DEBUG_

	const unsigned int numMeshes = scene->mNumMeshes;
	const unsigned int numMaterials = scene->mNumMaterials;

	// TODO Consider assets with some arrays empty, and prevent zero mallocs

	Solid* solid = malloc(sizeof(Solid));
	solid->numMeshes = numMeshes;
	solid->meshes = malloc(numMeshes * sizeof(Mesh));
	solid->numMaterials = numMaterials;
	solid->materials = malloc(numMaterials * sizeof(Material));

	for (unsigned int meshIndex = 0; meshIndex < numMeshes; ++meshIndex) {
		const struct aiMesh* aiMesh = scene->mMeshes[meshIndex];
		const unsigned int numVertices = aiMesh->mNumVertices;
		const unsigned int numFaces = aiMesh->mNumFaces;

		Mesh mesh = { .numVertices = numVertices,
		              .vertices = malloc(numVertices * sizeof(Vector)),
		              .normals = NULL,
		              .textureCoords = NULL,
		              .numFaces = numFaces,
		              .faces = malloc(numFaces * sizeof(Face)),
		              .materialIndex = aiMesh->mMaterialIndex };

		for (unsigned int vertIndex = 0; vertIndex < numVertices; ++vertIndex) {
			mesh.vertices[vertIndex] = convertAiVector3D(
					aiMesh->mVertices[vertIndex]);
		}

		if (aiMesh->mNormals != NULL) {
			mesh.normals = malloc(numVertices * sizeof(Vector));
			for (unsigned int normIndex = 0; normIndex < numVertices; ++normIndex) {
				mesh.normals[normIndex] = convertAiVector3D(
						aiMesh->mNormals[normIndex]);
			}
		}

		mesh.textureCoords = malloc(numVertices * sizeof(Vector));
		for (unsigned int texcIndex = 0; texcIndex < numVertices; ++texcIndex) {
			mesh.textureCoords[texcIndex] = convertAiVector3D(
					aiMesh->mTextureCoords[0][texcIndex]);
		}

		for (unsigned int faceIndex = 0; faceIndex < numFaces; ++faceIndex) {
			const struct aiFace aiFace = aiMesh->mFaces[faceIndex];
			const unsigned int numIndices = aiFace.mNumIndices;

			Face face = { .numIndices = numIndices,
			              .indices = malloc(numIndices * sizeof(size_t)),
			              .normals = malloc(numIndices * sizeof(Vector)) };

			for (unsigned int i = 0; i < numIndices; ++i) {
				face.indices[i] = aiFace.mIndices[i];
			}

			if (numIndices == 3) {
				Vector normal = triangleNormal(mesh.vertices[face.indices[0]],
				                               mesh.vertices[face.indices[1]],
				                               mesh.vertices[face.indices[2]]);
				for (size_t i = 0; i < numIndices; ++i) {
					face.normals[i] = normal;
				}
			}
			else {
				if (mesh.normals) {
					for (size_t i = 0; i < numIndices; ++i) {
						face.normals[i] = mesh.normals[face.indices[i]];
					}
				}
				else {
					free(face.normals);
					face.normals = NULL;
				}
			}

			mesh.faces[faceIndex] = face;
		}

		float smoothingThreshold = cosf(smoothingThresholdAngle);
		Face* smoothedFaces = malloc(mesh.numFaces * sizeof(Face));
		for (size_t faceIndex = 0; faceIndex < mesh.numFaces; ++faceIndex) {
			Face face = mesh.faces[faceIndex];

			if (face.normals) {
				face.normals = memcpy(malloc(face.numIndices * sizeof(Vector)),
				                      face.normals,
				                      face.numIndices * sizeof(Vector));

				for (size_t indexIndex = 0; indexIndex < face.numIndices; ++indexIndex) {
					Vector smoothedNormal = face.normals[indexIndex];

					for (size_t i = 0; i < mesh.numFaces; ++i) {
						if (i == faceIndex || !mesh.faces[i].normals) {
							continue;
						}

						for (size_t k = 0; k < mesh.faces[i].numIndices; ++k) {
							if (mesh.faces[i].indices[k] == face.indices[indexIndex]
							    && dotProduct(face.normals[indexIndex],
							                  mesh.faces[i].normals[k]) >= smoothingThreshold) {
								smoothedNormal = addVectors(smoothedNormal, mesh.faces[i].normals[k]);
							}
						}
					}

					face.normals[indexIndex] = normalized(smoothedNormal);
				}
			}
			smoothedFaces[faceIndex] = face;
		}
		// TODO Actually clean up the stuff inside
		free(mesh.faces);
		mesh.faces = smoothedFaces;

		solid->meshes[meshIndex] = mesh;
	}

	GLuint* textureIds = malloc(numMaterials * sizeof(GLuint));
	glGenTextures(numMaterials, textureIds);

	for (unsigned int matIndex = 0; matIndex < numMaterials; ++matIndex) {
		Material material = { .textureId = textureIds[matIndex] };

#if IMPORT_DEBUG_
		const struct aiMaterialProperty* prop;
		aiGetMaterialProperty(scene->mMaterials[matIndex],
		                      AI_MATKEY_SHADING_MODEL,
		                      &prop);

		String key = stringFromAiString(prop->mKey);

		logDebug("Material property \"%s\": Shading model: (length %u) %d",
		         key.cstr,
		         prop->mDataLength,
		         *((int32_t*) prop->mData));

		dropString(key);
#endif // IMPORT_DEBUG_

		glBindTexture(GL_TEXTURE_2D, material.textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		struct aiString originalTexturePath;
		if (aiGetMaterialTexture(scene->mMaterials[matIndex],
		                         aiTextureType_DIFFUSE,
		                         0,
		                         &originalTexturePath,
		                         NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			const char* textureFilename = replaceFileExtension(originalTexturePath, ".tga");
			const size_t textureFilenameLength = strlen(textureFilename);
			char* texturePath = malloc(strlen("assets/") + textureFilenameLength + 1);
			strcpy(texturePath, "assets/");
			strncat(texturePath, textureFilename, textureFilenameLength);

			TgaImage* textureImage = readTga(texturePath);
			if (textureImage == NULL) {
				logError("Importing solid from %s: Cannot read texture file %s", path, texturePath);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D,
				             0,
				             textureImage->imageComponents,
				             textureImage->header.imageWidth,
				             textureImage->header.imageHeight,
				             0,
				             textureImage->imageFormat,
				             GL_UNSIGNED_BYTE,
				             textureImage->bytes);
				free(textureImage->bytes);
				free(textureImage);
			}
		}

		solid->materials[matIndex] = material;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	aiReleaseImport(scene);
	return solid;
}

static const struct aiScene* importScene(const char* path) {
	const struct aiScene* scene = aiImportFile(path, aiProcess_JoinIdenticalVertices
	                                                 | aiProcess_PreTransformVertices
	                                                 | aiProcess_ValidateDataStructure);
	if (scene != NULL && scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		logError("Incomplete scene imported from %s", path);
		aiReleaseImport(scene);
		scene = NULL;
	}
	return scene;
}

static Vector triangleNormal(Vector v1, Vector v2, Vector v3) {
	return normalized(crossProduct(subtractVectors(v2, v1), subtractVectors(v3, v1)));
}

static Vector convertAiVector3D(struct aiVector3D vect) {
	return (Vector) { .x = vect.x,
	                  .y = vect.y,
	                  .z = vect.z };
}

/**
 * BUGS
 * The following function will not work properly with texture
 * file names (excluding directory part) beginning with '.'
 */
static const char* replaceFileExtension(const struct aiString path, const char* ext) {
		size_t lengthToCopy = path.length;

		char* lastDotSubstr = strrchr(path.data, '.');
		if (lastDotSubstr != NULL) {
			if (strpbrk(lastDotSubstr, "\\/") == NULL) {
				lengthToCopy = lastDotSubstr - path.data;
			}
		}

		size_t extLength = strlen(ext) + 1;
		char* newPath = malloc(lengthToCopy + extLength);
		strncpy(newPath, path.data, lengthToCopy);
		strncpy(newPath + lengthToCopy, ext, extLength);

		return newPath;
}
