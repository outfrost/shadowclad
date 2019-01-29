#include <stdio.h>
#include <stdlib.h>

#include "tga.h"

TgaImage* readTga(const char* path) {
	FILE* tgaFile = fopen(path, "rb");
	if (tgaFile == NULL) {
		return NULL;
	}
	
	TgaHeader header;
	
	if (fread(&header, sizeof(TgaHeader), 1, tgaFile) != 1) {
		fclose(tgaFile);
		return NULL;
	}
	
	GLenum imageFormat;
	GLint imageComponents;
	
	switch (header.imageBpp) {
		case 32:
			imageFormat = GL_BGRA;
			imageComponents = GL_RGBA8;
			break;
		case 24:
			imageFormat = GL_BGR;
			imageComponents = GL_RGB8;
			break;
		case 8:
			imageFormat = GL_LUMINANCE;
			imageComponents = GL_LUMINANCE8;
			break;
		default:
			fclose(tgaFile);
			return NULL;
	}
	
	unsigned long imageSize = header.imageWidth * header.imageHeight * (header.imageBpp >> 3);
	
	GLbyte* bytes = malloc(imageSize * sizeof(GLbyte));
	if (bytes == NULL) {
		fclose(tgaFile);
		return NULL;
	}
	
	if (fread(bytes, imageSize, 1, tgaFile) != 1) {
		free(bytes);
		fclose(tgaFile);
		return NULL;
	}
	
	fclose(tgaFile);
	
	TgaImage* image = malloc(sizeof(TgaImage));
	if (image == NULL) {
		return NULL;
	}
	
	(*image).header = header;
	(*image).imageFormat = imageFormat;
	(*image).imageComponents = imageComponents;
	(*image).bytes = bytes;
	
	return image;
}
