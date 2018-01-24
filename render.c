#include <GL/glut.h>

#include "render.h"
#include "typedefs.h"

const float AXIS_RADIUS = 5.0f;

void render_scene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	draw_axes();
	
	test_draw_wall();
	
	glFlush();
	glutSwapBuffers();
}

void draw_axes() {
	Point3f x_axis_start = { -AXIS_RADIUS, 0.0f, 0.0f };
	Point3f x_axis_end = { AXIS_RADIUS, 0.0f, 0.0f };
	Point3f y_axis_start = { 0.0f, -AXIS_RADIUS, 0.0f };
	Point3f y_axis_end = { 0.0f, AXIS_RADIUS, 0.0f };
	Point3f z_axis_start = { 0.0f, 0.0f, -AXIS_RADIUS };
	Point3f z_axis_end = { 0.0f, 0.0f, AXIS_RADIUS };
	
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

void test_draw_wall() {
	
}
