#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "GLT/error.h"

const GLchar *gltErrorString(GLenum err)
{
    switch (err) {
#define EXPAND(x) case x: return #x
        EXPAND(GL_INVALID_ENUM);
        EXPAND(GL_INVALID_VALUE);
        EXPAND(GL_INVALID_OPERATION);
        EXPAND(GL_STACK_OVERFLOW);
        EXPAND(GL_STACK_UNDERFLOW);
        EXPAND(GL_OUT_OF_MEMORY);
        EXPAND(GL_INVALID_FRAMEBUFFER_OPERATION);
        EXPAND(GL_CONTEXT_LOST);
        EXPAND(GL_TABLE_TOO_LARGE);
#undef EXPAND
    }

    return "unknown error";
}


GLint gltPrintErrorsImpl(const GLchar *source, const GLchar *file, int line)
{
    GLenum err;
    GLint res = 0;

    while ((err = glGetError()) != GL_NO_ERROR) {
        res = -1;

        if (source)
            fprintf(stderr, "(%s:%d): OpenGL Error: %s: %s\n", file, line, source, gltErrorString(err));
        else
            fprintf(stderr, "(%s:%d): OpenGL Error: %s\n", file, line, gltErrorString(err));

	/*
	 * This error is currently non-recoverable. In future provision should
	 * be made for it to be recoverable through the disposal of unnecessary
	 * memory consuming resources like caches.
	 */
        if (err == GL_OUT_OF_MEMORY)
            abort();
    }

    return res;
}

