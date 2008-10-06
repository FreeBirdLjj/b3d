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




#include "my_opengl.h"
#include "my_lua.h"
#include "lua_bind.h"

#include <float.h>
#include <stdlib.h>

/* untested. -ijt */
GLfloat* float_array_from_table_in_final_arg(lua_State* L, int *n)
{
    int i;
    GLfloat* ret;
    *n=luaL_getn(L,-1);
    ret=malloc(*n * sizeof(ret[0])); 
    for(i=0;i< *n;i++) {
        lua_pushnumber(L,i+1);
        lua_gettable(L,-2);
        ret[i]=luaL_checknumber(L,-1);
        lua_pop(L,1);
    }
    return ret;
}


BIND_0_1(glBegin,int);
BIND_0_0(glEnd)

int l_glVertex(lua_State* L) 
{ 
    switch(lua_gettop(L)) {
        case 2: glVertex2d(luaL_checknumber(L,1), luaL_checknumber(L,2)); break;
        case 3: 
            glVertex3d(
                    luaL_checknumber(L,1),
                    luaL_checknumber(L,2),
                    luaL_checknumber(L,3)); 
            break;
        case 4: 
            glVertex4d(
                luaL_checknumber(L,1),
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                luaL_checknumber(L,4)); 
            break;
        default:
            luaL_error(L,
                    "Wrong number of arguments to glVertex().  Need 2,3, or 4.");
            break;
    }
    return 0; 
}

BIND_0_3(glNormal3d,number,number,number)

int l_glColor(lua_State* L) 
{ 
    switch(lua_gettop(L)) {
        case 3: 
            glColor3d(
                luaL_checknumber(L,1),
                luaL_checknumber(L,2),
                luaL_checknumber(L,3)); 
            break;
        case 4:
            glColor4d(
                luaL_checknumber(L,1),
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                luaL_checknumber(L,4)); 
            break;
        default:
            luaL_error(L,"Wrong number of arguments to glColor().  Need 3 or 4.");
            break;
    }
    return 0; 
}

int l_glRasterPos(lua_State* L) 
{ 
    switch(lua_gettop(L)) {
        case 2: glRasterPos2d(luaL_checknumber(L,1), luaL_checknumber(L,2)); break;
        case 3: 
            glRasterPos3d(
                    luaL_checknumber(L,1),
                    luaL_checknumber(L,2),
                    luaL_checknumber(L,3)); 
            break;
        case 4: 
            glRasterPos4d(
                luaL_checknumber(L,1),
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                luaL_checknumber(L,4)); 
            break;
        default:
            luaL_error(L,
                    "Wrong number of arguments to glRasterPos().  Need 2,3, or 4.");
            break;
    }
    return 0; 
}

int l_glTexCoord(lua_State* L) 
{ 
    switch(lua_gettop(L)) {
        case 1: glTexCoord1d(luaL_checknumber(L,1)); break;
        case 2: glTexCoord2d(luaL_checknumber(L,1), luaL_checknumber(L,2)); break;
        case 3: 
            glTexCoord3d(
                    luaL_checknumber(L,1),
                    luaL_checknumber(L,2),
                    luaL_checknumber(L,3)); 
            break;
        case 4: 
            glTexCoord4d(
                luaL_checknumber(L,1),
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                luaL_checknumber(L,4)); 
            break;
        default:
            luaL_error(L,
"Wrong number of arguments to glTexCoord().  Need 1,2,3, or 4.");
            break;
    }
    return 0;
}

int l_glRect(lua_State* L) 
{ 
    glRectd(luaL_checknumber(L,1), luaL_checknumber(L,2), 
            luaL_checknumber(L,3), luaL_checknumber(L,4)); 
    return 0; 
}

int l_glMaterial(lua_State* L) 
{
    GLenum pname = luaL_checkint(L,2);
    switch(pname) {
        case GL_AMBIENT:
        case GL_DIFFUSE:
        case GL_SPECULAR:
        case GL_EMISSION:
        case GL_AMBIENT_AND_DIFFUSE:
            {
                GLfloat params[4];
                params[0] = luaL_checknumber(L,3);
                params[1] = luaL_checknumber(L,4);
                params[2] = luaL_checknumber(L,5);
                params[3] = luaL_checknumber(L,6);
                glMaterialfv(luaL_checkint(L,1), luaL_checkint(L,2), params);
            }
            break;

        case GL_COLOR_INDEXES:
            {
                GLfloat params[3];
                params[0] = luaL_checknumber(L,3);
                params[1] = luaL_checknumber(L,4);
                params[2] = luaL_checknumber(L,5);
                glMaterialfv(luaL_checkint(L,1), luaL_checkint(L,2), params);
            }
            break;

        case GL_SHININESS:
            glMaterialf(luaL_checkint(L,1), luaL_checkint(L,2), luaL_checknumber(L,3));
            break;

    }

    return 0;
}

