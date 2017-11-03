extern GLuint gltCreateShader(GLenum type);
extern GLint  gltShaderSource(GLuint shader, const GLchar *src);
extern GLint  gltCompileShader(GLuint shader);
extern GLuint gltShader(GLenum type, const char *src);
extern GLint  gltLoadShader(GLuint *shader, const char *name, GLenum type);
