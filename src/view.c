#include "globals.h"
#include "utils.h"
#include "view.h"
#include "my_lua.h"

#ifdef DEBUG
#define FPS
#endif

#ifdef FPS
int fps = 0;

void ShowFPS(int value){
	fprintf(stderr, "FPS: %d\n", fps);
	fps = 0;
	glutTimerFunc(1000, ShowFPS, 0);
}
#endif

void display(void){
	/* Do the Lua display callback `on_display' if it is defined. */
	lua_getglobal(lua_state, "on_display");
	if(lua_isfunction(lua_state, -1)){
		if(lua_pcall(lua_state, 0, 0, 0)){ 
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua display callback (on_display) failed: %s.  Removing the display callback.\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
			lua_pushnil(lua_state);
			lua_setglobal(lua_state, "on_display");
		}
	}
	else
		lua_pop(lua_state, 1);
#ifdef FPS
	fps++;
#endif
}

void mouse(int button, int state, int xi, int yi){
	lua_getglobal(lua_state, "on_mouse");
	if(lua_isfunction(lua_state, -1)){
		lua_pushnumber(lua_state, button);
		lua_pushnumber(lua_state, state);
		lua_pushnumber(lua_state, xi);
		lua_pushnumber(lua_state, yi);
		if(lua_pcall(lua_state, 4, 0, 0)){ 
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua mouse callback failed: %s\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
		}
	}
	else
		lua_pop(lua_state, 1);
}

void motion(int xi, int yi){
	lua_getglobal(lua_state, "on_motion");
	if(lua_isfunction(lua_state, -1)){
		lua_pushnumber(lua_state, xi);
		lua_pushnumber(lua_state, yi);
		if(lua_pcall(lua_state, 2, 0, 0)){ 
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua motion callback failed: %s\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
		}
	}
	else
		lua_pop(lua_state, 1);
}

void passive_motion(int xi, int yi){
	lua_getglobal(lua_state, "on_passive_motion");
	if(lua_isfunction(lua_state, -1)){
		lua_pushnumber(lua_state, xi);
		lua_pushnumber(lua_state, yi);
		if(lua_pcall(lua_state, 2, 0, 0)){
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua passive motion callback failed: %s\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
		}
	}
	else
		lua_pop(lua_state, 1);
}

void idle(void){
	/* Note: Moving this usleep call to the bottom of the function makes the
	 * window go blank if on_idle is undefined in lua.  
	 * */
	lua_getglobal(lua_state, "on_idle");
	if(lua_isfunction(lua_state, -1)){
		if(lua_pcall(lua_state, 0, 0, 0)){ 
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua idle callback failed: %s\nIdling is now turned off.\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
			glutIdleFunc(NULL);
		}
		glutPostRedisplay();
	}
	else
		lua_pop(lua_state, 1);
}

void keyboard(unsigned char key, int xi, int yi){
	lua_getglobal(lua_state, "on_keyboard");
	if(lua_isfunction(lua_state, -1)){
		lua_pushnumber(lua_state, key);
		lua_pushnumber(lua_state, xi);
		lua_pushnumber(lua_state, yi);
		if(lua_pcall(lua_state, 3, 0, 0)){
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua keyboard callback failed: %s\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
		}
	}
	else
		lua_pop(lua_state, 1);
}

void keyboard_up(unsigned char key, int xi, int yi){
	lua_getglobal(lua_state, "on_keyboard_up");
	if(lua_isfunction(lua_state, -1)){
		lua_pushnumber(lua_state, key);
		lua_pushnumber(lua_state, xi);
		lua_pushnumber(lua_state, yi);
		if(lua_pcall(lua_state, 3, 0, 0)){
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua keyboard-up callback failed: %s\n", lua_tostring(lua_state, -1));
			fprintf(stderr, "Warning: %s\n", msg);
		}
	}
	else
		lua_pop(lua_state, 1);
}

void reshape(int w, int h){
	glViewport(0, 0, max(w, 1), max(h, 1));
}

void menu_callback(int option){
	lua_getglobal(lua_state, "menu_callbacks");
	lua_pushnumber(lua_state, option);
	lua_gettable(lua_state, -2);
	if(lua_isfunction(lua_state, -1)){
		if(lua_pcall(lua_state, /*nargs*/ 0, /*nresults*/ 0, /*err_func*/ 0)){
			char msg[3*80];
			snprintf(msg, sizeof(msg), "Lua menu callback failed: %s\n", lua_tostring(lua_state,-1));
			fprintf(stderr, "Warning: %s\n", msg);
		}
		glutPostRedisplay();
	}
	else{
		if(!lua_isnil(lua_state, -1))
			fprintf(stderr, "Warning: menu callback is neither a function nor nil.\n");
		lua_pop(lua_state, 1);
	}
}

void view(char *title){
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutCreateWindow(title);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passive_motion);
	glutReshapeFunc(reshape);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	/* Initialize blending */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	brainmaps_start_lua();
	glutDisplayFunc(display);
#ifdef FPS
	glutTimerFunc(1000, ShowFPS, 0);
#endif
	glutMainLoop();
}


int l_set_idle_callback(lua_State *L){
	if(lua_isnil(L, 1))
		glutIdleFunc(NULL);
	else{
		luaL_checktype(L, 1, LUA_TFUNCTION);
		lua_setglobal(L, "on_idle");
		glutIdleFunc(idle);
	}
	return 0;
}

