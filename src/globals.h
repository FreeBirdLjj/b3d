#ifndef __B3D_GLOBALS_H__
#define __B3D_GLOBALS_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <lua.h>

#define MAX_LABEL_LEN	10*80

const enum {
	NS_DIV_BY_AREA,
	NS_NORMALIZE,
	NS_MUL_BY_AREA,
	NS_FLAT
};

/* These are defined in main.c */
extern const int normal_style, verbosity;
extern lua_State *lua_state;
extern const char *init_filename;

extern const GLdouble alpha;
extern const GLenum polygon_mode;

#endif
