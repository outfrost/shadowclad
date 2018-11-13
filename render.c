#include <GL/glut.h>
#include <assimp/scene.h>

#include "render.h"
#include "typedefs.h"

const float AXIS_RADIUS = 5.0f;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	drawAxes();
	drawModelRecursive(model, (*model).mRootNode);
	
	glFlush();
	glutSwapBuffers();
}

void drawAxes() {
	point3f xAxisStart = { 0.0f, 0.0f, 0.0f };
	point3f xAxisEnd = { AXIS_RADIUS, 0.0f, 0.0f };
	point3f yAxisStart = { 0.0f, 0.0f, 0.0f };
	point3f yAxisEnd = { 0.0f, AXIS_RADIUS, 0.0f };
	point3f zAxisStart = { 0.0f, 0.0f, 0.0f };
	point3f zAxisEnd = { 0.0f, 0.0f, AXIS_RADIUS };
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(xAxisStart);
	glVertex3fv(xAxisEnd);
	glEnd();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(yAxisStart);
	glVertex3fv(yAxisEnd);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3fv(zAxisStart);
	glVertex3fv(zAxisEnd);
	glEnd();
}

void drawModelRecursive(const struct aiScene* model, const struct aiNode* node) {
	if (((*model).mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE) {
		return;
	}
	
	for (int i = 0; i < (*node).mNumMeshes; ++i) {
		const struct aiMesh* mesh = (*model).mMeshes[(*node).mMeshes[i]];
		for (int k = 0; k < (*mesh).mNumFaces; ++k) {
			const struct aiFace face = (*mesh).mFaces[k];
			
			GLenum faceMode;
			switch (face.mNumIndices) {
				case 1: faceMode = GL_POINTS; break;
				case 2: faceMode = GL_LINES; break;
				case 3: faceMode = GL_TRIANGLES; break;
				default: faceMode = GL_POLYGON; break;
			}
			
			glBegin(faceMode);
			
			glColor3f(1.0f, 1.0f, 1.0f);
			for (int l = 0; l < face.mNumIndices; ++l) {
				glVertex3fv((const GLfloat*) &(*mesh).mVertices[face.mIndices[l]]);
			}
			
			glEnd();
		}
	}
	
	for (int i = 0; i < (*node).mNumChildren; ++i) {
		drawModelRecursive(model, (*node).mChildren[i]);
	}
}
