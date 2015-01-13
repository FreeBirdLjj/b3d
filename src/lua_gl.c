/* This is a partial binding of OpenGL for Lua.
 * I started writing this before I knew about gllua and tolua.
 * gllua was written for Lua 3.1 and did not compile for me with Lua 5.0.2.
 * So I will stay with my own binding for the time being.  -ijt
 *
 * glVertex calls glVertex2d, glVertex3d, or glVertex4d depending on its
 * argument count.  The same sort of thing goes for glColor and glTexCoord.
 *
 * Many OpenGL functions take a pointer, to the beginning of an array, as
 * their final argument.  Many of the Lua wrappers here just take a variable
 * number of arguments, packing the last ones into an array to send to OpenGL.
 * This makes writing the binding easier, and hopefully will make many calls
 * to these functions a bit less cluttered.  In some cases this is
 * impractical, such as glDrawPixels().
 *
 * Some functions have been bound because they are important, others because
 * they were easy to bind.
 *
 * */


/* Here is the comment at the top of the Mesa GL header from which this file
 * is derived: */

/*
 * Mesa 3-D graphics library
 * Version:  6.3
 *
 * Copyright (C) 1999-2005  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "lua_gl.h"

/* untested. -ijt */
BIND_0_1(glBegin, integer);
BIND_0_0(glEnd)

int l_glVertex(lua_State *L){
	switch(lua_gettop(L)){
	case 2:
		glVertex2d(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
		break;
	case 3:
		glVertex3d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3)
		);
		break;
	case 4:
		glVertex4d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3),
			luaL_checknumber(L, 4)
		);
		break;
	default:
		luaL_error(L, "Wrong number of arguments to glVertex().  Need 2,3, or 4.");
		break;
	}

	return 0;
}

BIND_0_3(glNormal3d, number, number, number)

int l_glColor(lua_State *L){
	switch(lua_gettop(L)){
	case 3:
		glColor3d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3)
		);
		break;
	case 4:
		glColor4d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3),
			luaL_checknumber(L, 4)
		);
		break;
	default:
		luaL_error(L,"Wrong number of arguments to glColor().  Need 3 or 4.");
		break;
	}

	return 0;
}

int l_glRasterPos(lua_State *L){
	switch(lua_gettop(L)){
	case 2:
		glRasterPos2d(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
		break;
	case 3:
		glRasterPos3d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3)
		);
		break;
	case 4:
		glRasterPos4d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3),
			luaL_checknumber(L, 4)
		);
		break;
	default:
		luaL_error(L, "Wrong number of arguments to glRasterPos().  Need 2,3, or 4.");
		break;
	}

	return 0;
}

int l_glTexCoord(lua_State *L){
	switch(lua_gettop(L)){
	case 1:
		glTexCoord1d(luaL_checknumber(L, 1));
		break;
	case 2:
		glTexCoord2d(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
		break;
	case 3:
		glTexCoord3d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3)
		);
		break;
	case 4:
		glTexCoord4d(
			luaL_checknumber(L, 1),
			luaL_checknumber(L, 2),
			luaL_checknumber(L, 3),
			luaL_checknumber(L, 4)
		);
		break;
	default:
		luaL_error(L, "Wrong number of arguments to glTexCoord().  Need 1,2,3, or 4.");
		break;
	}

	return 0;
}

int l_glRect(lua_State *L){
	glRectd(
		luaL_checknumber(L, 1),
		luaL_checknumber(L, 2),
		luaL_checknumber(L, 3),
		luaL_checknumber(L, 4)
	);

	return 0;
}

int l_glMaterial(lua_State *L){
	GLenum pname = luaL_checkinteger(L, 2);
	switch(pname){
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_EMISSION:
	case GL_AMBIENT_AND_DIFFUSE:
		{
			GLfloat params[] = {
				luaL_checknumber(L, 3),
				luaL_checknumber(L, 4),
				luaL_checknumber(L, 5),
				luaL_checknumber(L, 6)
			};
			glMaterialfv(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), params);
		}
		break;
	case GL_COLOR_INDEXES:
		{
			GLfloat params[] = {
				luaL_checknumber(L, 3),
				luaL_checknumber(L, 4),
				luaL_checknumber(L, 5),
			};
			glMaterialfv(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), params);
		}
		break;
	case GL_SHININESS:
		glMaterialf(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checknumber(L, 3));
		break;
	}

	return 0;
}

