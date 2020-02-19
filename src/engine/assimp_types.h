#ifndef ASSIMP_TYPES_H_
#define ASSIMP_TYPES_H_

#include <assimp/scene.h>

#ifdef ASSIMP_DOUBLE_PRECISION
	#error "ASSIMP_DOUBLE_PRECISION is defined"
	#error "shadowclad relies on ai_real defined as a single precision float"
#endif

typedef struct aiScene AiScene;
typedef struct aiNode AiNode;
typedef struct aiMesh AiMesh;
typedef struct aiFace AiFace;
typedef struct aiVector3D AiVector3D;
typedef struct aiString AiString;

#endif
