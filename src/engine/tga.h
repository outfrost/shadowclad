/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_TGA_H_
#define ENGINE_TGA_H_

#include <GL/gl.h>

typedef struct TgaHeader TgaHeader;
typedef struct TgaImage TgaImage;

#pragma pack(push, 1)
struct TgaHeader {
	GLubyte idLength;
	GLbyte colorMapType;
	GLbyte imageType;
	GLushort colorMapStart;
	GLushort colorMapLength;
	GLubyte colorMapBpp;
	GLushort originX;
	GLushort originY;
	GLushort imageWidth;
	GLushort imageHeight;
	GLubyte imageBpp;
	GLbyte imageDescriptor;
};
#pragma pack(pop)

struct TgaImage {
	TgaHeader header;
	GLenum imageFormat;
	GLint imageComponents;
	GLbyte* bytes;
};

TgaImage* readTga(const char* path);

#endif // ENGINE_TGA_H_