BIND_0_1(glEnable,  integer);
BIND_0_1(glDisable, integer);

/*
 * Transformation
 */

BIND_0_1(glMatrixMode, integer);

BIND_0_6(glOrtho, number, number, number, number, number, number)

BIND_0_6(glFrustum, number, number, number, number, number, number)

BIND_0_4(glViewport, integer, integer, integer, integer)

BIND_0_0(glPushMatrix)
BIND_0_0(glPopMatrix)
BIND_0_0(glLoadIdentity)
BIND_0_4(glRotated, number, number, number, number)
BIND_0_3(glScaled, number, number, number)
BIND_0_3(glTranslated, number, number, number)

static void make_gl_matrix_from_lua_stack(lua_State *L, GLdouble m[16]){
	int i;

	for(i = 0; i<16; i++){
		m[i] = luaL_checknumber(L, i+1);
	}
}

int l_glLoadMatrix(lua_State *L){
	GLdouble m[16];

	make_gl_matrix_from_lua_stack(L, m);
	glLoadMatrixd(m);

	return 0;
}

int l_glMultMatrix(lua_State *L){
	GLdouble m[16];

	make_gl_matrix_from_lua_stack(L, m);
	glMultMatrixd(m);

	return 0;
}

#define ARRAYSIZE_REF	2					/* array sizes */

/* convert a stack index to positive */
#define abs_index(L, i)	((((i)>0)||((i)<=LUA_REGISTRYINDEX))? (i) : lua_gettop(L)+(i)+1)

static int checkint (lua_State *L, int topop) {
	int n = (int)lua_tonumber(L, -1);

	if((n==0)&&(!lua_isnumber(L, -1))){
		n = -1;
	}
	lua_pop(L, topop);

	return n;
}

static void getsizes(lua_State *L){
	lua_rawgeti(L, LUA_REGISTRYINDEX, ARRAYSIZE_REF);
	if (lua_isnil(L, -1)){					/* no `size' table? */
		lua_pop(L, 1);					/* remove nil */
		lua_newtable(L);				/* create it */
		lua_pushvalue(L, -1);				/* `size' will be its own metatable */
		lua_setmetatable(L, -2);
		lua_pushliteral(L, "__mode");
		lua_pushliteral(L, "k");
		lua_rawset(L, -3);				/* metatable(N).__mode = "k" */
		lua_pushvalue(L, -1);
		lua_rawseti(L, LUA_REGISTRYINDEX, ARRAYSIZE_REF);	/* store in register */
	}
}

