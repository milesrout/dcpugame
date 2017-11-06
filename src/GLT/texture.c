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

#include "perrorf.h"

#include "stb_image.h"

GLint gltLoadTexture2D(const char *filename)
{
	GLuint tex;
	GLint res;
	int x, y, n;
	char *data;
	
	data = stbi_load(filename, &x, &y, &n, 0);
	if (data == NULL) {
		fprintf(stderr, "stbi_load: %s\n", stbi_failure_reason());
		return -1;
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	res = gltPrintErrors("gltLoadTexture");
	if (res == -1)
		tex = -1;

	stbi_image_free(data);
	return tex;
}
