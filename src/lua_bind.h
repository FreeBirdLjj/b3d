#ifndef __B3D_LUA_BIND_H__
#define __B3D_LUA_BIND_H__

#include <lua.h>

/* This macro binds a function with no returns and one argument that maps to a
 * specified Lua type.  The others are analogous, with types t1, t2, etc.
 * -ijt */
#define BIND_0_1(f, type)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## type(L,1));	\
		return 0;	\
	}

#define BIND_0_2(f, t1, t2)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2));	\
		return 0;	\
	}

#define BIND_0_3(f, t1, t2, t3)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2), luaL_check ## t3(L, 3));	\
		return 0;	\
	}

#define BIND_0_4(f, t1, t2, t3, t4)	\
	int l_ ## f(lua_State* L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2),	\
		  luaL_check ## t3(L, 3), luaL_check ## t4(L, 4));	\
		return 0;	\
	}

#define BIND_0_5(f, t1, t2, t3, t4, t5)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2),	\
		  luaL_check ## t3(L, 3), luaL_check ## t4(L, 4),	\
		  luaL_check ## t5(L, 5));	\
		return 0;	\
	}

#define BIND_0_6(f, t1, t2, t3, t4, t5, t6)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2),	\
		  luaL_check ## t3(L, 3), luaL_check ## t4(L, 4),	\
		  luaL_check ## t5(L, 5), luaL_check ## t6(L, 6));	\
		return 0;	\
	}

#define BIND_0_7(f, t1, t2, t3, t4, t5, t6, t7)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2),	\
		  luaL_check ## t3(L, 3), luaL_check ## t4(L, 4),	\
		  luaL_check ## t5(L, 5), luaL_check ## t6(L, 6),	\
		  luaL_check ## t7(L, 7));	\
		return 0;	\
	}

#define BIND_0_8(f, t1, t2, t3, t4, t5, t6, t7, t8)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2),	\
		  luaL_check ## t3(L, 3), luaL_check ## t4(L, 4),	\
		  luaL_check ## t5(L, 5), luaL_check ## t6(L, 6),	\
		  luaL_check ## t7(L, 7), luaL_check ## t8(L, 8));	\
		return 0;	\
	}

#define BIND_0_9(f, t1, t2, t3, t4, t5, t6, t7, t8, t9)	\
	int l_ ## f(lua_State *L){	\
		f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2),	\
		  luaL_check ## t3(L, 3), luaL_check ## t4(L, 4),	\
		  luaL_check ## t5(L, 5), luaL_check ## t6(L, 6),	\
		  luaL_check ## t7(L, 7), luaL_check ## t8(L, 8),	\
		  luaL_check ## t9(L, 9));				\
		return 0;	\
	}

/* Bind a `function' taking no inputs and returning void. */
#define BIND_0_0(f)	\
	int l_ ## f(lua_State *L){	\
		f();		\
		return 0;	\
	}

/* Bind a function with a single input argument and a non-void return value. */
#define BIND_1_1(t_out, f, t_in)	\
	int l_ ## f(lua_State *L){	\
		lua_push ## t_out(L, f(luaL_check ## t_in(L, 1)));	\
		return 1;	\
	}

#define BIND_1_2(t_out, f, t1, t2)	\
	int l_ ## f(lua_State *L){	\
		lua_push ## t_out(L, f(luaL_check ## t1(L, 1), luaL_check ## t2(L, 2)));	\
		return 1;	\
	}

#define BIND_1_0(t_out, f)	\
	int l_ ## f(lua_State *L){	\
		lua_push ## t_out(L, f());	\
		return 1;	\
	}

#endif

