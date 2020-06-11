#include "string.h"

#include <stdlib.h>

String stringFromAiString(const struct aiString aistr) {
	char* cstr = memcpy(malloc((aistr.length + 1) * sizeof(char)),
		                aistr.data,
		                aistr.length * sizeof(char));
	cstr[aistr.length] = '\0';
	return (String) { .length = aistr.length, .cstr = cstr };
}

void dropString(String str) {
	free(str.cstr);
	str.length = 0u;
	str.cstr = NULL;
}
