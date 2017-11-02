#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// http://glad.dav1d.de/#profile=core&specification=gl&api=gl%3D4.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "readfile.h"

struct vec3f { GLfloat x, y, z; };
struct box {
	struct vec3f center;
	GLfloat width, height, depth;
};

struct box objects[] = {
	{.center = {0.0f, 0.0f, 0.0f}, .width=1.0f, .height=1.0f, .depth=1.0f},
};

GLfloat verts[] = {
	/* front */
	-1.0f, -1.0f, -1.0f,   1.0, 0.0, 0.0,
	 1.0f, -1.0f, -1.0f,   0.0, 1.0, 0.0, 
	 1.0f,  1.0f, -1.0f,   0.0, 0.0, 1.0, 
	-1.0f,  1.0f, -1.0f,   1.0, 1.0, 1.0, 
	/* back */
	-1.0f, -1.0f, -3.0f,   1.0, 0.0, 0.0, 
	 1.0f, -1.0f, -3.0f,   0.0, 1.0, 0.0, 
	 1.0f,  1.0f, -3.0f,   0.0, 0.0, 1.0, 
	-1.0f,  1.0f, -3.0f,   1.0, 1.0, 1.0, 
};

GLushort elems[] = {
	0, 1, 2,
	2, 3, 0,

	1, 5, 6,
	6, 2, 1,

	7, 6, 5,
	5, 4, 7,

	4, 0, 3,
	3, 7, 4,

	4, 5, 1,
	1, 0, 4,

	3, 2, 6,
	6, 7, 3,
};

int main()
{
	GLFWwindow *window;
	GLint posattrib, colattrib;
	GLint ibosize;
	GLuint vao, vbo, ebo;
	GLuint vert, frag, program;
	const char *vertsrc, *fragsrc;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(640, 480, "Project Trillek", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vert = glCreateShader(GL_VERTEX_SHADER);
	vertsrc = readfile("shaders/null.vert");
	if (vertsrc == NULL) exit(EXIT_FAILURE);
	glShaderSource(vert, 1, &vertsrc, NULL);
	glCompileShader(vert);
	free((void *)vertsrc);

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	fragsrc = readfile("shadersnull.frag");
	if (fragsrc == NULL) exit(EXIT_FAILURE);
	glShaderSource(frag, 1, &fragsrc, NULL);
	glCompileShader(frag);
	free((void *)fragsrc);

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glUseProgram(program);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	posattrib = glGetAttribLocation(program, "position");
	glVertexAttribPointer(posattrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(posattrib);

	colattrib = glGetAttribLocation(program, "colour");
	glVertexAttribPointer(colattrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(colattrib);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &ibosize);
		glDrawElements(GL_TRIANGLES, ibosize / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