BIND_0_1(glEnable,int);
BIND_0_1(glDisable,int);

/*
 * Transformation
 */

BIND_0_1(glMatrixMode,int);

BIND_0_6(glOrtho,number,number,number,
                 number,number,number)

BIND_0_6(glFrustum,number,number,number,
                   number,number,number)

BIND_0_4(glViewport,int,int,int,int)

BIND_0_0(glPushMatrix)
BIND_0_0(glPopMatrix)
BIND_0_0(glLoadIdentity)
BIND_0_4(glRotated,number,number,number,number)
BIND_0_3(glScaled,number,number,number)
BIND_0_3(glTranslated,number,number,number)

static void make_gl_matrix_from_lua_stack(lua_State* L, GLdouble m[16])
{
    int i;
    for(i=0;i<16;i++) { m[i]=luaL_checknumber(L,i+1); }
}

int l_glLoadMatrix(lua_State* L)
{
    GLdouble m[16];
    make_gl_matrix_from_lua_stack(L,m);
    glLoadMatrixd(m);
    return 0;
}

int l_glMultMatrix(lua_State* L)
{
    GLdouble m[16];
    make_gl_matrix_from_lua_stack(L,m);
    glMultMatrixd(m);
    return 0;
}

/* Need to test this... -ijt */
int l_glInterleavedArrays_n3f_v3f(lua_State* L)
{
    int i, n=luaL_getn(L,2);
    GLfloat* data;
    
    if(n%6!=0) {
        luaL_error(L,
"Bad array size for glInterleavedArrays_n3f_v3f(): expected a multiple of six.");
    }

    data = malloc(n*sizeof(GLfloat));
    for(i=0;i<n;i++) { 
        lua_pushnumber(L,i+1);
        lua_gettable(L,2);
        data[i] = luaL_checknumber(L,3);
        lua_pop(L,1);
    }

    glInterleavedArrays(GL_N3F_V3F, luaL_checkint(L,1), data);

    free(data);
    return 0;
}

#if 0
/* Uncomment this once the first one is tested. -ijt */

int l_glInterleavedArrays_t2f_n3f_v3f(lua_State* L)
{
    int i, n=luaL_getn(L,2);
    GLfloat* data;
    
    if(n%8!=0) {
        luaL_error(L,
"Bad array size for glInterleavedArrays_t2f_n3f_v3f(): expected a multiple of eight.");
    }

    data = malloc(n*sizeof(GLfloat));
    for(i=0;i<n;i++) { 
        lua_pushnumber(L,i+1);
        lua_gettable(L,2);
        data[i] = luaL_checknumber(L,3);
        lua_pop(L,1);
    }

    glInterleavedArrays(GL_T2F_N3F_V3F, luaL_checkint(L,1), data);

    free(data);
    return 0;
}
#endif


BIND_0_3(glDrawArrays,int,int,int)

/*
 * Miscellaneous
 */

BIND_0_1(glClear,int)
BIND_0_1(glIndexMask,int)
BIND_0_1(glLogicOp,int)
BIND_0_1(glCullFace,int)
BIND_0_1(glFrontFace,int)
BIND_0_1(glDrawBuffer,int)
BIND_0_1(glReadBuffer,int)
int l_glEdgeFlag(lua_State* L) { glEdgeFlag(lua_toboolean(L,1)); return 0; }

BIND_0_1(glClearIndex,number)
BIND_0_1(glPointSize,number)
BIND_0_1(glLineWidth,number)

BIND_0_4(glClearColor,number,number,number,number);
BIND_0_4(glColorMask,number,number,number,number);
BIND_0_2(glAlphaFunc,int,number);
BIND_0_2(glBlendFunc,int,int);
BIND_0_2(glLineStipple,int,int);
BIND_0_2(glPolygonMode,int,int);
BIND_0_2(glPolygonOffset,number,number);

#if 0
int l_glPolygonStipple(lua_State* L) 
int l_glGetPolygonStipple(lua_State* L) 
#endif

BIND_0_4(glScissor,int,int,int,int);

