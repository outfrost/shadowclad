#include <GL/glut.h>
#include <assimp/scene.h>

#include "render.h"
#include "typedefs.h"

const float AXIS_RADIUS = 5.0f;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	
	drawAxes();
	
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	drawModelRecursive(model, (*model).mRootNode);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	
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
		
		applyMaterial((*model).mMaterials[(*mesh).mMaterialIndex]);

		if ((*mesh).mNormals == NULL) {
			glDisable(GL_LIGHTING);
		}
		else {
			glShadeModel(GL_SMOOTH);
			glEnable(GL_LIGHTING);
		}
		
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
				int index = face.mIndices[l];
				if ((*mesh).mColors[0] != NULL) {
					glColor4fv((GLfloat*) &(*mesh).mColors[0][index]);
				}
				if ((*mesh).mNormals != NULL) {
					if ((*mesh).mTextureCoords[0] != NULL) {
						glTexCoord2f(
								(*mesh).mTextureCoords[0][index].x,
								1.0f - (*mesh).mTextureCoords[0][index].y);
						
					}
					
					glNormal3fv(&(*mesh).mNormals[index].x);
				}
				glVertex3fv((const GLfloat*) &(*mesh).mVertices[index].x);
			}
			
			glEnd();
		}
	}
	
	for (int i = 0; i < (*node).mNumChildren; ++i) {
		drawModelRecursive(model, (*node).mChildren[i]);
	}
}

void applyMaterial(const struct aiMaterial* mtl) {
	float c[4];
	
	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	ai_real shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;
	
	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
	
	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	
	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
	
	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
	
	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}
	
	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
	
	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
