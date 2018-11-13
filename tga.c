#include <GL/gl.h>

#include <stdlib.h>
#include <stdio.h>

#include "tga.h"

TGAimage* read_tga(const char* path) {
	FILE* tga_file = fopen(path, "rb");
	if (tga_file == NULL) {
		return NULL;
	}
	
	TGAheader header;
	
	if (fread(&header, sizeof(TGAheader), 1, tga_file) != 1) {
		fclose(tga_file);
		return NULL;
	}
	
	GLenum image_format;
	GLint image_components;
	
	if (header.image_bpp == 32) {
		image_format = GL_BGRA_EXT;
		image_components = GL_RGBA8;
	}
	else if (header.image_bpp == 24) {
		image_format = GL_BGR_EXT;
		image_components = GL_RGB8;
	}
	else if (header.image_bpp == 8) {
		image_format = GL_LUMINANCE;
		image_components = GL_LUMINANCE8;
	}
	else {
		fclose(tga_file);
		return NULL;
	}
	
	unsigned long image_size = header.image_width * header.image_height * (header.image_bpp >> 3);
	
	GLbyte* bytes = malloc(image_size * sizeof(GLbyte));
	if (bytes == NULL) {
		fclose(tga_file);
		return NULL;
	}
	
	if (fread(bytes, image_size, 1, tga_file) != 1) {
		free(bytes);
		fclose(tga_file);
		return NULL;
	}
	
	fclose(tga_file);
	
	TGAimage* image = malloc(sizeof(TGAimage));
	if (image == NULL) {
		return NULL;
	}
	
	(*image).header = header;
	(*image).image_format = image_format;
	(*image).image_components = image_components;
	(*image).bytes = bytes;
	
	return image;
}
