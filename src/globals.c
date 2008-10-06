#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef DARWIN
#include <malloc.h>
#endif

#ifdef HAVE_GETOPT
#include <getopt.h>
#endif

#include "view.h"
#include "mesh.h"
#include "globals.h"
#include "my_lua.h"

/* 
 * Definitions of global variables 
 */
mesh_t* the_meshes[MAX_MESHES];
int num_meshes=0;
int normal_style=NS_NORMALIZE;
int verbosity=1;
char *progname=NULL;
lua_State *lua_state = NULL;
char *init_filename="bminit.lua";
char *config_filename="bmconf.lua";

GLdouble alpha=1.0;
GLenum polygon_mode=GL_FILL;

