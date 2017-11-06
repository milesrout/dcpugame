typedef struct {
	GLTvector3f ver_position;
	GLTvector3f ver_normal;
	GLTvector2f ver_texcoords;
} GLTvertex;
typedef struct {
	GLuint      sh_vao;
	GLTvertex  *sh_vertices;
	GLuint     *sh_indices;
	GLsizei     sh_vertex_count;
	GLsizei     sh_index_count;
} GLTshape;
extern GLint     gltLoadShape(GLTshape **shape, const char *name);
extern GLTshape *gltCreateShape(GLTvertex  *vertices, GLsizei vertex_count,
                                GLuint     *indices,  GLsizei index_count);
enum GLTvertexattrib {
	GLT_POSITION_ATTRIB = 0,
	GLT_NORMAL_ATTRIB = 1,
	GLT_TEXCOORDS_ATTRIB = 2
};
