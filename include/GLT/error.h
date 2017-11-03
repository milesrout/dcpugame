extern const GLchar *gltErrorString(GLenum);
extern GLint         gltPrintErrorsImpl(const char *source, const GLchar *file, int line);
#define gltPrintErrors(source) gltPrintErrorsImpl(source, __FILE__, __LINE__)