int l_glClipPlane(lua_State* L) 
{
    GLdouble coeffs[4];
    coeffs[0]=luaL_checknumber(L,2);
    coeffs[1]=luaL_checknumber(L,3);
    coeffs[2]=luaL_checknumber(L,4);
    glClipPlane(luaL_checkint(L,1),coeffs);
    return 0;
}

int l_glGetClipPlane(lua_State* L) 
{
    GLdouble coeffs[4];
    glGetClipPlane(luaL_checkint(L,1),coeffs);
    lua_pushnumber(L,coeffs[0]);
    lua_pushnumber(L,coeffs[1]);
    lua_pushnumber(L,coeffs[2]);
    lua_pushnumber(L,coeffs[3]);
    return 4;
}

BIND_1_1(number,glIsEnabled,int)

#if 0
Maybe do these later... -ijt

void glGetBooleanv( GLenum pname, GLboolean *params );
void glGetFloatv( GLenum pname, GLfloat *params );
void glGetIntegerv( GLenum pname, GLint *params );
#endif

int l_glGetDoublev(lua_State* L)
{
	GLdouble params[32];
	GLenum pname;
	int i, num_params;
	for(i=0;i<32;i++) { params[i] = DBL_MAX; }
	pname = luaL_checkint(L,1);
	glGetDoublev(pname, params);	
	for(i=0;i<32;i++) { 
		if(params[i]==DBL_MAX) { break; }
	}
	num_params=i;
	for(i=0;i<num_params;i++) {
		lua_pushnumber(L,params[i]);
	}
	return num_params;
}


BIND_0_1(glPushAttrib,int)
BIND_0_0(glPopAttrib)

BIND_0_1(glPushClientAttrib,int)
BIND_0_0(glPopClientAttrib)

BIND_1_1(number,glRenderMode,int)

int l_glGetError(lua_State* L) { lua_pushnumber(L,glGetError()); return 1; }

int l_glGetString(lua_State* L)
{
    lua_pushstring(L,(char const*)glGetString(luaL_checkint(L,1)));
    return 1;
}

BIND_0_0(glFinish)
BIND_0_0(glFlush)
BIND_0_2(glHint,int,int)

/*
 * Depth Buffer
 */

BIND_0_1(glClearDepth,int)
BIND_0_1(glDepthFunc,int)

int l_glDepthMask(lua_State* L) 
{ 
    luaL_checktype(L,1, LUA_TBOOLEAN);
    glDepthMask(lua_toboolean(L,1));
    return 0;
}

BIND_0_2(glDepthRange,int,int);

/*
 * Accumulation Buffer
 */


BIND_0_4(glClearAccum,number,number,number,number);
BIND_0_2(glAccum,int,number);

/*
 * Display Lists
 */

BIND_1_1(number,glIsList,int);
BIND_0_2(glDeleteLists,int,int);
BIND_1_1(number,glGenLists,int);
BIND_0_2(glNewList,int,int);
BIND_0_0(glEndList);
BIND_0_1(glCallList,int);
BIND_0_1(glListBase,int);
/* void glCallLists( GLsizei n, GLenum type, const GLvoid *lists ); */

/*
 * Lighting  
 */

BIND_0_1(glShadeModel,int);
BIND_0_3(glLightf,int,int,number);
BIND_0_2(glLightModelf,int,number);
BIND_0_2(glColorMaterial,int,int);

int l_glLightModelfv(lua_State* L) 
{
    int i;
    GLfloat params[4];
    for(i=0;i<4;i++) { params[i]=luaL_checknumber(L,2+i); }
    glLightModelfv(luaL_checkint(L,1), params);
    return 0;
}

int l_glLightv(lua_State* L)
{
    int i;
    GLfloat params[4];
    for(i=0;i<4;i++) { params[i]=luaL_checknumber(L,3+i); }
    glLightfv(luaL_checkint(L,1), luaL_checkint(L,2), params);
    return 0;
}




/*
 * Raster functions
 */

BIND_0_2(glPixelZoom,number,number)
BIND_0_2(glPixelStoref,int,number)
BIND_0_2(glPixelStorei,int,number)

BIND_0_2(glPixelTransferf,int,number)
BIND_0_2(glPixelTransferi,int,int)

#if 0
void glPixelMapfv( GLenum map, GLsizei mapsize, const GLfloat *values );
void glPixelMapuiv( GLenum map, GLsizei mapsize,
                                     const GLuint *values );
void glPixelMapusv( GLenum map, GLsizei mapsize,
                                     const GLushort *values );

