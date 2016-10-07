#ifndef __B3D_VIEW_H__
#define __B3D_VIEW_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "globals.h"
#include "utils.h"
#include "mesh.h"
#include "my_lua.h"

void view(char window_title[]);
int l_set_idle_callback(lua_State *L);
void menu_callback(int option);

#endif
