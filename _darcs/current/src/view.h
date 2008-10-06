#ifndef VIEW_INCLUDED
#define VIEW_INCLUDED

#include "my_opengl.h"
#include "mesh.h"
#include "my_lua.h"

void view(char window_title[]);
int l_get_mouse_location(lua_State* L);
int l_draw_string(lua_State* L);
int l_set_color(lua_State* L);
int l_set_idle_callback(lua_State* L);
void menu_callback(int option);

#endif