int luaL_getn(lua_State *L, int t){
	int n;

	t = abs_index(L, t);
	lua_pushliteral(L, "n");				/* try t.n */
	lua_rawget(L, t);
	n = checkint(L, 1);
	if(n>=0){
		return n;
	}
	getsizes(L);						/* else try sizes[t] */
	lua_pushvalue(L, t);
	lua_rawget(L, -2);
	if((n = checkint(L, 2))>=0){
		return n;
	}
	for(n = 1; 1; n++){					/* else must count elements */
		lua_rawgeti(L, t, n);
		if(lua_isnil(L, -1)){
			break;
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	return n-1;
}

GLfloat *float_array_from_table_in_final_arg(lua_State *L, int *n){
	int i;

	*n = luaL_getn(L, -1);
	GLfloat *ret = (GLfloat *)malloc((*n)*sizeof(ret[0]));
	for(i = 0; i<*n; i++){
		lua_pushnumber(L, i+1);
		lua_gettable(L, -2);
		ret[i] = luaL_checknumber(L, -1);
		lua_pop(L, 1);
	}

	return ret;
}

/* Need to test this... -ijt */
int l_glInterleavedArrays_n3f_v3f(lua_State *L){
	int i, n = luaL_getn(L, 2);

	if(n%6){
		luaL_error(L, "Bad array size for glInterleavedArrays_n3f_v3f(): expected a multiple of six.");
	}

	GLfloat *data = malloc(n*sizeof(GLfloat));
	for(i = 0; i<n; i++){
		lua_pushnumber(L, i+1);
		lua_gettable(L, 2);
		data[i] = luaL_checknumber(L, 3);
		lua_pop(L, 1);
	}

	glInterleavedArrays(GL_N3F_V3F, luaL_checkinteger(L, 1), data);

	free(data);

	return 0;
}

BIND_0_3(glDrawArrays, integer, integer, integer)

/*
 * Miscellaneous
 */

BIND_0_1(glClear, integer)
BIND_0_1(glIndexMask, integer)
BIND_0_1(glLogicOp, integer)
BIND_0_1(glCullFace, integer)
BIND_0_1(glFrontFace, integer)
BIND_0_1(glDrawBuffer, integer)
BIND_0_1(glReadBuffer, integer)

int l_glEdgeFlag(lua_State *L){
	glEdgeFlag(lua_toboolean(L, 1));

	return 0;
}

BIND_0_1(glClearIndex, number)
BIND_0_1(glPointSize, number)
BIND_0_1(glLineWidth, number)

BIND_0_4(glClearColor, number, number, number, number);
BIND_0_4(glColorMask, number, number, number, number);
BIND_0_2(glAlphaFunc, integer, number);
BIND_0_2(glBlendFunc, integer, integer);
BIND_0_2(glLineStipple, integer, integer);
BIND_0_2(glPolygonMode, integer, integer);
BIND_0_2(glPolygonOffset, number, number);

BIND_0_4(glScissor, integer, integer, integer, integer);

int l_glClipPlane(lua_State *L){
	GLdouble coeffs[4];
	int i;

	for(i = 0; i<3; i++){
		coeffs[0] = luaL_checknumber(L, i+2);
	}
	glClipPlane(luaL_checkinteger(L, 1), coeffs);

	return 0;
}

int l_glGetClipPlane(lua_State *L){
	int i;
	GLdouble coeffs[4];

	glGetClipPlane(luaL_checkinteger(L, 1), coeffs);

	for(i = 0; i<4; i++){
		lua_pushnumber(L, coeffs[i]);
	}

	return 4;
}

BIND_1_1(number, glIsEnabled, integer)

int l_glGetDoublev(lua_State *L){
	GLdouble params[32];
	int i;

	for(i = 0; i<32; i++){
		params[i] = DBL_MAX;
	}
	glGetDoublev(luaL_checkinteger(L, 1), params);

	for(i = 0; (i<32)&&(params[i]!=DBL_MAX); i++){
		lua_pushnumber(L, params[i]);
	}

	return i;
}

int l_glGetBooleanv(lua_State *L){
	GLboolean params[32];
	int i;

	for(i = 0; i<32; i++){
		params[i] = ~0;
	}
	glGetBooleanv(luaL_checkinteger(L, 1), params);

	for(i = 0; (i<32)&&(params[i]!=(~0)); i++){
		lua_pushnumber(L, params[i]);
	}

	return i;
}

BIND_0_1(glPushAttrib, integer)
BIND_0_0(glPopAttrib)

BIND_0_1(glPushClientAttrib, integer)
BIND_0_0(glPopClientAttrib)

BIND_1_1(number, glRenderMode, integer)

int l_glGetError(lua_State *L){
	lua_pushnumber(L, glGetError());

	return 1;
}

int l_glGetString(lua_State *L){
	lua_pushstring(L, (char const *)glGetString(luaL_checkinteger(L, 1)));

	return 1;
}

BIND_0_0(glFinish)
BIND_0_0(glFlush)
BIND_0_2(glHint, integer, integer)

/*
 * Depth Buffer
 */

BIND_0_1(glClearDepth, integer)
BIND_0_1(glDepthFunc, integer)

int l_glDepthMask(lua_State *L){
	luaL_checktype(L, 1, LUA_TBOOLEAN);
	glDepthMask(lua_toboolean(L, 1));

	return 0;
}

BIND_0_2(glDepthRange, integer, integer);

/*
 * Accumulation Buffer
 */

BIND_0_4(glClearAccum, number, number, number, number);
BIND_0_2(glAccum, integer, number);

/*
 * Display Lists
 */

BIND_1_1(number, glIsList, integer);
BIND_0_2(glDeleteLists, integer, integer);
BIND_1_1(number, glGenLists, integer);
BIND_0_2(glNewList, integer, integer);
BIND_0_0(glEndList);
BIND_0_1(glCallList, integer);
BIND_0_1(glListBase, integer);

/*
 * Lighting
 */

BIND_0_1(glShadeModel, integer);
BIND_0_3(glLightf, integer, integer, number);
BIND_0_2(glLightModelf, integer, number);
BIND_0_2(glColorMaterial, integer, integer);

int l_glLightModelfv(lua_State *L){
	int i;
	GLfloat params[4];

	for(i = 0; i<4; i++)
		params[i] = luaL_checknumber(L, i+2);
	glLightModelfv(luaL_checkinteger(L, 1), params);

	return 0;
}

int l_glLightv(lua_State *L){
	int i;
	GLfloat params[4];

	for(i = 0; i<4; i++)
		params[i] = luaL_checknumber(L, i+3);
	glLightfv(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), params);

	return 0;
}

/*
 * Raster functions
 */

BIND_0_2(glPixelZoom, number, number)
BIND_0_2(glPixelStoref, integer, number)
BIND_0_2(glPixelStorei, integer, number)

BIND_0_2(glPixelTransferf, integer, number)
BIND_0_2(glPixelTransferi, integer, integer)

/* The pixels are returned on the Lua stack, not in a table. */
int l_glReadPixels(lua_State *L){
	GLsizei w = luaL_checkinteger(L, 3), h = luaL_checkinteger(L, 4);
	GLfloat *pixels = malloc(w*h*sizeof(GLfloat));
	int i;

	glReadPixels(
		luaL_checkinteger(L, 1),
		luaL_checkinteger(L, 2),
		w, h,
		luaL_checkinteger(L, 5),
		luaL_checkinteger(L, 6),
		pixels
	);

	for(i = 0; i<w*h; i++){
		lua_pushnumber(L, pixels[i]);
	}

	free(pixels);

	return w*h;
}

int l_glDrawPixels(lua_State *L){
	int n;
	GLfloat *pixels = float_array_from_table_in_final_arg(L, &n);

	luaL_argcheck(L, luaL_checkinteger(L, 4)==GL_FLOAT, 4, "Only GL_FLOAT is currently supported for the type in glDrawPixels.");
	glDrawPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), pixels);

	free(pixels);

	return 0;
}

