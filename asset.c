#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "assimp_types.h"

#include "logger.h"

const AiScene* importScene(const char* path) {
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
	// TODO aiReleaseImport(scene);
}
