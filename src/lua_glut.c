#include "lua_glut.h"

/* GLUT initialization sub-API. */

/* Just pass the strings in argv as arguments, not the count. */
int l_glutInit(lua_State *L){
	int i;
	int argc = lua_gettop(L), saved_argc = lua_gettop(L);
	char **argv = (char **)calloc(argc+1, sizeof(char *));

	for(i = 0; i<argc; i++){
		argv[i] = strdup(luaL_checkstring(L, i+1));
	}
	glutInit(&argc, argv);

	for(i = 0; i<saved_argc; i++){
		free(argv[i]);
	}
	free(argv);

	return 0;
}

BIND_0_1(glutInitDisplayMode, integer)
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=9))
BIND_0_1(glutInitDisplayString, string)
#endif
BIND_0_2(glutInitWindowPosition, integer, integer)
BIND_0_2(glutInitWindowSize, integer, integer)
BIND_0_0(glutMainLoop)

/* GLUT window sub-API. */
BIND_1_1(number, glutCreateWindow, string)

int l_glutCreateSubWindow(lua_State *L){
	lua_pushnumber(L,
		glutCreateSubWindow(
			luaL_checkinteger(L, 1),
			luaL_checkinteger(L, 2),
			luaL_checkinteger(L, 3),
			luaL_checkinteger(L, 4),
			luaL_checkinteger(L, 5)
		)
	);

	return 1;
}

BIND_0_1(glutDestroyWindow, integer)
BIND_0_0(glutPostRedisplay)
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=11))
BIND_0_1(glutPostWindowRedisplay, integer)
#endif
BIND_0_0(glutSwapBuffers)
BIND_1_0(number, glutGetWindow)
BIND_0_1(glutSetWindow, integer)
BIND_0_1(glutSetWindowTitle, string)
BIND_0_1(glutSetIconTitle, string)
BIND_0_2(glutPositionWindow, integer, integer)
BIND_0_2(glutReshapeWindow, integer, integer)
BIND_0_0(glutPopWindow)
BIND_0_0(glutPushWindow)
BIND_0_0(glutIconifyWindow)
BIND_0_0(glutShowWindow)
BIND_0_0(glutHideWindow)
#if (GLUT_API_VERSION>=3)
BIND_0_0(glutFullScreen)
BIND_0_1(glutSetCursor, integer)
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=9))
BIND_0_2(glutWarpPointer, integer, integer)
#endif

/* GLUT overlay sub-API. */
BIND_0_0(glutEstablishOverlay)
BIND_0_0(glutRemoveOverlay)
BIND_0_1(glutUseLayer, integer)
BIND_0_0(glutPostOverlayRedisplay)
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=11))
BIND_0_1(glutPostWindowOverlayRedisplay, integer)
#endif
BIND_0_0(glutShowOverlay)
BIND_0_0(glutHideOverlay)
#endif

/* GLUT menu sub-API. */
BIND_0_1(glutDestroyMenu, integer)
BIND_1_0(number, glutGetMenu)
BIND_0_1(glutSetMenu, integer)
BIND_0_2(glutAddMenuEntry, string, integer)
BIND_0_2(glutAddSubMenu, string, integer)
BIND_0_3(glutChangeToMenuEntry, integer, string, integer)
BIND_0_3(glutChangeToSubMenu, integer, string, integer)
BIND_0_1(glutRemoveMenuItem, integer)
BIND_0_1(glutAttachMenu, integer)
BIND_0_1(glutDetachMenu, integer)

/* GLUT color index sub-API. */
BIND_0_4(glutSetColor, integer, number, number, number)
BIND_1_2(number, glutGetColor, integer, integer)
BIND_0_1(glutCopyColormap, integer)

/* GLUT state retrieval sub-API. */
BIND_1_1(number, glutGet, integer)
BIND_1_1(number, glutDeviceGet, integer)
#if (GLUT_API_VERSION>=2)
/* GLUT extension support sub-API */
BIND_1_1(number, glutExtensionSupported, string)
#endif
#if (GLUT_API_VERSION>=3)
BIND_1_0(number, glutGetModifiers)
BIND_1_1(number, glutLayerGet, integer)
#endif

/* GLUT font sub-API */
#define STRING_TO_FONT_ENTRY(s, font)	(strcmp(s, # font)==0)? font :

