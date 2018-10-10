#ifndef TGA_H_
#define TGA_H_

#include <GL/gl.h>

#pragma pack(push, 1)
typedef struct {
	GLubyte id_length;
	GLbyte color_map_type;
	GLbyte image_type;
	GLushort color_map_start;
	GLushort color_map_length;
	GLubyte color_map_bpp;
	GLushort x_origin;
	GLushort y_origin;
	GLushort image_width;
	GLushort image_height;
	GLubyte image_bpp;
	GLbyte image_descriptor;
} TGAheader;
#pragma pack(pop)

typedef struct {
	TGAheader header;
	GLenum image_format;
	GLint image_components;
	GLbyte* bytes;
} TGAimage;

TGAimage* read_tga(const char* path);

#endif
