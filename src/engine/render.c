#include "render.h"

#include <stdbool.h>
#include <GL/freeglut_std.h>
#define  GL_GLEXT_PROTOTYPES
#include <GL/glext.h>
#undef GL_GLEXT_PROTOTYPES

#include "geometry.h"
#include "performance.h"

float viewportAspectRatio = 1.0f;
const Scene* cameraAnchor;

static const float AXIS_RADIUS = 5.0f;

static void renderScene(const Scene*, const Transform baseTransform);
static void setupCamera();
static void moveCameraTo(const Scene* scene);
static void drawAxes();
static void drawSolid(const Solid* solid);



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

void renderFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	setupCamera();
	moveCameraTo(cameraAnchor);

	renderScene(currentScene, identity());

	glFlush();
	glutSwapBuffers();
	frameRendered();
	glutPostRedisplay();
}

static void renderScene(const Scene* scene, const Transform baseTransform) {
	if (!scene) {
		return;
	}

	Transform transform = multiply(scene->transform, baseTransform);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((const GLfloat*) &transform);

	glDisable(GL_LIGHTING);
	drawAxes();
	glEnable(GL_LIGHTING);

	if (scene->solid) {
		glEnable(GL_LIGHT0);
		glEnable(GL_TEXTURE_2D);
		drawSolid(scene->solid);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHT0);
	}

	for (size_t i = 0; i < scene->numChildren; ++i) {
		renderScene(scene->children[i], transform);
	}
}

static void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-16.0,
	        16.0,
	        -16.0/viewportAspectRatio,
	        16.0/viewportAspectRatio,
	        -128.0,
	        128.0);
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
}

static void moveCameraTo(const Scene* anchor) {
	glMatrixMode(GL_PROJECTION);
	Vector3D pos = translationOf(worldTransform(anchor));
	glTranslatef(-pos.x, -pos.y, -pos.z);
}

static void drawAxes() {
	glMatrixMode(GL_MODELVIEW);
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

static void drawSolid(const Solid* solid) {
	if (solid == NULL) {
		return;
	}
	
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.5f, 1.0f, 0.0f);
	
	for (size_t meshIndex = 0; meshIndex < solid->numMeshes; ++meshIndex) {
		const Mesh mesh = solid->meshes[meshIndex];
		glBindTexture(GL_TEXTURE_2D,
		              solid->materials[mesh.materialIndex].textureId);
		bool hasNormals = mesh.normals != NULL;
		bool hasTextureCoords = mesh.textureCoords != NULL;
		
		for (size_t faceIndex = 0; faceIndex < mesh.numFaces; ++faceIndex) {
			const Face face = mesh.faces[faceIndex];
			
			GLenum faceMode;
			switch (face.numIndices) {
				case 1: faceMode = GL_POINTS; break;
				case 2: faceMode = GL_LINES; break;
				case 3: faceMode = GL_TRIANGLES; break;
				default: faceMode = GL_POLYGON; break;
			}
			
			glBegin(faceMode);
			
			for (size_t i = 0; i < face.numIndices; ++i) {
				size_t vertIndex = face.indices[i];
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
