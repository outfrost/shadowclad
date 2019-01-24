#include <GL/glut.h>

#include "level.h"
#include "performance.h"
#include "render.h"
#include "typedefs.h"

const float AXIS_RADIUS = 5.0f;

void initRender() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	GLfloat light0_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_position[] = {5.0f, 10.0f, 10.0f, 1.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	
	glDisable(GL_LIGHTING);
	drawAxes();
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
	drawSceneRecursive(levelScene, levelScene->mRootNode);
	glDisable(GL_LIGHT0);
	
	glFlush();
	glutSwapBuffers();
	frameRendered();
	glutPostRedisplay();
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

void drawSceneRecursive(const AiScene* scene, const AiNode* node) {
	if (((*scene).mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE) {
		return;
	}
	
	for (int i = 0; i < (*node).mNumMeshes; ++i) {
		const AiMesh* mesh = (*scene).mMeshes[(*node).mMeshes[i]];
		for (int k = 0; k < (*mesh).mNumFaces; ++k) {
			const AiFace face = (*mesh).mFaces[k];
			
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
		drawSceneRecursive(scene, (*node).mChildren[i]);
	}
}
