#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "GLT/error.h"
#include "GLT/shader.h"

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

	log = malloc(len);
	if (log == NULL) {
		perror("malloc");
		return;
	}

	glGetShaderInfoLog(shader, len, NULL, log);
	res = gltPrintErrors("glGetShaderInfoLog");
	if (res != -1) 
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
	char *log;

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

GLint gltLoadShader(GLuint *shader, const char *name, GLenum type)
{

}
