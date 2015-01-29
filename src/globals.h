#ifndef __B3D_GLOBALS_H__
#define __B3D_GLOBALS_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <lua.h>

extern const int MAX_LABEL_LEN;

const enum {
	NS_DIV_BY_AREA,
	NS_NORMALIZE,
	NS_MUL_BY_AREA,
	NS_FLAT
};

/* These are defined in globals.c */
extern const int normal_style, verbosity;
extern lua_State *lua_state;
extern const char *init_filename;

extern const GLdouble alpha;
extern const GLenum polygon_mode;

#endif