BIND_0_5(glCopyPixels, integer, integer, integer, integer, integer)

/*
 * Stenciling
 */

BIND_0_3(glStencilFunc, integer, integer, integer)
BIND_0_1(glStencilMask, integer)
BIND_0_3(glStencilOp, integer, integer, integer)
BIND_0_1(glClearStencil, integer)


/*
 * Texture mapping
 */

int l_glTexGen(lua_State *L){
	int i, nparams = lua_gettop(L)-2;

	if(nparams<1){
		luaL_error(L, "Too few params passed to glTexGen");
	}
	GLdouble *params = calloc(nparams, sizeof(GLdouble));
	for(i = 0; i<nparams; i++){
		params[i] = luaL_checknumber(L, i+3);
	}
	glTexGendv(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), params);

	free(params);

	return 0;
}

BIND_0_3(glTexEnvf, integer, integer, number)
BIND_0_3(glTexEnvi, integer, integer, integer)

BIND_0_3(glTexParameterf, integer, integer, number)
BIND_0_3(glTexParameteri, integer, integer, integer)

int l_gluBuild1DMipmaps(lua_State *L){
	int n;
	GLfloat *pixels = float_array_from_table_in_final_arg(L, &n);

	luaL_argcheck(L, luaL_checkinteger(L, 5)==GL_FLOAT, 5, "The type argument of glTexImage1D should be GL_FLOAT, since it is the only\ntype that is currently supported.\n");
	gluBuild1DMipmaps(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), pixels);

	free(pixels);

	return 0;
}

