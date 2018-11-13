#include <GL/gl.h>
#include <assimp/scene.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* getGlInfoString() {
	const char* glVersion = (const char*) glGetString(GL_VERSION);
	const char* glslVersion = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
	const char* glRenderer = (const char*) glGetString(GL_RENDERER);
	
	size_t glInfoLength = strlen("OpenGL  - GLSL  - ")
	                      + strlen(glVersion)
	                      + strlen(glslVersion)
	                      + strlen(glRenderer);
	
	char* glInfoString = malloc(glInfoLength + sizeof(char));
	sprintf(glInfoString,
		    "OpenGL %s - GLSL %s - %s",
			glVersion,
			glslVersion,
			glRenderer);
	
	return glInfoString;
}

void dumpScene(FILE* stream, const struct aiScene* scene) {
	if (scene == NULL) {
		fprintf(stream, "NULL");
		return;
	}
	fprintf(stream, "{ mFlags = %u, mRootNode = %p, mNumMeshes = %u, mMeshes = %p, mNumMaterials = %u, mMaterials = %p, mNumAnimations = %u, mAnimations = %p, mNumTextures = %u, mTextures = %p, mNumLights = %u, mLights = %p }",
			(*scene).mFlags,
			(void*) (*scene).mRootNode,
			(*scene).mNumMeshes,
			(void*) (*scene).mMeshes,
			(*scene).mNumMaterials,
			(void*) (*scene).mMaterials,
			(*scene).mNumAnimations,
			(void*) (*scene).mAnimations,
			(*scene).mNumTextures,
			(void*) (*scene).mTextures,
			(*scene).mNumLights,
			(void*) (*scene).mLights);
}

void dumpNode(FILE* stream, const struct aiNode* node) {
	if (node == NULL) {
		fprintf(stream, "NULL");
		return;
	}
	fprintf(stream, "{ mName = %s, mNumMeshes = %u, mMeshes = %p }",
			(*node).mName.data,
			(*node).mNumMeshes,
			(void*) (*node).mMeshes);
}
