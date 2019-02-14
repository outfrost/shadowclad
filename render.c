#include <GL/glut.h>
#include <stdbool.h>

#include "level.h"
#include "performance.h"
#include "typedefs.h"

const float AXIS_RADIUS = 5.0f;

static void drawAxes();
static void renderBlockGrid(const BlockGrid grid);
static void drawBlock(const Block* block);



void initRender() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	GLfloat light0_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0_specular[] = {0.96f, 0.98f, 1.0f, 1.0f};
	GLfloat light0_position[] = {5.0f, 10.0f, 5.0f, 0.0f}; // (w == 0.0f) == directional
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005f);
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_LIGHTING);
	drawAxes();
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	renderBlockGrid(levelGrid);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	
	glFlush();
	glutSwapBuffers();
	frameRendered();
	glutPostRedisplay();
}

static void drawAxes() {
	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(AXIS_RADIUS, 0.0f, 0.0f);
	glEnd();
	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, AXIS_RADIUS, 0.0f);
	glEnd();
	// Z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, AXIS_RADIUS);
	glEnd();
}

static void renderBlockGrid(const BlockGrid grid) {
	glMatrixMode(GL_MODELVIEW);
	for (int z = 0; z < grid.depth; ++z) {
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, z * BLOCKGRID_CELL_SIZE);
		for (int x = 0; x < grid.width; ++x) {
			drawBlock(getBlockFromGrid(grid, x, z));
			glTranslatef(BLOCKGRID_CELL_SIZE, 0.0f, 0.0f);
		}
	}
	glLoadIdentity();
}

static void drawBlock(const Block* block) {
	if (block->asset3D == NULL) {
		return;
	}
	
	glColor3f(0.5f, 1.0f, 0.0f);
	
	const Asset3D* asset3D = block->asset3D;
	for (int meshIndex = 0; meshIndex < asset3D->numMeshes; ++meshIndex) {
		const Mesh mesh = asset3D->meshes[meshIndex];
		glBindTexture(GL_TEXTURE_2D,
		              asset3D->materials[mesh.materialIndex].textureId);
		bool hasNormals = mesh.normals != NULL;
		bool hasTextureCoords = mesh.textureCoords != NULL;
		
		for (int faceIndex = 0; faceIndex < mesh.numFaces; ++faceIndex) {
			const Face face = mesh.faces[faceIndex];
			
			GLenum faceMode;
			switch (face.numIndices) {
				case 1: faceMode = GL_POINTS; break;
				case 2: faceMode = GL_LINES; break;
				case 3: faceMode = GL_TRIANGLES; break;
				default: faceMode = GL_POLYGON; break;
			}
			
			glBegin(faceMode);
			
			for (int i = 0; i < face.numIndices; ++i) {
				unsigned int vertIndex = face.indices[i];
				if (hasNormals) {
					if (hasTextureCoords) {
						Vector3D coords = mesh.textureCoords[vertIndex];
						glTexCoord2f(coords.x, coords.y);
					}
					Vector3D normal = mesh.normals[vertIndex];
					glNormal3f(normal.x, normal.y, normal.z);
				}
				Vector3D vertex = mesh.vertices[vertIndex];
				glVertex3f(vertex.x, vertex.y, vertex.z);
			}
			
			glEnd();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