int l_glTexImage1D(lua_State *L){
	int n;
	GLfloat *pixels = float_array_from_table_in_final_arg(L, &n);

	luaL_argcheck(L, luaL_checkinteger(L, 7)==GL_FLOAT, 7, "The type argument of glTexImage1D should be GL_FLOAT, since it is the only\ntype that is currently supported.\n");
	glTexImage1D(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6), luaL_checkinteger(L, 7), pixels);

	free(pixels);

	return 0;
}

int l_glTexImage2D(lua_State *L){
	int n;
	GLfloat *pixels = float_array_from_table_in_final_arg(L, &n);

	luaL_argcheck(L, luaL_checkinteger(L, 8)==GL_FLOAT, 8, "The type argument of glTexImage2D should be GL_FLOAT, since it is the only\ntype that is currently supported.\n");
	glTexImage2D(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), luaL_checkinteger(L, 6), luaL_checkinteger(L, 7), luaL_checkinteger(L, 8), pixels);

	free(pixels);

	return 0;
}


/* 1.1 functions */

/* This wrapper is a bit different from the function it wraps.  For example:
 *
 * t = glGenTextures(1)
 * t1,t2 = glGenTextures(2)
 *
 */
int l_glGenTextures(lua_State *L){
	int i;
	GLsizei n = luaL_checkinteger(L, 1);
	GLuint *textures = calloc(n, sizeof(textures[0]));

	glGenTextures(n, textures);

	for(i = 0; i<n; i++){
		lua_pushnumber(L, textures[i]);
	}

	free(textures);

	return n;
}

/* This wrapper just takes a variable number of texture arguments.
 * The number of textures must not be included in the argument list,
 * unlike the C version of glDeleteTextures.
 */
int l_glDeleteTextures(lua_State *L){
	int i;
	int n = lua_gettop(L);
	GLuint *textures = calloc(n, sizeof(textures[0]));

	for(i = 1; i<=n; i++){
		textures[i-1] = luaL_checkinteger(L, i);
	}

	glDeleteTextures(n, textures);

	return 0;
}

BIND_0_2(glBindTexture, integer, integer)

BIND_1_1(number, glIsTexture, integer)

BIND_0_7(glCopyTexImage1D, integer, integer, integer, integer, integer, integer, integer)
BIND_0_8(glCopyTexImage2D, integer, integer, integer, integer, integer, integer, integer, integer)
BIND_0_6(glCopyTexSubImage1D, integer, integer, integer, integer, integer, integer)
BIND_0_8(glCopyTexSubImage2D, integer, integer, integer, integer, integer, integer, integer, integer)


/*
 * Fog
 */

BIND_0_2(glFogf, integer, number)
BIND_0_2(glFogi, integer, integer)

int l_glFogfv(lua_State *L){
	int n;
	GLfloat *params = float_array_from_table_in_final_arg(L, &n);

	luaL_argcheck(L, lua_tonumber(L, 1)==GL_FOG_COLOR, 1, "The first argument to glFogfv should be GL_FOG_COLOR.");
	luaL_argcheck(L, n==4, 2, "There should be five arguments to glFogfv, the last four being color components.");
	glFogfv(luaL_checkinteger(L, 1), params);

	free(params);

	return 0;
}

/*
 * OpenGL 1.2
 */

/*
 * Selection and Feedback
 */

BIND_0_1(glPassThrough, number)
BIND_0_0(glInitNames)
BIND_0_1(glLoadName, integer)
BIND_0_1(glPushName, integer)
BIND_0_0(glPopName)


/*
 * OpenGL 1.3
 */

#define ENTRY(f)	{# f, l_ ## f}

