#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "asset.h"
#include "logger.h"
#include "tga.h"

static const AiScene* importScene(const char* path);
static Vector3D convertAiVector3D(AiVector3D vect);
static const char* replaceFileExtension(const AiString path, const char* ext);



const Asset3D* importAsset(const char* path) {
	const AiScene* scene = importScene(path);
	if (scene == NULL) {
		return NULL;
	}
	
	const unsigned int numMeshes = scene->mNumMeshes;
	const unsigned int numMaterials = scene->mNumMaterials;
	
	Asset3D* asset = malloc(sizeof(Asset3D));
	asset->numMeshes = numMeshes;
	asset->meshes = malloc(numMeshes * sizeof(Mesh));
	asset->numMaterials = numMaterials;
	asset->materials = malloc(numMaterials * sizeof(Material));
	
	for (unsigned int meshIndex = 0; meshIndex < numMeshes; ++meshIndex) {
		const AiMesh* aiMesh = scene->mMeshes[meshIndex];
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
			const AiFace aiFace = aiMesh->mFaces[faceIndex];
			const unsigned int numIndices = aiFace.mNumIndices;
			
			Face face = { .numIndices = numIndices,
			              .indices = malloc(numIndices
			                                * sizeof(unsigned int)) };
			
			for (unsigned int i = 0; i < numIndices; ++i) {
				face.indices[i] = aiFace.mIndices[i];
			}
			
			mesh.faces[faceIndex] = face;
		}
		
		asset->meshes[meshIndex] = mesh;
	}
	
	GLuint* textureIds = malloc(numMaterials * sizeof(GLuint));
	glGenTextures(numMaterials, textureIds);
	
	for (unsigned int matIndex = 0; matIndex < numMaterials; ++matIndex) {
		Material material = { .textureId = textureIds[matIndex] };
		
		glBindTexture(GL_TEXTURE_2D, material.textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		AiString originalTexturePath;
		if (aiGetMaterialTexture(scene->mMaterials[matIndex],
		                         aiTextureType_DIFFUSE,
		                         0,
		                         &originalTexturePath,
		                         NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			const char* textureFile = replaceFileExtension(originalTexturePath, ".tga");
			const size_t textureFileLength = strlen(textureFile);
			char* texturePath = malloc(strlen("assets/") + textureFileLength + 1);
			strcpy(texturePath, "assets/");
			strncat(texturePath, textureFile, textureFileLength);
			
			TgaImage* textureImage = readTga(texturePath);
			if (textureImage == NULL) {
				logError("Asset texture file not found: %s", texturePath);
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
		
		asset->materials[matIndex] = material;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	aiReleaseImport(scene);
	return asset;
}

static const AiScene* importScene(const char* path) {
	const AiScene* scene = aiImportFile(path, aiProcess_PreTransformVertices);
	if (scene == NULL) {
		logError("Failed to import asset from %s", path);
	}
	else if ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE) {
		logError("Incomplete scene imported from %s", path);
		aiReleaseImport(scene);
		scene = NULL;
	}
	return scene;
}

static Vector3D convertAiVector3D(AiVector3D vect) {
	return (Vector3D) { .x = vect.x,
	                    .y = vect.y,
	                    .z = vect.z };
}

/** BUGS
 * The following function will not work properly with texture
 * file names (excluding directory part) beginning with '.'
 */
static const char* replaceFileExtension(const AiString path, const char* ext) {
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
