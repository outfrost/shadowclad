#include "asset.h"

#include <stdlib.h>
#include <assimp/cimport.h>
//#include <assimp/metadata.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "logger.h"
#include "tga.h"

#define IMPORT_DEBUG_ 1

static const struct aiScene* importScene(const char* path);
static Vector3D convertAiVector3D(struct aiVector3D vect);
static const char* replaceFileExtension(const struct aiString path, const char* ext);



#if IMPORT_DEBUG_
static void printMetadata(const struct aiMetadata* meta) {
	if (meta) {
		for (size_t i = 0; i < meta->mNumProperties; ++i) {
			char* key = memcpy(malloc((meta->mKeys[i].length + 1) * sizeof(char)),
			                   meta->mKeys[i].data,
			                   meta->mKeys[i].length * sizeof(char));
			key[meta->mKeys[i].length] = '\0';
			const struct aiMetadataEntry value = meta->mValues[i];
			switch (value.mType) {
				case AI_BOOL:
					logDebug("\"%s\": (bool) %d", key, *((int*) value.mData));
					break;
				case AI_INT32:
					logDebug("\"%s\": (int32) %d", key, *((int32_t*) value.mData));
					break;
				case AI_UINT64:
					logDebug("\"%s\": (uint64) %llu", key, *((uint64_t*) value.mData));
					break;
				case AI_FLOAT:
					logDebug("\"%s\": (float) %f", key, *((float*) value.mData));
					break;
				case AI_DOUBLE:
					logDebug("\"%s\": (double) %f", key, *((double*) value.mData));
					break;
				case AI_AISTRING: {
					struct aiString aistr = *((struct aiString*) value.mData);
					char* str = memcpy(malloc((aistr.length + 1) * sizeof(char)),
					                   aistr.data,
					                   aistr.length * sizeof(char));
					str[aistr.length] = '\0';
					logDebug("\"%s\": (string) %s", key, str);
					free(str);
					break; }
				case AI_AIVECTOR3D: {
					struct aiVector3D vec = *((struct aiVector3D*) value.mData);
					logDebug("\"%s\": (vector3d) { %f, %f, %f }", key, vec.x, vec.y, vec.z);
					break; }
				case AI_META_MAX:
				default:
					logDebug("\"%s\": (unrecognized type)", key);
					break;
			}
			free(key);
		}
	}
}

void printAiNodeMetadata(const struct aiNode* node) {
	if (!node) {
		return;
	}

	struct aiString aistr = node->mName;
	char* name = memcpy(malloc((aistr.length + 1) * sizeof(char)),
	                    aistr.data,
	                    aistr.length * sizeof(char));
	name[aistr.length] = '\0';
	logDebug("Metadata from node \"%s\": %p", name, node->mMetaData);
	printMetadata(node->mMetaData);

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		printAiNodeMetadata(node->mChildren[i]);
	}
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
		              .vertices = malloc(numVertices * sizeof(Vector3D)),
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
			mesh.normals = malloc(numVertices * sizeof(Vector3D));
			for (unsigned int normIndex = 0; normIndex < numVertices; ++normIndex) {
				mesh.normals[normIndex] = convertAiVector3D(
						aiMesh->mNormals[normIndex]);
			}
		}
		
		mesh.textureCoords = malloc(numVertices * sizeof(Vector3D));
		for (unsigned int texcIndex = 0; texcIndex < numVertices; ++texcIndex) {
			mesh.textureCoords[texcIndex] = convertAiVector3D(
					aiMesh->mTextureCoords[0][texcIndex]);
		}
		
		for (unsigned int faceIndex = 0; faceIndex < numFaces; ++faceIndex) {
			const struct aiFace aiFace = aiMesh->mFaces[faceIndex];
			const unsigned int numIndices = aiFace.mNumIndices;
			
			Face face = { .numIndices = numIndices,
			              .indices = malloc(numIndices * sizeof(size_t)) };
			
			for (unsigned int i = 0; i < numIndices; ++i) {
				face.indices[i] = aiFace.mIndices[i];
			}
			
			mesh.faces[faceIndex] = face;
		}
		
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
		// print mKey
		struct aiString aistr = prop->mKey;
		char* key = memcpy(malloc((aistr.length + 1) * sizeof(char)),
		                   aistr.data,
		                   aistr.length * sizeof(char));
		key[aistr.length] = '\0';

		logDebug("Material property \"%s\": Shading model: (length %u) %d",
		         key,
		         prop->mDataLength,
		         *((int32_t*) prop->mData));
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
	const struct aiScene* scene = aiImportFile(path, aiProcess_PreTransformVertices);
	if (scene != NULL && scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		logError("Incomplete scene imported from %s", path);
		aiReleaseImport(scene);
		scene = NULL;
	}
	return scene;
}

static Vector3D convertAiVector3D(struct aiVector3D vect) {
	return (Vector3D) { .x = vect.x,
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
