#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <GL/glut.h>

// wheel mouse event
#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#endif
#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN 4
#endif

#include "cemath.h"
#include "cegl.h"
#include "logging.h"
#include "camera.h"
#include "resfile.h"
#include "mprfile.h"

mprfile* mpr;
camera* cam;

bool mouse_left_down;
bool mouse_right_down;
float mouse_x, mouse_y;

int draw_mode;
int val;

static void idle(void)
{
	glutPostRedisplay();
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_setup(cam);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	mprfile_debug_render(val, mpr);

	glutSwapBuffers();
}

static void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

static void keyboard(unsigned char key, int x, int y)
{
	x = x;
	y = y;
    switch (key) {
	case 'd': // switch rendering modes (fill -> wire -> point)
	case 'D':
		draw_mode = (draw_mode + 1) % 3;
		if(draw_mode == 0) {			// fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
		} else if(draw_mode == 1) {		// wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		} else {						// point mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		break;
	case 't':
	case 'T':
		++val;
		break;
    case 27: // escape
		camera_delete(cam);
		mprfile_close(mpr);
		logging_close();
		exit(0);
		break;
	}
}

static void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		camera_move(-1.0f, 0.0f, cam);
		break;
	case GLUT_KEY_UP:
		camera_move(0.0f, 1.0f, cam);
		break;
	case GLUT_KEY_RIGHT:
		camera_move(1.0f, 0.0f, cam);
		break;
	case GLUT_KEY_DOWN:
		camera_move(0.0f, -1.0f, cam);
		break;
	};
}

static void mouse(int button, int state, int x, int y)
{
	mouse_x = x;
	mouse_y = y;

	if (GLUT_LEFT_BUTTON == button) {
		mouse_left_down = GLUT_DOWN == state;
	}

	if (GLUT_RIGHT_BUTTON == button) {
		mouse_right_down = GLUT_DOWN == state;
	}

	if (GLUT_WHEEL_UP == button && GLUT_DOWN == state) {
		camera_zoom(10.0f, cam);
	}

	if (GLUT_WHEEL_DOWN == button && GLUT_DOWN == state) {
		camera_zoom(-10.0f, cam);
	}
}

static void motion(int x, int y)
{
	if (mouse_right_down) {
		camera_yaw_pitch(deg2rad(-0.1f * (x - mouse_x)),
						deg2rad(-0.1f * (y - mouse_y)), cam);
    }

	mouse_x = x;
	mouse_y = y;
}

int main(int argc, char* argv[])
{
	if (3 != argc) {
		printf("Usage: %s ei_path mpr_name\n", argv[0]);
		return 1;
	}

	logging_open();
	logging_set_level(LOGGING_DEBUG_LEVEL);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutInit(&argc, argv);

	glutCreateWindow("Cursed Earth");
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	gl_init();

	cam = camera_new();
	camera_set_eye((float[]){ 50.0f, 100.0f, 50.0f }, cam);
	camera_yaw_pitch(deg2rad(0.0f), deg2rad(60.0f), cam);

	char tex_path[512];
	snprintf(tex_path, sizeof(tex_path), "%s/Res/textures.res", argv[1]);

	char mpr_path[512];
	snprintf(mpr_path, sizeof(mpr_path), "%s/Maps/%s", argv[1], argv[2]);

	resfile* tex_res = resfile_open_file(tex_path);
	if (NULL == tex_res) {
		printf("Could not open file '%s'\n", tex_path);
		return 1;
	}

	resfile* mpr_res = resfile_open_file(mpr_path);
	if (NULL == mpr_res) {
		printf("Could not open file '%s'\n", mpr_path);
		resfile_close(tex_res);
		return 1;
	}

	mpr = mprfile_open(mpr_res, tex_res);
	if (!mpr) {
		printf("Could not open file '%s'\n", mpr_path);
		resfile_close(tex_res);
		resfile_close(mpr_res);
		return 1;
	}

	resfile_close(tex_res);
	resfile_close(mpr_res);

	mprfile_debug_print(mpr);

	glutMainLoop();
	return 0;
}
