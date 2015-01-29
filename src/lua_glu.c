#include "lua_glu.h"

BIND_1_1(string, gluErrorString, integer)

BIND_1_1(string, gluGetString, integer)

BIND_0_9(gluLookAt, number, number, number, number, number, number, number, number, number)

BIND_0_4(gluOrtho2D, number, number, number, number)

BIND_0_4(gluPerspective, number, number, number, number)

/* The viewport argument is optional. */
int l_gluPickMatrix(lua_State *L){
	int i;
	GLint viewport[4];

	if(lua_gettop(L)==8){
		for(i = 0; i<4; i++){
			viewport[i] = luaL_checkinteger(L, i+5);
		}
	}
	else{
		glGetIntegerv(GL_VIEWPORT,viewport);
	}
	gluPickMatrix(
		luaL_checknumber(L, 1),
		luaL_checknumber(L, 2),
		luaL_checknumber(L, 3),
		luaL_checknumber(L, 4),
		viewport
	);

	return 0;
}

/* The model, proj, and view arguments are omitted. */
int l_gluProject(lua_State *L){
	GLint viewport[4];
	GLdouble model[16], proj[16], x, y, z;

	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, viewport);
	if(!gluProject(luaL_checknumber(L, 1),	luaL_checknumber(L, 2),	luaL_checknumber(L, 3),	model, proj, viewport, &x, &y, &z)){
		luaL_error(L, "gluProject() failed");
	}

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

/* The model, proj, and view arguments are omitted. */
int l_gluUnProject(lua_State *L){
	GLint viewport[4];
	GLdouble model[16], proj[16], x, y, z;

	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, viewport);
	if(!gluUnProject(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), model, proj, viewport, &x, &y, &z)){
		luaL_error(L, "gluUnProject() failed");
	}

	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	return 3;
}

#define ENTRY(f)	{# f, l_ ## f}

static const struct luaL_Reg glu_lib[] = {
	ENTRY(gluErrorString),
	ENTRY(gluGetString),
	ENTRY(gluLookAt),
	ENTRY(gluOrtho2D),
	ENTRY(gluPerspective),
	ENTRY(gluPickMatrix),
	ENTRY(gluProject),
	ENTRY(gluUnProject),
	{NULL, NULL},
};

int luaopen_glu(lua_State *L){
	luaL_newlib(L, glu_lib);
	return 1;
}

