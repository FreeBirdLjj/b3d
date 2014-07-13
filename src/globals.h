#ifndef __B3D_GLOBALS_H__
#define __B3D_GLOBALS_H__

#include <GL/glut.h>
#include <lua.h>

#define MAX_LABEL_LEN	10*80

#define NS_DIV_BY_AREA	0
#define NS_NORMALIZE	1
#define NS_MUL_BY_AREA	2
#define NS_FLAT		3

#define MAX_MESHES	10000

/* These are defined in main.c */
extern int normal_style, verbosity;
extern lua_State *lua_state;
extern char *progname, *init_filename;

extern GLdouble alpha;
extern GLenum polygon_mode;

#endif