void glGetPixelMapfv( GLenum map, GLfloat *values );
void glGetPixelMapuiv( GLenum map, GLuint *values );
void glGetPixelMapusv( GLenum map, GLushort *values );

void glBitmap( GLsizei width, GLsizei height,
        GLfloat xorig, GLfloat yorig,
        GLfloat xmove, GLfloat ymove,
        const GLubyte *bitmap );
#endif

/* The pixels are returned on the Lua stack, not in a table. */
int l_glReadPixels(lua_State* L)
{
    GLsizei i,w,h;
    GLfloat *pixels;
    w=luaL_checkint(L,3);
    h=luaL_checkint(L,4);
    pixels=malloc(w*h*sizeof(pixels[0]));
    glReadPixels(
            luaL_checkint(L,1),
            luaL_checkint(L,2),
            w,h,
            luaL_checkint(L,5),
            luaL_checkint(L,6),
            pixels);
    for(i=0;i<w*h;i++) { lua_pushnumber(L,pixels[i]); }
    free(pixels);
    return w*h;
}

int l_glDrawPixels(lua_State* L)
{
    int n;
    GLfloat* pixels = float_array_from_table_in_final_arg(L,&n);
    luaL_argcheck(L, luaL_checkint(L,4)==GL_FLOAT, 4, 
            "Only GL_FLOAT is currently supported for the type in glDrawPixels.");
    glDrawPixels(luaL_checkint(L,1), luaL_checkint(L,2), 
            luaL_checkint(L,3), luaL_checkint(L,4), pixels);
    free(pixels);
    return 0;
}

BIND_0_5(glCopyPixels,int,int,int,int,int)

/*
 * Stenciling
 */

BIND_0_3(glStencilFunc,int,int,int)
BIND_0_1(glStencilMask,int)
BIND_0_3(glStencilOp,int,int,int)
BIND_0_1(glClearStencil,int)


/*
 * Texture mapping
 */

/*
BIND_0_3(glTexGend,int,int,number)
BIND_0_3(glTexGeni,int,int,int)
*/

int l_glTexGen(lua_State* L)
{
    int i, nparams = lua_gettop(L)-2;
    GLdouble* params;
    
    if(nparams<1) { luaL_error(L,"Too few params passed to glTexGen"); }
    params = calloc(nparams,sizeof(GLdouble));
    for(i=0;i<nparams;i++) { params[i]=luaL_checknumber(L,3+i); }
    glTexGendv(luaL_checkint(L,1), luaL_checkint(L,2), params);
    free(params);
    return 0;
}

#if 0
void glGetTexGendv( GLenum coord, GLenum pname, GLdouble *params );
void glGetTexGenfv( GLenum coord, GLenum pname, GLfloat *params );
void glGetTexGeniv( GLenum coord, GLenum pname, GLint *params );
#endif

BIND_0_3(glTexEnvf,int,int,number)
BIND_0_3(glTexEnvi,int,int,int)

#if 0
void glTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
void glTexEnviv( GLenum target, GLenum pname, const GLint *params );

void glGetTexEnvfv( GLenum target, GLenum pname, GLfloat *params );
void glGetTexEnviv( GLenum target, GLenum pname, GLint *params );
#endif

BIND_0_3(glTexParameterf,int,int,number)
BIND_0_3(glTexParameteri,int,int,int)

#if 0
void glTexParameterfv( GLenum target, GLenum pname, const GLfloat *params );
void glTexParameteriv( GLenum target, GLenum pname, const GLint *params );

void glGetTexParameterfv( GLenum target, GLenum pname, GLfloat *params);
void glGetTexParameteriv( GLenum target, GLenum pname, GLint *params );

void glGetTexLevelParameterfv( GLenum target, GLint level,
                                                GLenum pname, GLfloat *params );
void glGetTexLevelParameteriv( GLenum target, GLint level,
                                                GLenum pname, GLint *params );


void glGetTexImage( GLenum target, GLint level,
                                     GLenum format, GLenum type,
                                     GLvoid *pixels );
#endif


int l_gluBuild1DMipmaps(lua_State* L)
{
    int n;
    GLfloat* pixels = float_array_from_table_in_final_arg(L,&n);
    luaL_argcheck(L, luaL_checkint(L,5)==GL_FLOAT, 5,
"The type argument of glTexImage1D should be GL_FLOAT, since it is the only\n"
"type that is currently supported.\n");
    gluBuild1DMipmaps(luaL_checkint(L,1), luaL_checkint(L,2), luaL_checkint(L,3),
                  luaL_checkint(L,4), luaL_checkint(L,5), pixels);
    free(pixels);
    return 0;
}

