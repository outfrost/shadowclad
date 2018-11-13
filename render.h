#ifndef RENDER_H_
#define RENDER_H_

const struct aiScene* model; // TODO remove

void renderScene();
void drawAxes();
void drawModelRecursive(const struct aiScene* model, const struct aiNode* node);

#endif
