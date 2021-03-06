/* Much of the code in this file comes from lua-5.2.3/src/lua/lua.c */

#include "my_lua.h"

static int l_reset_menu(lua_State *L)
{
	if (luaL_loadbuffer(L, "menu_callbacks={}", /* strlen */ 17 , "cmd") || lua_pcall(L, 0, 0, 0)) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);	/* pop error message from the stack */
	}

	glutCreateMenu(menu_callback);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return 0;
}

static int l_add_menu_item(lua_State *L)
{
	static int index = 0;

	luaL_checktype(L, 2, LUA_TFUNCTION);

	/* Add the callback function to the menu_callbacks table,
	 * with the index as its key. */
	lua_getglobal(L, "menu_callbacks");
	lua_pushnumber(L, index);
	lua_pushvalue(L, -3);		/* copy the callback to the top of the stack */

	assert(lua_istable(L, -3));
	assert(lua_isnumber(L, -2));
	assert(lua_isfunction(L, -1));

	lua_settable(L, -3);

	glutAddMenuEntry(luaL_checkstring(L, 1), index);

	lua_pushnumber(L, index++);

	return 1;
}

/* Some of the code in this function was taken from Microsoft's
   documentation on their website. */
static void run_process_in_background(const char *command_line)
{
	char *command_line_copy = strdup(command_line);
	const int len = strlen(command_line);

	/* Append the background symbol '&' if it is not already at the end of the
	 * string. */
	if (command_line_copy[len - 1] != '&') {
		command_line_copy = realloc(command_line_copy, len + 2);
		command_line_copy[len] = '&';
		command_line_copy[len + 1] = '\0';
	}

	system(command_line_copy);

	free(command_line_copy);
}

BIND_0_1(run_process_in_background, string);

static int l_get_filename(lua_State *L)
{
	char buf[4 * 80];

	printf("Please enter a filename: ");
	fgets(buf, sizeof(buf), stdin);

	lua_pushstring(L, buf);

	return 1;
}

/* f should be the lua name, without quotes */

#define ENTRY(f)	{# f, l_ ## f}

static const struct luaL_Reg bmlib[] = {
	ENTRY(reset_menu),
	ENTRY(add_menu_item),
	ENTRY(get_filename),
	ENTRY(run_process_in_background),
	ENTRY(set_idle_callback),
	{NULL, NULL},
};

static int l_open_bmlib(lua_State *L)
{
	luaL_newlib(L, bmlib);
	return 1;
}

static const luaL_Reg lualibs[] = {
	{"base", luaopen_base},
	{"image", lua_openimage},
	{"mesh", luaopen_mesh},
	{"bm", l_open_bmlib},		/* brainmaps functions */
	{"gl", luaopen_gl},
	{"glu", luaopen_glu},
	{"glut", luaopen_glut},
	{NULL, NULL},
};

static int file_exists(const char filename[])
{
	FILE *file = fopen(filename, "r");

	if (file == NULL) {
		return 0;
	}

	fclose(file);

	return 1;
}

static char *get_current_dir(void)
{
	char *buf = (char *)malloc(80);

	getcwd(buf, 80);

	return buf;
}

/* based on http://www.lua.org/pil/24.1.html */
/* Global inputs: config_filename, init_filename */
/* Global outputs: lua_state */
void brainmaps_start_lua(void)
{
	lua_state = luaL_newstate();

	if (lua_state == NULL) {
		fprintf(stderr, "Fatal error: Could not start Lua interpreter.\n");
		exit(-1);
	}

	/* Load libraries */
	luaL_openlibs(lua_state);

	const luaL_Reg *lib;

	for (lib = lualibs; lib->func; lib++) {

		if (verbosity) {
			fprintf(stderr, "Loading lua lib: %s\n", lib->name);
		}

		luaL_requiref(lua_state, lib->name, lib->func, 0);	/* open library */
		lua_pop(lua_state, 1);					/* discard any results */
	}

	l_reset_menu(lua_state);

	/* Search up the directory tree for a while to find the init script. */
	while (strcmp(get_current_dir(), "/")) {
		if (file_exists(init_filename)) {
			if (luaL_loadfile(lua_state, init_filename) || lua_pcall(lua_state, 0, 0, 0)) {
				fprintf(stderr, "Fatal error: The init file did not run: %s\n", lua_tostring(lua_state, -1));
				exit(-1);
			} else {
				break;	/* The init file has successfully run.  Our search is over. */
			}
		} else {
			if (verbosity) {
				fprintf(stderr,	"%s is not in the current directory.  Changing to parent directory.\n", init_filename);
			}

			chdir("..");
		}
	}
}
