#ifndef __B3D_LUA_GLU_H__
#define __B3D_LUA_GLU_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "my_lua.h"
#include "lua_bind.h"

LUALIB_API int luaopen_glu(lua_State *L);

#endif
