#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#include "mesh.h"
#include "view.h"
#include <lua.h>

#define MAX_LABEL_LEN 10*80

#ifndef WIN32
#define SOCKET int
#endif

#define NS_DIV_BY_AREA 0
#define NS_NORMALIZE 1
#define NS_MUL_BY_AREA 2
#define NS_FLAT 3

#define MAX_MESHES 10000

/* These are defined in main.c */
extern mesh_t* the_meshes[];
extern int num_meshes;
extern int normal_style;
extern int num_labels;
extern int verbosity;
extern char *progname;
extern lua_State *lua_state;
extern char *init_filename;
extern char *config_filename;

extern GLdouble alpha;
extern GLenum polygon_mode;

#endif

