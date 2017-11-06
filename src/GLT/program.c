#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "GLT/error.h"
#include "GLT/shader.h"
#include "GLT/program.h"

static void gltPrintProgramInfoLog(GLuint program)
{
	GLint len, res;
	GLchar *log;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	res = gltPrintErrors("glGetProgramiv");
	if (res == -1)
		return;

	if (len == 0)
		return;

	log = malloc(len);
	if (log == NULL) {
		perror("malloc");
		return;
	}

	glGetProgramInfoLog(program, len, NULL, log);
	res = gltPrintErrors("glGetProgramInfoLog");
	if (res == -1) 
		goto error;

	log[len] = '\0';

	res = fprintf(stderr, "OpenGL Program Info Log: %s\n", log);
	if (res < 0) 
		goto error;

error:
	free(log);
}

GLint gltLinkProgram(GLuint program)
{
	GLint status, res;
	char *log;

	glLinkProgram(program);
	res = gltPrintErrors("glLinkProgram");
	if (res == -1)
		return res;

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	res = gltPrintErrors("glGetProgram");
	if (status == GL_FALSE)
		res = -1;

	gltPrintProgramInfoLog(program);
	return res;
}

GLint gltLoadProgram(GLuint *program, const char *name)
{
	GLuint vert, frag;
	GLint res = 0;

	res = gltLoadShader(&vert, name, GL_VERTEX_SHADER);
	if (res == -1)
		goto error1;

	res = gltLoadShader(&frag, name, GL_FRAGMENT_SHADER);
	if (res == -1)
		goto error2;

	*program = glCreateProgram();
	if (*program == 0)
		goto error3;

	if (vert != -1) {
		glAttachShader(*program, vert);
		res = gltPrintErrors("glAttachShader");
		if (res == -1)
			goto error3;
	}
	if (frag != -1) {
		glAttachShader(*program, frag);
		res = gltPrintErrors("glAttachShader");
		if (res == -1)
			goto error3;
	}

	res = gltLinkProgram(*program);
	if (res == -1)
		goto error3;

	goto error1;

error3:
	glDeleteShader(frag);
error2:
	glDeleteShader(vert);
error1:
	gltPrintErrors("gltLoadProgram");
	return res;
}
