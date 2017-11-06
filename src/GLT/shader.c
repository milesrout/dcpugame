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

#include "GLT/error.h"
#include "GLT/shader.h"

#include "perrorf.h"
#include "readfile.h"

GLuint gltCreateShader(GLenum type)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0)
		gltPrintErrors("glCreateShader");

	return shader;
}

GLint gltShaderSource(GLuint shader, const GLchar *src)
{
	glShaderSource(shader, 1, &src, NULL);
	return gltPrintErrors("glShaderSource");
}

static void gltPrintShaderInfoLog(GLuint shader)
{
	GLint len, res;
	GLchar *log;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	res = gltPrintErrors("glGetShaderiv");
	if (res == -1)
		return;

	if (len == 0)
		return;

	log = malloc(len);
	if (log == NULL) {
		perror("malloc");
		return;
	}

	glGetShaderInfoLog(shader, len, NULL, log);
	res = gltPrintErrors("glGetShaderInfoLog");
	if (res == -1) 
		goto error;

	log[len] = '\0';

	res = fprintf(stderr, "OpenGL Shader Info Log: %s\n", log);
	if (res < 0) 
		goto error;

error:
	free(log);
}

GLint gltCompileShader(GLuint shader)
{
	GLint status, res;

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		res = 0;

	gltPrintShaderInfoLog(shader);
error:
	res = gltPrintErrors("glCompileShader");
end:
	return res;
}

GLuint gltShader(GLenum type, const char *src)
{
	GLint res;
	GLuint shader = gltCreateShader(type);
	if (shader == 0)
		goto error1;

	res = gltShaderSource(shader, src);
	if (res == -1)
		goto error2;

	res = gltCompileShader(shader);
	if (res == -1)
		goto error2;

	return shader;
error2:
	glDeleteShader(shader);
error1:
	return shader;
}

static char *shaderfilename(const char *name, GLenum type)
{
	char *str;
	int res;

	if (type == GL_VERTEX_SHADER)
		res = asprintf(&str, "shaders/%s.vert", name);
	else if (type == GL_FRAGMENT_SHADER)
		res = asprintf(&str, "shaders/%s.frag", name);
	else
		return NULL;

	if (res == -1) {
		perror("asprintf");
		str = NULL;
	}

	return str;
}

GLint gltLoadShader(GLuint *shader, const char *name, GLenum type)
{

	int res, fd;
	char *filename, *src;

	*shader = -1;

	filename = shaderfilename(name, type);
	if (filename == NULL)
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
	res = (*shader == -1) ? -1 : 0;
	free(src);

error2:
	fd = close(fd);
	if (fd == -1) {
		perrorf("close(%s)", filename);
		res = -1;
	}
error1:
	free(filename);
	return res;
}
