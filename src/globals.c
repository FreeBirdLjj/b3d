#include "globals.h"

/*
 * Definitions of global variables
 */
const int MAX_LABEL_LEN = 10 * 80;

const int normal_style = NS_NORMALIZE, verbosity = 1;
const char *init_filename = "bminit.lua";
lua_State *lua_state = NULL;

const GLdouble alpha = 1.0;
const GLenum polygon_mode = GL_FILL;
