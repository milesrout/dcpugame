#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

// http://glad.dav1d.de/#profile=core&specification=gl&api=gl%3D4.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "GLT/glt.h"
#include "readfile.h"

void error_callback(int error, const char *description)
{
	fprintf(stderr, "Fatal GLFW Error: %s\n", description);
	abort();
}

int main()
{
	GLFWwindow *window;
	GLint posattrib, colattrib;
	GLint ibosize;
	GLuint vao, vbo, ebo, program;
	GLTshape *shape;
	GLint res;

	glfwSetErrorCallback(&error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(640, 480, "Project Trillek", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	res = gltLoadProgram(&program, "null");
	if (res == -1) {
		gltPrintErrors("gltLoadProgram");
		exit(EXIT_FAILURE);
	}

	res = gltPrintErrors("glUseProgram");
	if (res == -1) {
		exit(EXIT_FAILURE);
	}

	res = gltLoadShape(&shape, "cube");
	if (res == -1) {
		gltPrintErrors("gltLoadShape");
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(shape->sh_vao);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 12);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