int l_glTexImage1D(lua_State* L)
{
    int n;
    GLfloat* pixels = float_array_from_table_in_final_arg(L,&n);
    luaL_argcheck(L, luaL_checkint(L,7)==GL_FLOAT, 7,
"The type argument of glTexImage1D should be GL_FLOAT, since it is the only\n"
"type that is currently supported.\n");
    glTexImage1D( luaL_checkint(L,1), luaL_checkint(L,2), luaL_checkint(L,3),
                  luaL_checkint(L,4), luaL_checkint(L,5), luaL_checkint(L,6),
                  luaL_checkint(L,7), pixels);
    free(pixels);
    return 0;
}

int l_glTexImage2D(lua_State* L)
{
    int n;
    GLfloat* pixels = float_array_from_table_in_final_arg(L,&n);
    luaL_argcheck(L, luaL_checkint(L,8)==GL_FLOAT, 8,
"The type argument of glTexImage2D should be GL_FLOAT, since it is the only\n"
"type that is currently supported.\n");
    glTexImage2D( luaL_checkint(L,1), luaL_checkint(L,2), 
                  luaL_checkint(L,3), luaL_checkint(L,4), 
                  luaL_checkint(L,5), luaL_checkint(L,6), 
                  luaL_checkint(L,7), luaL_checkint(L,8), 
                  pixels);
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
int l_glGenTextures(lua_State* L)
{
    int i;
    GLuint *textures;
    GLsizei n = luaL_checkint(L,1);
    textures = calloc(n,sizeof(textures[0]));
    glGenTextures(n,textures);
    for(i=0;i<n;i++) { lua_pushnumber(L,textures[i]); }
    free(textures);
    return n;
}

/* This wrapper just takes a variable number of texture arguments. 
 * The number of textures must not be included in the argument list,
 * unlike the C version of glDeleteTextures. 
 */
int l_glDeleteTextures(lua_State* L)
{
    int i;
    int n=lua_gettop(L);
    GLuint *textures;
    textures = calloc(n,sizeof(textures[0]));
    for(i=1;i<=n;i++) { textures[i-1]=luaL_checkint(L,i); }
    glDeleteTextures(n,textures);
    return 0;
}

BIND_0_2(glBindTexture,int,int)

#if 0
void glPrioritizeTextures( GLsizei n, const GLuint *textures, const GLclampf *priorities );
GLboolean glAreTexturesResident( GLsizei n, const GLuint *textures, GLboolean *residences );
#endif

BIND_1_1(number,glIsTexture,int)

#if 0
void glTexSubImage1D( GLenum target, GLint level,
                                       GLint xoffset,
                                       GLsizei width, GLenum format,
                                       GLenum type, const GLvoid *pixels );


void glTexSubImage2D( GLenum target, GLint level,
                                       GLint xoffset, GLint yoffset,
                                       GLsizei width, GLsizei height,
                                       GLenum format, GLenum type,
                                       const GLvoid *pixels );
#endif

BIND_0_7(glCopyTexImage1D,int,int,int,int,
                          int,int,int)
BIND_0_8(glCopyTexImage2D,int,int,int,int,
                          int,int,int,int)
BIND_0_6(glCopyTexSubImage1D,int,int,int,
                             int,int,int)
BIND_0_8(glCopyTexSubImage2D,int,int,int,int,
                             int,int,int,int)






/*
 * Fog
 */

BIND_0_2(glFogf,int,number)
BIND_0_2(glFogi,int,int)

int l_glFogfv(lua_State* L) 
{
    int n;
    GLfloat* params = float_array_from_table_in_final_arg(L,&n);
    luaL_argcheck(L, lua_tonumber(L,1)==GL_FOG_COLOR, 1,
            "The first argument to glFogfv should be GL_FOG_COLOR.");
    luaL_argcheck(L, n==4, 2,
"There should be five arguments to glFogfv, the last four being color components.");
    glFogfv(luaL_checkint(L,1), params);
    free(params);
    return 0;
}



/*
 * OpenGL 1.2
 */


#if 0
void glDrawRangeElements( GLenum mode, GLuint start,
	GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
#endif

#if 0
int l_glTexImage3D(lua_State* L)
{
    int n;
    GLfloat* pixels = float_array_from_table_in_final_arg(L,&n);
    luaL_argcheck(L, lua_tonumber(L,9)==GL_FLOAT, 9,
"The ninth argument (type) to glTexImage3D should be GL_FLOAT since no other types\n"
"are currently supported.");
    luaL_argcheck(L, n >= luaL_checkint(L,4)*luaL_checkint(L,5)*luaL_checkint(L,6), 10,
"The pixel table passed in as the final argument to glTexImage3D should contain "
"at least width*height*depth pixels, but it does not.");
    glTexImage3D(
            luaL_checkint(L,1), luaL_checkint(L,2), 
            luaL_checkint(L,3),
            luaL_checkint(L,4), luaL_checkint(L,5),
            luaL_checkint(L,6), luaL_checkint(L,7),
            luaL_checkint(L,8), luaL_checkint(L,9),
            pixels);
    free(pixels);
    return 0;
}
#endif


#if 0
void glTexSubImage3D( GLenum target, GLint level,
                                         GLint xoffset, GLint yoffset,
                                         GLint zoffset, GLsizei width,
                                         GLsizei height, GLsizei depth,
                                         GLenum format,
                                         GLenum type, const GLvoid *pixels);

void glCopyTexSubImage3D( GLenum target, GLint level,
                                             GLint xoffset, GLint yoffset,
                                             GLint zoffset, GLint x,
                                             GLint y, GLsizei width,
                                             GLsizei height );
#endif


/*
 * Selection and Feedback
 */

#if 0
void glFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer );
void glSelectBuffer( GLsizei size, GLuint *buffer );
#endif

BIND_0_1(glPassThrough,number)
BIND_0_0(glInitNames)
BIND_0_1(glLoadName,int)
BIND_0_1(glPushName,int)
BIND_0_0(glPopName)




/*
 * OpenGL 1.3
 */

#if 0
BIND_0_1(glActiveTexture,int)
BIND_0_1(glClientActiveTexture,int)
#endif

#if 0
void glCompressedTexImage1D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
void glCompressedTexImage2D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
void glCompressedTexImage3D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
void glCompressedTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
void glCompressedTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
void glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
void glGetCompressedTexImage( GLenum target, GLint lod, GLvoid *img );
#endif

#if 0
BIND_0_2(glMultiTexCoord1d,int,number)
BIND_0_3(glMultiTexCoord2d,int,number,number)
BIND_0_4(glMultiTexCoord3d,int,number,number,number)
BIND_0_5(glMultiTexCoord4d,int,number,number,number,number)
#endif

#if 0
void glLoadTransposeMatrixd( const GLdouble m[16] );
void glMultTransposeMatrixd( const GLdouble m[16] );

int l_glSampleCoverage(lua_State* L)
{
    luaL_argcheck(L, lua_isboolean(L,2), 2,
"The second argument to glSampleCoverage() must be a boolean.");
    glSampleCoverage(luaL_checknumber(L,1), lua_toboolean(L,2));
    return 0;
}
#endif



#if 0


Things to do later or, in some cases, never...


void glGetMaterialfv( GLenum face, GLenum pname, GLfloat *params );
void glGetLightfv( GLenum light, GLenum pname, GLfloat *params );
void glGetLightiv( GLenum light, GLenum pname, GLint *params );


/*
 * Drawing Functions
 */

void glIndexd( GLdouble c );
void glIndexf( GLfloat c );
void glIndexi( GLint c );
void glIndexs( GLshort c );
void glIndexub( GLubyte c );  /* 1.1 */

void glIndexdv( const GLdouble *c );
void glIndexfv( const GLfloat *c );
void glIndexiv( const GLint *c );
void glIndexsv( const GLshort *c );
void glIndexubv( const GLubyte *c );  /* 1.1 */





/*
 * Vertex Arrays  (1.1)
 */

void glVertexPointer( GLint size, GLenum type,
                                       GLsizei stride, const GLvoid *ptr );

void glNormalPointer( GLenum type, GLsizei stride,
                                       const GLvoid *ptr );

void glColorPointer( GLint size, GLenum type,
                                      GLsizei stride, const GLvoid *ptr );

void glIndexPointer( GLenum type, GLsizei stride,
                                      const GLvoid *ptr );

void glTexCoordPointer( GLint size, GLenum type,
                                         GLsizei stride, const GLvoid *ptr );

void glEdgeFlagPointer( GLsizei stride, const GLvoid *ptr );

void glGetPointerv( GLenum pname, GLvoid **params );

void glArrayElement( GLint i );

void glDrawElements( GLenum mode, GLsizei count,
                                      GLenum type, const GLvoid *indices );





/*
 * Evaluators
 */

void glMap1d( GLenum target, GLdouble u1, GLdouble u2,
                               GLint stride,
                               GLint order, const GLdouble *points );
void glMap1f( GLenum target, GLfloat u1, GLfloat u2,
                               GLint stride,
                               GLint order, const GLfloat *points );

void glMap2d( GLenum target,
		     GLdouble u1, GLdouble u2, GLint ustride, GLint uorder,
		     GLdouble v1, GLdouble v2, GLint vstride, GLint vorder,
		     const GLdouble *points );
void glMap2f( GLenum target,
		     GLfloat u1, GLfloat u2, GLint ustride, GLint uorder,
		     GLfloat v1, GLfloat v2, GLint vstride, GLint vorder,
		     const GLfloat *points );

void glGetMapdv( GLenum target, GLenum query, GLdouble *v );
void glGetMapfv( GLenum target, GLenum query, GLfloat *v );
void glGetMapiv( GLenum target, GLenum query, GLint *v );

void glEvalCoord1d( GLdouble u );
void glEvalCoord1f( GLfloat u );

void glEvalCoord1dv( const GLdouble *u );
void glEvalCoord1fv( const GLfloat *u );

void glEvalCoord2d( GLdouble u, GLdouble v );
void glEvalCoord2f( GLfloat u, GLfloat v );

void glEvalCoord2dv( const GLdouble *u );
void glEvalCoord2fv( const GLfloat *u );

void glMapGrid1d( GLint un, GLdouble u1, GLdouble u2 );
void glMapGrid1f( GLint un, GLfloat u1, GLfloat u2 );

void glMapGrid2d( GLint un, GLdouble u1, GLdouble u2,
                                   GLint vn, GLdouble v1, GLdouble v2 );
void glMapGrid2f( GLint un, GLfloat u1, GLfloat u2,
                                   GLint vn, GLfloat v1, GLfloat v2 );

void glEvalPoint1( GLint i );

void glEvalPoint2( GLint i, GLint j );

void glEvalMesh1( GLenum mode, GLint i1, GLint i2 );

void glEvalMesh2( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 );



/*
 * GL_ARB_imaging
 */



void glColorTable( GLenum target, GLenum internalformat,
                                    GLsizei width, GLenum format,
                                    GLenum type, const GLvoid *table );

void glColorSubTable( GLenum target,
                                       GLsizei start, GLsizei count,
                                       GLenum format, GLenum type,
                                       const GLvoid *data );

void glColorTableParameteriv(GLenum target, GLenum pname,
                                              const GLint *params);

void glColorTableParameterfv(GLenum target, GLenum pname,
                                              const GLfloat *params);

void glCopyColorSubTable( GLenum target, GLsizei start,
                                           GLint x, GLint y, GLsizei width );

void glCopyColorTable( GLenum target, GLenum internalformat,
                                        GLint x, GLint y, GLsizei width );

void glGetColorTable( GLenum target, GLenum format,
                                       GLenum type, GLvoid *table );

void glGetColorTableParameterfv( GLenum target, GLenum pname,
                                                  GLfloat *params );

void glGetColorTableParameteriv( GLenum target, GLenum pname,
                                                  GLint *params );

void glBlendEquation( GLenum mode );

void glBlendColor( GLclampf red, GLclampf green,
                                    GLclampf blue, GLclampf alpha );

void glHistogram( GLenum target, GLsizei width,
				   GLenum internalformat, GLboolean sink );

void glResetHistogram( GLenum target );

void glGetHistogram( GLenum target, GLboolean reset,
				      GLenum format, GLenum type,
				      GLvoid *values );

void glGetHistogramParameterfv( GLenum target, GLenum pname,
						 GLfloat *params );

void glGetHistogramParameteriv( GLenum target, GLenum pname,
						 GLint *params );

void glMinmax( GLenum target, GLenum internalformat,
				GLboolean sink );

void glResetMinmax( GLenum target );

void glGetMinmax( GLenum target, GLboolean reset,
                                   GLenum format, GLenum types,
                                   GLvoid *values );

void glGetMinmaxParameterfv( GLenum target, GLenum pname,
					      GLfloat *params );

void glGetMinmaxParameteriv( GLenum target, GLenum pname,
					      GLint *params );

void glConvolutionFilter1D( GLenum target,
	GLenum internalformat, GLsizei width, GLenum format, GLenum type,
	const GLvoid *image );

void glConvolutionFilter2D( GLenum target,
	GLenum internalformat, GLsizei width, GLsizei height, GLenum format,
	GLenum type, const GLvoid *image );

void glConvolutionParameterf( GLenum target, GLenum pname,
	GLfloat params );

void glConvolutionParameterfv( GLenum target, GLenum pname,
	const GLfloat *params );

void glConvolutionParameteri( GLenum target, GLenum pname,
	GLint params );

void glConvolutionParameteriv( GLenum target, GLenum pname,
	const GLint *params );

void glCopyConvolutionFilter1D( GLenum target,
	GLenum internalformat, GLint x, GLint y, GLsizei width );

void glCopyConvolutionFilter2D( GLenum target,
	GLenum internalformat, GLint x, GLint y, GLsizei width,
	GLsizei height);

void glGetConvolutionFilter( GLenum target, GLenum format,
	GLenum type, GLvoid *image );

void glGetConvolutionParameterfv( GLenum target, GLenum pname,
	GLfloat *params );

void glGetConvolutionParameteriv( GLenum target, GLenum pname,
	GLint *params );

void glSeparableFilter2D( GLenum target,
	GLenum internalformat, GLsizei width, GLsizei height, GLenum format,
	GLenum type, const GLvoid *row, const GLvoid *column );

void glGetSeparableFilter( GLenum target, GLenum format,
	GLenum type, GLvoid *row, GLvoid *column, GLvoid *span );





void glActiveTextureARB(GLenum texture);
void glClientActiveTextureARB(GLenum texture);
void glMultiTexCoord1dARB(GLenum target, GLdouble s);
void glMultiTexCoord1dvARB(GLenum target, const GLdouble *v);
void glMultiTexCoord1fARB(GLenum target, GLfloat s);
void glMultiTexCoord1fvARB(GLenum target, const GLfloat *v);
void glMultiTexCoord1iARB(GLenum target, GLint s);
void glMultiTexCoord1ivARB(GLenum target, const GLint *v);
void glMultiTexCoord1sARB(GLenum target, GLshort s);
void glMultiTexCoord1svARB(GLenum target, const GLshort *v);
void glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t);
void glMultiTexCoord2dvARB(GLenum target, const GLdouble *v);
void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
void glMultiTexCoord2fvARB(GLenum target, const GLfloat *v);
void glMultiTexCoord2iARB(GLenum target, GLint s, GLint t);
void glMultiTexCoord2ivARB(GLenum target, const GLint *v);
void glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t);
void glMultiTexCoord2svARB(GLenum target, const GLshort *v);
void glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r);
void glMultiTexCoord3dvARB(GLenum target, const GLdouble *v);
void glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
void glMultiTexCoord3fvARB(GLenum target, const GLfloat *v);
void glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r);
void glMultiTexCoord3ivARB(GLenum target, const GLint *v);
void glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r);
void glMultiTexCoord3svARB(GLenum target, const GLshort *v);
void glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void glMultiTexCoord4dvARB(GLenum target, const GLdouble *v);
void glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void glMultiTexCoord4fvARB(GLenum target, const GLfloat *v);
void glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q);
void glMultiTexCoord4ivARB(GLenum target, const GLint *v);
void glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void glMultiTexCoord4svARB(GLenum target, const GLshort *v);




