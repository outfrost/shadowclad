#ifndef RENDER_H_
#define RENDER_H_

const struct aiScene* model;

void render_scene();
void draw_axes();
void draw_model_recursive(const struct aiScene* model, const struct aiNode* node);

#endif
