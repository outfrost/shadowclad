/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "string.h"

#include <stdlib.h>

String newString(const char* s) {
	size_t len = 0u;
	char* cstr;
	if (s) {
		len = strlen(s);
		cstr = memcpy(malloc((len + 1) * sizeof(char)),
		              s,
		              len * sizeof(char));
	}
	else {
		cstr = malloc(1 * sizeof(char));
	}
	cstr[len] = '\0';
	return (String) { .length = len, .cstr = cstr };
}

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
