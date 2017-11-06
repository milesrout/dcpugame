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
#include "GLT/maths.h"
#include "GLT/shape.h"
#include "GLT/texture.h"

#include "perrorf.h"
#include "readfile.h"

static int ispow2(int n)
{
	return n && (n & (n - 1)) == 0;
}

static int appendindex(GLuint **p, int i, GLuint v)
{
	if (ispow2(i)) {
		GLuint *vs = realloc(*p, 2 * (i + 1) * sizeof(GLuint));
		if (vs == NULL)
			return -1;
		*p = vs;
	}
	(*p)[i] = v;
	return 0;
}

static int appendvertex(GLTvertex **p, int i, GLTvertex v)
{
	if (ispow2(i)) {
		GLTvertex *vs = realloc(*p, 2 * (i + 1) * sizeof(GLTvertex));
		if (vs == NULL)
			return -1;
		*p = vs;
	}
	(*p)[i] = v;
	return 0;
}

static int parseindices(const char *src, GLuint **pindices, GLsizei *pindex_count)
{
	GLuint *vs;
	GLuint v;
	int i, n = 0, m = 0;

	vs = malloc(2 * sizeof(GLuint));
	if (vs == NULL) {
		perror("malloc");
		return -1;
	}

	sscanf(src, "%u%n", pindex_count, &n);
	for (i = 0; i < *pindex_count; i++) {
		sscanf(src + n, "%u%n", &v, &m); n += m;
		if (appendindex(&vs, i, v) == -1) {
			free(vs);
			return -1;
		}
	}

	*pindices = vs;
	return n;
}

static int parsevertices(const char *src, GLTvertex **pvertices, GLsizei *pvertex_count)
{
	GLTvertex *vs;
	GLTvertex v;
	int i, n = 0, m = 0;

	vs = malloc(2 * sizeof(GLTvertex));
	if (vs == NULL) {
		perror("malloc");
		return -1;
	}

	sscanf(src, "%u%n", pvertex_count, &n);
	for (i = 0; i < *pvertex_count; i++) {
		sscanf(src + n, "%f%n", &v.ver_position.x, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_position.y, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_position.z, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_normal.x, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_normal.y, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_normal.z, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_texcoords.x, &m); n += m;
		sscanf(src + n, "%f%n", &v.ver_texcoords.y, &m); n += m;
		if (appendvertex(&vs, i, v) == -1) {
			free(vs);
			return -1;
		}
	}

	*pvertices = vs;
	return n;
}

static char *shapefilename(const char *name)
{
	char *str;
	int res;

	res = asprintf(&str, "mesh/%s.shape", name);
	if (res == -1) {
		perror("asprintf");
		str = NULL;
	}

	return str;
}

GLint gltParseShape(GLTvertex  **pvertices, GLsizei *pvertex_count,
		GLuint     **pindices,  GLsizei *pindex_count,
		const char *src)
{
	GLint res = -1;
	int n;

	n = parsevertices(src, pvertices, pvertex_count);
	if (n == -1)
		return -1;
	n = parseindices(src + n, pindices, pindex_count);
	if (n == -1) {
		free(*pvertices);
		return -1;
	}

	return 0;
}

GLTshape *gltCreateShape(GLTvertex  *vertices, GLsizei vertex_count,
                         GLuint     *indices,  GLsizei index_count)
{
	GLuint vao, vbo, ebo;
	GLTshape *shape = malloc(sizeof *shape);
	if (shape == NULL)
		return NULL;

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLTvertex));
	glNamedBufferData(vbo, vertex_count * sizeof(GLTvertex), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(vao, GLT_POSITION_ATTRIB);
	glVertexArrayAttribFormat(vao, GLT_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE,
			offsetof(GLTvertex, ver_position));

	glEnableVertexArrayAttrib(vao, GLT_NORMAL_ATTRIB);
	glVertexArrayAttribFormat(vao, GLT_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE,
			offsetof(GLTvertex, ver_normal));

	glEnableVertexArrayAttrib(vao, GLT_TEXCOORDS_ATTRIB);
	glVertexArrayAttribFormat(vao, GLT_TEXCOORDS_ATTRIB, 3, GL_FLOAT, GL_FALSE,
			offsetof(GLTvertex, ver_texcoords));

	glCreateBuffers(1, &ebo);
	glVertexArrayElementBuffer(vao, ebo);
	glNamedBufferData(ebo, index_count * sizeof(GLuint), indices, GL_STATIC_DRAW);

	shape->sh_vao = vao;

	shape->sh_vertices = vertices;
	shape->sh_vertex_count = vertex_count;

	shape->sh_indices = indices;
	shape->sh_index_count = index_count;

	return shape;
}

GLint gltLoadShape(GLTshape **shape, const GLchar *name)
{
	GLint res = -1, err;
	int fd, result;
	char *filename, *src;
	GLTvertex *vertices;
	GLuint *indices;
	GLsizei vertex_count, index_count;

	filename = shapefilename(name);
	if (filename == NULL)
		return -1;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perrorf("open(\"%s\")", filename);
		goto error1;
	}

	src = readfd(fd);
	if (src == NULL)
		goto error2;

	err = gltParseShape(&vertices, &vertex_count, &indices, &index_count, src);
	if (err == -1)
		goto error3;

	*shape = gltCreateShape(vertices, vertex_count, indices, index_count);
	if (*shape == NULL)
		goto error4;

	res = 0;
	goto error3;

error4:
	free(vertices);
error3:
	free(src);
error2:
	result = close(fd);
	if (result == -1) {
		res = result;
		perror("close");
	}
error1:
	free(filename);
	return res;
}
