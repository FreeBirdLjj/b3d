/* Much of the code in this file comes from lua-5.0.2/src/lua/lua.c */

#ifdef WIN32
#include <windows.h>
#endif

#include "image.h"
#include "mesh.h"
#include "my_lua.h"
#include "globals.h"
#include "utils.h"
#include "view.h"
#include "client.h"
#include "lua_gl.h"
#include "lua_glu.h"
#include "lua_glut.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* Create functions to set and get a global numerical C variable from within
 * lua. */
#define DEFINE_LUA_SETGET_NUMBER(var)\
int l_set_ ## var(lua_State* L)\
{\
    if(!lua_isnumber(L,-1)) {\
        fprintf(stderr,"Argument to set_%s should be a number.\n", #var);\
            return 0;\
    }\
    var = lua_tonumber(L,-1);\
        return 0;\
} \
\
    int l_get_ ## var(lua_State* L)\
{\
    lua_pushnumber(L,var);\
        return 1;\
}

#define DEFINE_LUA_SETGET_BOOLEAN(var)\
int l_set_ ## var(lua_State* L)\
{\
    if(!lua_isboolean(L,-1)) {\
        fprintf(stderr,"Argument to set_%s should be a boolean.\n", #var);\
            return 0;\
    }\
    var = lua_toboolean(L,-1);\
        return 0;\
} \
\
    int l_get_ ## var(lua_State* L)\
{\
    lua_pushboolean(L,var);\
        return 1;\
}

static void stack_dump(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    fprintf(stderr,"== lua stack ==\n");
    for(i=1;i<=top;i++) {
        int t=lua_type(L,i);
        fprintf(stderr,"%s\n",lua_typename(L,t));
    }
    fprintf(stderr,"\n");
}

int l_warn(lua_State* L)
{
    const char* msg=luaL_checkstring(L,1);
    b3d_warn(msg);
    return 0;
}

int l_reset_menu(lua_State* L)
{
    char cmd[]="menu_callbacks={}";
    int error = luaL_loadbuffer(L, cmd, strlen(cmd), "cmd") 
        || lua_pcall(L, 0, 0, 0);
    if (error) {
        fprintf(stderr, "%s", lua_tostring(L, -1));
        lua_pop(L, 1);  /* pop error message from the stack */
    }
    glutCreateMenu(menu_callback);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    return 0;
}

int l_add_menu_item(lua_State* L)
{
    static int index=0;
    const char *name;

    name=luaL_checkstring(L,1);
    luaL_checktype(L,2,LUA_TFUNCTION);

    /* Add the callback function to the menu_callbacks table, 
     * with the index as its key. */
    lua_getglobal(L, "menu_callbacks");
    lua_pushnumber(L, index);
    lua_pushvalue(L,-3); /* copy the callback to the top of the stack */
    assert(lua_istable(L,-3));
    assert(lua_isnumber(L,-2));
    assert(lua_isfunction(L,-1));
    lua_settable(L, -3);

    glutAddMenuEntry(name, index); 
    lua_pushnumber(L,index);
    index++;
    return 1;
}


int l_get_gl_area_size(lua_State* L)
{
    int w,h;
    w=glutGet(GLUT_WINDOW_WIDTH);
    h=glutGet(GLUT_WINDOW_HEIGHT);
    lua_pushnumber(L,w);
    lua_pushnumber(L,h);
    return 2;
}

int l_os(lua_State* L)
{
#ifdef WIN32
    lua_pushstring(L,"windows"); 
#elif defined(DARWIN)
    lua_pushstring(L,"mac"); 
#else
    lua_pushstring(L,"gnu");
#endif
    return 1;
}

int l_system(lua_State* L)
{
    int result;
    const char *cmd;
    cmd = luaL_checkstring(L,1);
    result=system(cmd);
    lua_pushnumber(L,result);
    return 1;
}

/* Some of the code in this function was taken from Microsoft's 
   documentation on their website. */
int l_run_process_in_background(lua_State* L)
{
    int len;
    char* command_line = strdup(luaL_checkstring(L,1));
#ifdef WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line). 
                TEXT(command_line),  
                NULL,             // Process handle not inheritable. 
                NULL,             // Thread handle not inheritable. 
                FALSE,            // Set handle inheritance to FALSE. 
                0,                // No creation flags. 
                NULL,             // Use parent's environment block. 
                NULL,             // Use parent's starting directory. 
                &si,              // Pointer to STARTUPINFO structure.
                &pi )             // Pointer to PROCESS_INFORMATION structure.
      ) 
    {
        char buf[3*80];
        _snprintf(buf, sizeof(buf),"CreateProcess failed with error code %d.", 
                GetLastError());
        MessageBox(NULL,buf,"b3d",MB_OK);
        return 0;
    }
#else
    len=strlen(command_line);
    /* Append the background symbol '&' if it is not already at the end of the
     * string. */
    if(command_line[len-1]!='&') {
        command_line=realloc(command_line,2*strlen(command_line));
        command_line[len]='&';
        command_line[len+1]='\0';
    }
    system(command_line);
#endif
    free(command_line);
    return 0;
}

