#include <GL/glut.h>
#include <assimp/scene.h>

#include "render.h"
#include "typedefs.h"

const float AXIS_RADIUS = 5.0f;

void render_scene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	draw_axes();
	draw_model_recursive(model, (*model).mRootNode);
	
	glFlush();
	glutSwapBuffers();
}

void draw_axes() {
	point3f x_axis_start = { 0.0f, 0.0f, 0.0f };
	point3f x_axis_end = { AXIS_RADIUS, 0.0f, 0.0f };
	point3f y_axis_start = { 0.0f, 0.0f, 0.0f };
	point3f y_axis_end = { 0.0f, AXIS_RADIUS, 0.0f };
	point3f z_axis_start = { 0.0f, 0.0f, 0.0f };
	point3f z_axis_end = { 0.0f, 0.0f, AXIS_RADIUS };
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(x_axis_start);
	glVertex3fv(x_axis_end);
	glEnd();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(y_axis_start);
	glVertex3fv(y_axis_end);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3fv(z_axis_start);
	glVertex3fv(z_axis_end);
	glEnd();
}

void draw_model_recursive(const struct aiScene* model, const struct aiNode* node) {
	if (((*model).mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE) {
		return;
	}
	
	for (int i = 0; i < (*node).mNumMeshes; ++i) {
		const struct aiMesh* mesh = (*model).mMeshes[(*node).mMeshes[i]];
		for (int k = 0; k < (*mesh).mNumFaces; ++k) {
			const struct aiFace face = (*mesh).mFaces[k];
			
			GLenum face_mode;
			switch(face.mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
			
			glBegin(face_mode);
			
			glColor3f(1.0f, 1.0f, 1.0f);
			for (int l = 0; l < face.mNumIndices; ++l) {
				glVertex3fv((const GLfloat*) &(*mesh).mVertices[face.mIndices[l]]);
			}
			
			glEnd();
		}
	}
	
	for (int i = 0; i < (*node).mNumChildren; ++i) {
		draw_model_recursive(model, (*node).mChildren[i]);
	}
}