#define STRING_TO_FONT(L, s)	\
	STRING_TO_FONT_ENTRY(s, GLUT_STROKE_ROMAN)		\
	STRING_TO_FONT_ENTRY(s, GLUT_STROKE_MONO_ROMAN)		\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_9_BY_15)		\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_8_BY_13)		\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_TIMES_ROMAN_10)	\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_TIMES_ROMAN_24)	\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_HELVETICA_10)	\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_HELVETICA_12)	\
	STRING_TO_FONT_ENTRY(s, GLUT_BITMAP_HELVETICA_18)	\
	(strcmp(s, "9x15")==0)? GLUT_BITMAP_9_BY_15 :		\
	(strcmp(s, "8x13")==0)? GLUT_BITMAP_8_BY_13 :		\
	(strcmp(s, "t10")==0)?  GLUT_BITMAP_TIMES_ROMAN_10 :	\
	(strcmp(s, "t24")==0)?  GLUT_BITMAP_TIMES_ROMAN_24 :	\
	(strcmp(s, "h10")==0)?  GLUT_BITMAP_HELVETICA_10 :	\
	(strcmp(s, "h12")==0)?  GLUT_BITMAP_HELVETICA_12 :	\
	(strcmp(s, "h18")==0)?  GLUT_BITMAP_HELVETICA_18 :	\
	(luaL_error(L, "Unrecognized font: %s", s), /* ignored */GLUT_BITMAP_HELVETICA_18)

int l_glutBitmapCharacter(lua_State *L){
	const char *font_name = luaL_checkstring(L, 1);

	glutBitmapCharacter(STRING_TO_FONT(L, font_name), luaL_checkinteger(L, 2));

	return 0;
}

int l_glutBitmapWidth(lua_State *L){
	const char *font_name = luaL_checkstring(L, 1);
	const char *c = luaL_checkstring(L, 2);

	luaL_argcheck(L, strlen(c)==1, 2, "The second argument to glutBitmapWidth must be a string of length 1.");
	glutBitmapWidth(STRING_TO_FONT(L, font_name), (int)c[0]);

	return 0;
}

int l_glutStrokeCharacter(lua_State *L){
	const char *font_name = luaL_checkstring(L, 1);
	const char *c = luaL_checkstring(L, 2);

	luaL_argcheck(L, strlen(c)==1, 2, "The second argument to glutStrokeCharacter must be a string of length 1.");
	glutStrokeCharacter(STRING_TO_FONT(L, font_name), (int)c[0]);

	return 0;
}

int l_glutStrokeWidth(lua_State *L){
	const char *font_name = luaL_checkstring(L, 1);
	const char *c = luaL_checkstring(L, 2);

	luaL_argcheck(L, strlen(c)==1, 2, "The second argument to glutStrokeWidth must be a string of length 1.");
	glutStrokeWidth(STRING_TO_FONT(L, font_name), (int)c[0]);

	return 0;
}

/* GLUT pre-built models sub-API */
BIND_0_3(glutWireSphere, number, integer, integer)
BIND_0_3(glutSolidSphere, number, integer, integer)
BIND_0_4(glutWireCone, number, number, integer, integer)
BIND_0_4(glutSolidCone, number, number, integer, integer)
BIND_0_1(glutWireCube, number)
BIND_0_1(glutSolidCube, number)
BIND_0_4(glutWireTorus, number, number, integer, integer)
BIND_0_4(glutSolidTorus, number, number, integer, integer)
BIND_0_0(glutWireDodecahedron)
BIND_0_0(glutSolidDodecahedron)
BIND_0_1(glutWireTeapot, number)
BIND_0_1(glutSolidTeapot, number)
BIND_0_0(glutWireOctahedron)
BIND_0_0(glutSolidOctahedron)
BIND_0_0(glutWireTetrahedron)
BIND_0_0(glutSolidTetrahedron)
BIND_0_0(glutWireIcosahedron)
BIND_0_0(glutSolidIcosahedron)

#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=9))
/* GLUT video resize sub-API. */
BIND_1_1(number, glutVideoResizeGet, integer)
BIND_0_0(glutSetupVideoResizing)
BIND_0_0(glutStopVideoResizing)
BIND_0_4(glutVideoResize, integer, integer, integer, integer)
BIND_0_4(glutVideoPan, integer, integer, integer, integer)

/* GLUT debugging sub-API. */
BIND_0_0(glutReportErrors)
#endif

#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=13))
/* GLUT device control sub-API. */
/* glutSetKeyRepeat modes. */

/* Joystick button masks. */
BIND_0_1(glutIgnoreKeyRepeat, integer)
BIND_0_1(glutSetKeyRepeat, integer)
BIND_0_0(glutForceJoystickFunc)

/* GLUT game mode sub-API. */
/* glutGameModeGet. */
BIND_1_0(number, glutEnterGameMode)
BIND_0_0(glutLeaveGameMode)
BIND_1_1(number, glutGameModeGet, integer)
#endif

