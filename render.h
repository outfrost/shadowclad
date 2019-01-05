#ifndef RENDER_H_
#define RENDER_H_

#include "tga.h" // TODO remove

const struct aiScene* model; // TODO remove
const TgaImage* texImage; // TODO remove

void renderScene();
void drawAxes();
void drawModelRecursive(const struct aiScene* model, const struct aiNode* node);
void applyMaterial(const struct aiMaterial* mtl);
void color4_to_float4(const struct aiColor4D *c, float f[4]);
void set_float4(float f[4], float a, float b, float c, float d);

#endif
