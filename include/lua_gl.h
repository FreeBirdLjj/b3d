#ifndef __B3D_LUA_GL_H__
#define __B3D_LUA_GL_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <float.h>
#include <stdlib.h>

#include "my_lua.h"
#include "lua_bind.h"

LUALIB_API int luaopen_gl(lua_State *L);

#endif
