#define _GNU_SOURCE
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "GLT/glt.h"
#include "perrorf.h"
#include "readfile.h"

static char *shaderfilename(const char *name, const char *ext)
{
	char *str;
	int res;

	res = asprintf(&str, "shaders/%s%s", name, ext);
	if (res == -1) {
		perror("asprintf");
		str = NULL;
	}

	return str;
}

int loadshader(GLuint *shader, const char *name, GLenum type, const char *ext)
{
	int res, fd;
	char *filename, *src;

	filename = shaderfilename(name, ext);
	if (name == NULL)
		return -1;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		if (errno == ENOENT) {
			*shader = -1;
			return 0;
		}

		perrorf("open(%s)", filename);
		goto error1;
	}

	src = readfd(fd);
	if (src == NULL) {
		goto error2;
	}
	
	*shader = gltShader(type, src);
	res = (*shader != -1);
	free(src);

error2:
	res = close(fd);
	if (res == -1)
		perrorf("close(%s)", filename);
error1:
	free(filename);
	return res;
}


GLuint loadprogram(const char *name)
{
	GLuint vert=0, frag=0, program=0;
	int res;

	res = loadshader(&vert, name, GL_VERTEX_SHADER, ".vert");
	if (res == -1)
		goto error1;

	res = loadshader(&frag, name, GL_FRAGMENT_SHADER, ".frag");
	if (res == -1)
		goto error2;

	program = glCreateProgram();
	if (program == 0)
		goto error3;

	if (vert != -1) glAttachShader(program, vert);
	if (frag != -1) glAttachShader(program, frag);
	glLinkProgram(program);
	return program;

error3:
	glDeleteShader(frag);
error2:
	glDeleteShader(vert);
error1:
	gltPrintErrors("loadprogram");
	return program;
}
