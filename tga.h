#ifndef TGA_H_
#define TGA_H_

#include <GL/gl.h>

#pragma pack(push, 1)
typedef struct {
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
} TgaHeader;
#pragma pack(pop)

typedef struct {
	TgaHeader header;
	GLenum imageFormat;
	GLint imageComponents;
	GLbyte* bytes;
} TgaImage;

TgaImage* readTga(const char* path);

#endif
