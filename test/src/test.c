#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <GL/glut.h>

#include "memfile.h"
#include "resfile.h"
#include "mmpfile.h"

resfile* res;
GLuint texid;

static bool bindtex(int index)
{
	memfile* mem = resfile_node_memfile(index, res);
	if (NULL == mem) {
		return false;
	}
	texid = mmpfile_create_texture(texid, mem);
	memfile_close(mem);
	if (0 == texid) {
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, texid);
	return true;
}

static void display(void)
{
	const float w = 2.0f, h = 2.0f;

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(w, -h);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(w, h);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -h);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(-w, -h);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(-w, h);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(w, h);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(w, -h);
	glEnd();

	glutSwapBuffers();
}

static void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLdouble)width / height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

static void keyboard(unsigned char key, int x, int y)
{
	x = x;
	y = y;
	switch (key) {
	case 27: // escape
		resfile_close(res);
		exit(0);
		break;
	}
}

static void timer(int index)
{
	if (0 == resfile_node_count(res)) {
		return;
	}

	if (index >= resfile_node_count(res)) {
		index = 0;
	}

	if (!bindtex(index)) {
		printf("Could not load texture '%s'\n", resfile_node_name(index, res));
		glutTimerFunc(0, timer, index + 1);
		return;
	}

	glutTimerFunc(1000, timer, true ?
		rand() % resfile_node_count(res) : index + 1);

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	if (argc < 2) {
		printf("Usage: %s res <name>\n", argv[0]);
		return 1;
	}

	glutInitDisplayMode(GLUT_RGB | GLUT_ALPHA);

	glutInitWindowPosition(300, 500);
	glutInitWindowSize(400, 300);
	glutInit(&argc, argv);

	glutCreateWindow("test");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f);

	res = resfile_open(argv[1]);
	if (NULL == res) {
		printf("Could not open file '%s'\n", argv[1]);
		return 1;
	}

	if (argc > 2) {
		int index = resfile_node_index(argv[2], res);
		if (index < 0) {
			printf("Could not find texture '%s'\n", argv[2]);
		} else if (!bindtex(index)) {
			printf("Could not load texture '%s'\n", argv[2]);
		}
	} else {
		glutTimerFunc(0, timer, 0);
	}

	glutMainLoop();
	return 0;
}
