#ifndef __B3D_MY_LUA_H__
#define __B3D_MY_LUA_H__

#include <math.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "image.h"
#include "mesh.h"
#include "globals.h"
#include "utils.h"
#include "view.h"
#include "lua_gl.h"
#include "lua_glu.h"
#include "lua_glut.h"

LUALIB_API void brainmaps_start_lua(void);

#endif