static const struct luaL_Reg gl_lib[] = {
	ENTRY(glBegin),
	ENTRY(glEnd),
	ENTRY(glNormal3d),
	ENTRY(glEnable),
	ENTRY(glDisable),
	ENTRY(glMatrixMode),
	ENTRY(glOrtho),
	ENTRY(glFrustum),
	ENTRY(glViewport),
	ENTRY(glPushMatrix),
	ENTRY(glPopMatrix),
	ENTRY(glLoadIdentity),
	ENTRY(glRotated),
	ENTRY(glScaled),
	ENTRY(glTranslated),
	ENTRY(glDrawArrays),
	ENTRY(glClear),
	ENTRY(glIndexMask),
	ENTRY(glLogicOp),
	ENTRY(glCullFace),
	ENTRY(glFrontFace),
	ENTRY(glDrawBuffer),
	ENTRY(glReadBuffer),
	ENTRY(glClearIndex),
	ENTRY(glPointSize),
	ENTRY(glLineWidth),
	ENTRY(glClearColor),
	ENTRY(glColorMask),
	ENTRY(glAlphaFunc),
	ENTRY(glBlendFunc),
	ENTRY(glLineStipple),
	ENTRY(glPolygonMode),
	ENTRY(glPolygonOffset),
	ENTRY(glScissor),
	ENTRY(glIsEnabled),
	ENTRY(glGetDoublev),
	ENTRY(glGetBooleanv),
	ENTRY(glPushAttrib),
	ENTRY(glPopAttrib),
	ENTRY(glPushClientAttrib),
	ENTRY(glPopClientAttrib),
	ENTRY(glRenderMode),
	ENTRY(glFinish),
	ENTRY(glFlush),
	ENTRY(glHint),
	ENTRY(glClearDepth),
	ENTRY(glDepthFunc),
	ENTRY(glDepthRange),
	ENTRY(glClearAccum),
	ENTRY(glAccum),
	ENTRY(glIsList),
	ENTRY(glDeleteLists),
	ENTRY(glGenLists),
	ENTRY(glNewList),
	ENTRY(glEndList),
	ENTRY(glCallList),
	ENTRY(glListBase),
	ENTRY(glShadeModel),
	ENTRY(glLightf),
	ENTRY(glLightModelf),
	ENTRY(glColorMaterial),
	ENTRY(glPixelZoom),
	ENTRY(glPixelStoref),
	ENTRY(glPixelStorei),
	ENTRY(glPixelTransferf),
	ENTRY(glPixelTransferi),
	ENTRY(glCopyPixels),
	ENTRY(glStencilFunc),
	ENTRY(glStencilMask),
	ENTRY(glStencilOp),
	ENTRY(glClearStencil),
	ENTRY(glTexGen),
	ENTRY(glTexEnvf),
	ENTRY(glTexEnvi),
	ENTRY(glTexParameterf),
	ENTRY(glTexParameteri),
	ENTRY(glBindTexture),
	ENTRY(glIsTexture),
	ENTRY(glCopyTexImage1D),
	ENTRY(glCopyTexImage2D),
	ENTRY(glCopyTexSubImage1D),
	ENTRY(glCopyTexSubImage2D),
	ENTRY(glFogf),
	ENTRY(glFogi),
	ENTRY(glVertex),
	ENTRY(glColor),
	ENTRY(glRasterPos),
	ENTRY(glTexCoord),
	ENTRY(glRect),
	ENTRY(glMaterial),
	ENTRY(glLoadMatrix),
	ENTRY(glMultMatrix),
	ENTRY(glInterleavedArrays_n3f_v3f),
	ENTRY(glEdgeFlag),
	ENTRY(glClipPlane),
	ENTRY(glGetClipPlane),
	ENTRY(glGetError),
	ENTRY(glGetString),
	ENTRY(glDepthMask),
	ENTRY(glLightModelfv),
	ENTRY(glLightv),
	ENTRY(glReadPixels),
	ENTRY(glDrawPixels),
	ENTRY(gluBuild1DMipmaps),
	ENTRY(glTexImage1D),
	ENTRY(glTexImage2D),
	ENTRY(glGenTextures),
	ENTRY(glDeleteTextures),
	ENTRY(glFogfv),
	{NULL, NULL},
};


int luaopen_gl(lua_State *L){
	luaL_newlib(L, gl_lib);
	return 1;
}