#ifdef WIN32
int l_get_filename(lua_State* L)
{
	static char result[10*80];
	/* http://msdn.microsoft.com/library/default.asp?
url=/library/en-us/winui/winui/windowsuserinterface/
userinput/commondialogboxlibrary/usingcommondialogboxes.asp */
	OPENFILENAME ofn;       /* common dialog box structure */
	HWND hwnd=NULL;         /* owner window */

	/* Initialize OPENFILENAME */
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	
	/*
	 * Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	 * use the contents of szFile to initialize itself.
	 */
	ofn.lpstrFile = result;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(result);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0"; /* fixme */
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	/* Display the Open dialog box. */
	if(GetOpenFileName(&ofn)) {
		lua_pushstring(L,result);
		return 1;
	}
	else {
		return 0;
	}
}

#else

int l_get_filename(lua_State* L)
{
    char buf[4*80];
    printf("Please enter a filename: ");
    fgets(buf,sizeof(buf),stdin);
    lua_pushstring(L,buf);
    return 1;
}
#endif

/* f should be the lua name, without quotes */
#define REGISTER_LUA_FUNC(L,f) \
lua_pushcfunction(L, l_ ## f); \
    lua_setglobal(L, #f); 

#define REGISTER_LUA_SETGET(L,var) \
REGISTER_LUA_FUNC(L, set_ ## var); \
    REGISTER_LUA_FUNC(L, get_ ## var);

#define TO_STRING(identifier) # identifier

#define SETGET_ENTRIES(var) \
{ TO_STRING(set_ ## var), l_set_ ## var }, \
{ TO_STRING(get_ ## var), l_get_ ## var } 


static const struct luaL_reg bmlib[] =
{
    {"reset_menu", l_reset_menu},
    {"add_menu_item", l_add_menu_item},
    {"system", l_system},
    {"warn", l_warn},
    {"get_filename", l_get_filename},
    {"run_process_in_background", l_run_process_in_background},
    {"get_gl_area_size", l_get_gl_area_size},
    /* {"http_get", l_http_get},  Use bm.system("wget...") instead. */
    {"set_idle_callback", l_set_idle_callback},
    {"os", l_os},
    {NULL, NULL}
};

int l_open_bmlib(lua_State* L)
{
    luaL_openlib(L, "bm", bmlib, 0);
    return 1;
}

int lua_bitlibopen (lua_State *L);

static const luaL_reg lualibs[] = {
    {"base", luaopen_base},
    {"table", luaopen_table},
    {"io", luaopen_io},
    {"string", luaopen_string},
    {"math", luaopen_math},
    {"debug", luaopen_debug},
    {"loadlib", luaopen_loadlib},

    {"bitlib",lua_bitlibopen},
    {"image", lua_openimage},
    {"mesh", luaopen_mesh},

    {"bm", l_open_bmlib},  /* brainmaps functions */
    {"gl", luaopen_gl}, 
    {"glu", luaopen_glu}, 
    {"glut", luaopen_glut}, 
    {NULL, NULL}
};

int file_exists(char filename[])
{
    FILE* file=fopen(filename,"r");
    if(file) { fclose(file); return 1; } else { return 0; }
}

/* based on http://www.lua.org/pil/24.1.html */
/* Global inputs: config_filename, init_filename */
/* Global outputs: lua_state */
void brainmaps_start_lua(void)
{
    int i;
    lua_State* L;
    const luaL_reg* lib;

    L=lua_open();
    if(L==NULL) {
        b3d_fatal_error("Could not start Lua interpreter.");
    }

    /* Load libraries */
    for(lib=lualibs; lib->func!=NULL; lib++) {
        fprintf(stderr,"loading lua lib: %s\n",lib->name);
        lib->func(L); /* open library */
        lua_settop(L,0); /* discard any results */
    }

    l_reset_menu(L);

    /* Search up the directory tree for a while to find the init script. */
    for (i=0;i<10;i++) {
        if (file_exists(init_filename)) {
            if (luaL_loadfile(L, init_filename) || lua_pcall(L, 0, 0, 0)) {
                char msg[3*80];
                snprintf(msg,sizeof(msg),"The init file did not run: %s",lua_tostring(L,-1));
                b3d_fatal_error(msg);
            }
            else {
                /* The init file has successfully run.  Our search is over. */
                break;
            }
        }
        else {
            if(verbosity>=1) {
                fprintf(stderr,
                        "%s is not in the current directory.  Changing to parent directory.\n",init_filename);
            }
            chdir("..");
        }
    }

    lua_state=L;
}