typedef void (*GLprogramcallbackMESA)(GLenum target, GLvoid *data);

void glProgramCallbackMESA(GLenum target, GLprogramcallbackMESA callback, GLvoid *data);

void glGetProgramRegisterfvMESA(GLenum target, GLsizei len, const GLubyte *name, GLfloat *v);



#ifndef GL_ATI_blend_equation_separate
#define GL_ATI_blend_equation_separate 1

#define GL_ALPHA_BLEND_EQUATION_ATI	        0x883D

void glBlendEquationSeparateATI( GLenum modeRGB, GLenum modeA );
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEATIPROC) (GLenum modeRGB, GLenum modeA);

#endif /* GL_ATI_blend_equation_separate */

#endif


#define ENTRY(f) { # f , l_ ## f }

static const struct luaL_reg gl_lib[] =
{
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
#if 0
	ENTRY(glActiveTexture),
    ENTRY(glClientActiveTexture),
    ENTRY(glMultiTexCoord1d),
    ENTRY(glMultiTexCoord2d),
    ENTRY(glMultiTexCoord3d),
    ENTRY(glMultiTexCoord4d),
#endif

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
#if 0
    ENTRY(glTexImage3D), 
    ENTRY(glSampleCoverage)
#endif
	{NULL, NULL}
};


int luaopen_gl(lua_State* L)
{
    luaL_openlib(L, "gl", gl_lib, 0);
    return 1;
}

