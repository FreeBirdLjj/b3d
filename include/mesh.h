#ifndef __B3D_MESH_H__
#define __B3D_MESH_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "globals.h"
#include "utils.h"
#include "my_lua.h"

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
	GLuint gl_display_list;
	GLfloat pmin[3];	/* min corner point of the bounding box */
	GLfloat pmax[3];	/* max corner point */
}mesh_t;

LUALIB_API int luaopen_mesh(lua_State *L);

#endif
