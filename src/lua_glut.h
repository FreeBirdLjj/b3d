#ifndef __B3D_LUA_GLUT_H__
#define __B3D_LUA_GLUT_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "my_lua.h"
#include "lua_bind.h"

int luaopen_glut(lua_State *L);

#endif