#define ENTRY(f)	{ # f, l_ ## f }

static const struct luaL_Reg glut_lib[] = {
	ENTRY(glutInit),
	ENTRY(glutInitDisplayMode),
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=9))
	ENTRY(glutInitDisplayString),
#endif
	ENTRY(glutInitWindowPosition),
	ENTRY(glutInitWindowSize),
	ENTRY(glutMainLoop),

/* GLUT window sub-API. */
	ENTRY(glutCreateWindow),
	ENTRY(glutCreateSubWindow),
	ENTRY(glutDestroyWindow),
	ENTRY(glutPostRedisplay),
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=11))
	ENTRY(glutPostWindowRedisplay),
#endif
	ENTRY(glutSwapBuffers),
	ENTRY(glutGetWindow),
	ENTRY(glutSetWindow),
	ENTRY(glutSetWindowTitle),
	ENTRY(glutSetIconTitle),
	ENTRY(glutPositionWindow),
	ENTRY(glutReshapeWindow),
	ENTRY(glutPopWindow),
	ENTRY(glutPushWindow),
	ENTRY(glutIconifyWindow),
	ENTRY(glutShowWindow),
	ENTRY(glutHideWindow),
#if (GLUT_API_VERSION>=3)
	ENTRY(glutFullScreen),
	ENTRY(glutSetCursor),
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=9))
	ENTRY(glutWarpPointer),
#endif

/* GLUT overlay sub-API. */
	ENTRY(glutEstablishOverlay),
	ENTRY(glutRemoveOverlay),
	ENTRY(glutUseLayer),
	ENTRY(glutPostOverlayRedisplay),
#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=11))
	ENTRY(glutPostWindowOverlayRedisplay),
#endif
	ENTRY(glutShowOverlay),
	ENTRY(glutHideOverlay),
#endif

/* GLUT menu sub-API. */
	ENTRY(glutDestroyMenu),
	ENTRY(glutGetMenu),
	ENTRY(glutSetMenu),
	ENTRY(glutAddMenuEntry),
	ENTRY(glutAddSubMenu),
	ENTRY(glutChangeToMenuEntry),
	ENTRY(glutChangeToSubMenu),
	ENTRY(glutRemoveMenuItem),
	ENTRY(glutAttachMenu),
	ENTRY(glutDetachMenu),

	ENTRY(glutSetColor),
	ENTRY(glutGetColor),
	ENTRY(glutCopyColormap),

	ENTRY(glutGet),
	ENTRY(glutDeviceGet),
#if (GLUT_API_VERSION>=2)
	ENTRY(glutExtensionSupported),
#endif
#if (GLUT_API_VERSION>=3)
	ENTRY(glutGetModifiers),
	ENTRY(glutLayerGet),
#endif

	ENTRY(glutBitmapCharacter),
	ENTRY(glutBitmapWidth),
	ENTRY(glutStrokeCharacter),
	ENTRY(glutStrokeWidth),

	ENTRY(glutWireSphere),
	ENTRY(glutSolidSphere),
	ENTRY(glutWireCone),
	ENTRY(glutSolidCone),
	ENTRY(glutWireCube),
	ENTRY(glutSolidCube),
	ENTRY(glutWireTorus),
	ENTRY(glutSolidTorus),
	ENTRY(glutWireDodecahedron),
	ENTRY(glutSolidDodecahedron),
	ENTRY(glutWireTeapot),
	ENTRY(glutSolidTeapot),
	ENTRY(glutWireOctahedron),
	ENTRY(glutSolidOctahedron),
	ENTRY(glutWireTetrahedron),
	ENTRY(glutSolidTetrahedron),
	ENTRY(glutWireIcosahedron),
	ENTRY(glutSolidIcosahedron),

#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=9))
	ENTRY(glutVideoResizeGet),
	ENTRY(glutSetupVideoResizing),
	ENTRY(glutStopVideoResizing),
	ENTRY(glutVideoResize),
	ENTRY(glutVideoPan),
	ENTRY(glutReportErrors),
#endif

#if ((GLUT_API_VERSION>=4)||(GLUT_XLIB_IMPLEMENTATION>=13))
	ENTRY(glutIgnoreKeyRepeat),
	ENTRY(glutSetKeyRepeat),
	ENTRY(glutForceJoystickFunc),

	ENTRY(glutEnterGameMode),
	ENTRY(glutLeaveGameMode),
	ENTRY(glutGameModeGet),
#endif
	{NULL, NULL},
};

int luaopen_glut(lua_State *L){
	luaL_newlib(L, glut_lib);

	return 1;
}
