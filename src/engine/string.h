#ifndef ENGINE_STRING_H_
#define ENGINE_STRING_H_

#include <assimp/types.h>

typedef struct String String;

struct String {
	size_t length;
	char* cstr;
};

String stringFromAiString(const struct aiString aistr);
void dropString(String str);

#endif // ENGINE_STRING_H_
