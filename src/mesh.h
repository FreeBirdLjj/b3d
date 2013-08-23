#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "my_lua.h"
#include <GL/glut.h>

typedef struct{
	float n[3];	/* normal */
	float p[3];	/* position */
}vertex_t;

typedef struct{
	int iv[3];	/* vertex indices */
}triangle_t;

typedef struct{
	vertex_t vv[3];
}gl_triangle_t;

typedef struct{ 
	int nv, nt;		/* number of vertices and triangles */
	vertex_t *verts; 
	triangle_t *tris; 
	gl_triangle_t *gl_tris; 
	GLuint gl_display_list;
	GLfloat pmin[3];	/* min corner point of the bounding box */
	GLfloat pmax[3];	/* max corner point */
}mesh_t;

int luaopen_mesh(lua_State *L);

#endif

