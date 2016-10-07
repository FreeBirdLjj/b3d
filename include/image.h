#ifndef __B3D_IMAGE_H__
#define __B3D_IMAGE_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <assert.h>
#include <lua.h>
#include <jpeglib.h>

#include "my_lua.h"

/* rgba image type */
typedef struct {
	int nx, ny;
	GLubyte *pixels;
	GLboolean texture_is_valid;
	GLuint gl_texture_name;
} image_t;

LUALIB_API int lua_openimage(lua_State *L);

#endif
