#include <GL/gl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assimp/scene.h>

char * get_gl_info() {
	const char * gl_version = (const char *) glGetString(GL_VERSION);
	const char * gl_sl_version = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
	const char * gl_renderer = (const char *) glGetString(GL_RENDERER);
	
	size_t gl_info_length = strlen("OpenGL  - GLSL  - ")
				+ strlen(gl_version)
				+ strlen(gl_sl_version)
				+ strlen(gl_renderer);
	
	char * gl_info = malloc(gl_info_length + sizeof(char));
	sprintf(gl_info,
		    "OpenGL %s - GLSL %s - %s",
			gl_version,
			gl_sl_version,
			gl_renderer);
	
	return gl_info;
}

void print_struct_aiScene(FILE* stream, const struct aiScene* scene) {
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

void print_struct_aiNode(FILE* stream, const struct aiNode* node) {
	if (node == NULL) {
		fprintf(stream, "NULL");
		return;
	}
	fprintf(stream, "{ mName = %s, mNumMeshes = %u, mMeshes = %p }",
			(*node).mName.data,
			(*node).mNumMeshes,
			(void*) (*node).mMeshes);
}
