#ifndef BRAINMAPS_IMAGE_INCLUDED
#define BRAINMAPS_IMAGE_INCLUDED

#include <lua.h>
#include <GL/glut.h>

/* rgba image type */
typedef struct{
	int nx, ny;
	GLubyte *pixels;
	GLboolean texture_is_valid;
	GLuint gl_texture_name;
}image_t;

int lua_openimage(lua_State *L);

#endif

