/**
 * Copyright 2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_STRING_H_
#define ENGINE_STRING_H_

#include <assimp/types.h>

typedef struct String String;

struct String {
	size_t length;
	char* cstr;
};

String newString(const char* s);
String stringFromAiString(const struct aiString aistr);
void dropString(String str);

#endif // ENGINE_STRING_H_
