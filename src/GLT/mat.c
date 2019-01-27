#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <sys/mman.h> */
#include <sys/stat.h>
#include <sys/types.h>

/* http://glad.dav1d.de/#profile=core&specification=gl&api=gl%3D4.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c */
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "GLT/glt.h"
#include "readfile.h"

typedef struct GLTmat4f {
	float data[4][4];
} GLTmat4f;

GLTmat4f *
gltCreateMat4()
{
	return malloc(sizeof(GLTmat4f));
}

void gltDestroyMat4(GLTmat4f *m)
{
	free(m);
}

void
gltZeroMat4(GLTmat4f *m)
{
	memset(m, 0, sizeof *m);
}

void
gltIdentityMat4(GLTmat4f *m)
{
	m->data[0][0] = 1.0f;
	m->data[1][1] = 1.0f;
	m->data[2][2] = 1.0f;
	m->data[3][3] = 1.0f;
}

void gltMulMat4(GLTmat4f **m, GLTmat4f *l, GLTmat4f *r)
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			(*m)->data[i][j] = l->data[i][0] * r->data[0][j]
			                 + l->data[i][1] * r->data[1][j]
			                 + l->data[i][2] * r->data[2][j]
			                 + l->data[i][3] * r->data[3][j];
}

int test_mat()
{
	GLTmat4f *m1 = gltCreateMat4();
	GLTmat4f *m2 = gltCreateMat4();
	GLTmat4f *m3 = gltCreateMat4();

	gltZeroMat4(m1);
	m1->data[0][0] = 1;
	m1->data[1][1] = 2;
	m1->data[1][2] = 2;
	m1->data[2][2] = 3;
	m1->data[3][3] = 4;

	gltZeroMat4(m2);
	m2->data[0][0] = 1;
	m2->data[1][1] = 2;
	m2->data[1][2] = 2;
	m2->data[2][2] = 3;
	m2->data[3][3] = 4;

	gltMulMat4(&m3, m1, m2);

	fprintf(stdout, "[%f %f %f %f]\n", m1->data[0][0], m1->data[0][1], m1->data[0][2], m1->data[0][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m1->data[1][0], m1->data[1][1], m1->data[1][2], m1->data[1][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m1->data[2][0], m1->data[2][1], m1->data[2][2], m1->data[2][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m1->data[3][0], m1->data[3][1], m1->data[3][2], m1->data[3][3]);
	fprintf(stdout, "-------------\n");
	fprintf(stdout, "[%f %f %f %f]\n", m2->data[0][0], m2->data[0][1], m2->data[0][2], m2->data[0][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m2->data[1][0], m2->data[1][1], m2->data[1][2], m2->data[1][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m2->data[2][0], m2->data[2][1], m2->data[2][2], m2->data[2][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m2->data[3][0], m2->data[3][1], m2->data[3][2], m2->data[3][3]);
	fprintf(stdout, "-------------\n");
	fprintf(stdout, "[%f %f %f %f]\n", m3->data[0][0], m3->data[0][1], m3->data[0][2], m3->data[0][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m3->data[1][0], m3->data[1][1], m3->data[1][2], m3->data[1][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m3->data[2][0], m3->data[2][1], m3->data[2][2], m3->data[2][3]);
	fprintf(stdout, "[%f %f %f %f]\n", m3->data[3][0], m3->data[3][1], m3->data[3][2], m3->data[3][3]);
}
