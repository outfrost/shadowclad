#include "render.h"

#include <stdbool.h>

#include "geometry.h"
#include "performance.h"

#define SCENE_DEBUG_ 0
#define RENDER_DEBUG_ 0

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
	
	//glShadeModel(GL_FLAT);
}

void renderFrame(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	setupCamera();
	moveCameraTo(cameraAnchor);

	renderScene(currentScene, identity());

	glFlush();
	glfwSwapBuffers(window);
	frameRendered();
}

static void renderScene(const Scene* scene, const Transform baseTransform) {
	if (!scene) {
		return;
	}

	Transform transform = multiply(scene->transform, baseTransform);

	glMatrixMode(GL_MODELVIEW);
	glLoadTransposeMatrixf((const GLfloat*) &transform);

	glDisable(GL_LIGHTING);
#if SCENE_DEBUG_
	drawAxes();
#endif // SCENE_DEBUG_
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

#if RENDER_DEBUG_
static GLfloat ab(GLfloat a) {
	return a < 0 ? -a : a;
}
#endif // RENDER_DEBUG_

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
		
		for (size_t faceIndex = 0; faceIndex < mesh.numFaces; ++faceIndex) {
			const Face face = mesh.faces[faceIndex];
			
#if RENDER_DEBUG_
			if (face.normals) {
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);
				for (size_t i = 0; i < face.numIndices; ++i) {
					size_t vertIndex = face.indices[i];
					Vector3D vertex = mesh.vertices[vertIndex];
					Vector3D normal = face.normals[i];
					glColor3f(ab(normal.x), ab(normal.y), ab(normal.z));
					glVertex3f(vertex.x, vertex.y, vertex.z);
					glVertex3f(vertex.x + normal.x, vertex.y + normal.y, vertex.z + normal.z);
				}
				glEnd();
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
			}
#endif // RENDER_DEBUG_
			
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
				if (face.normals) {
					if (mesh.textureCoords) {
						Vector3D coords = mesh.textureCoords[vertIndex];
						glTexCoord2f(coords.x, coords.y);
					}
					Vector3D normal = face.normals[i];
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
