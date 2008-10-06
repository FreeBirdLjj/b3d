/* Bitwise operations library */
/* Reuben Thomas   11/00 */

#include "lauxlib.h"
#include "lua.h"

typedef long Integer;
typedef unsigned long UInteger;

#define luaL_check_bit(L,n) ((Integer)luaL_check_number(L, n))
#define luaL_check_ubit(L,n) ((UInteger)luaL_check_bit(L, n))

#define TDYADIC(name, op, t1, t2) \
  static int name(lua_State* L) { \
    lua_pushnumber(L, \
      luaL_check_ ## t1 ## bit(L, 1) op luaL_check_ ## t2 ## bit(L, 2)); \
    return 1; \
  }

#define DYADIC(name, op)  TDYADIC(name, op, , )

#define MONADIC(name, op) \
  static int name(lua_State* L) { \
    lua_pushnumber(L, op luaL_check_bit(L, 1)); \
    return 1; \
  }

MONADIC(int_not,      ~)
DYADIC(int_mod,       %)
DYADIC(int_and,       &)
DYADIC(int_or,        |)
DYADIC(int_xor,       ^)
TDYADIC(int_lshift,  <<, , u)
TDYADIC(int_rshift,  >>, u, u)
TDYADIC(int_arshift, >>, , u)

static const struct luaL_reg bitlib[] = {
{"bnot",    int_not},
{"imod",    int_mod},  /* "mod" already in Lua math library */
{"band",    int_and},
{"bor",     int_or},
{"bxor",    int_xor},
{"lshift",  int_lshift},
{"rshift",  int_rshift},
{"arshift", int_arshift},
{NULL, NULL},
};

LUALIB_API void lua_bitlibopen (lua_State *L) {
    luaL_openlib(L, "bitlib", bitlib, 0);
}
