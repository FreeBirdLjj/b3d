#ifndef VIEW_INCLUDED
#define VIEW_INCLUDED

#include <GL/glut.h>
#include "mesh.h"
#include "my_lua.h"

void view(char window_title[]);
int l_set_idle_callback(lua_State *L);
void menu_callback(int option);

#endif

