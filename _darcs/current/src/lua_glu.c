#include "my_lua.h"
#include "my_opengl.h"
#include "lua_bind.h"

#if 0
typedef struct GLUnurbs GLUnurbsObj;
typedef struct GLUquadric GLUquadricObj;
typedef struct GLUtesselator GLUtesselatorObj;
typedef struct GLUtesselator GLUtriangulatorObj;

/* Internal convenience typedefs */
#ifdef __cplusplus
typedef GLvoid (*_GLUfuncptr)();
#else
typedef GLvoid (*_GLUfuncptr)(GLvoid);
#endif
#endif

#if 0
extern void gluBeginCurve (GLUnurbs* nurb);
extern void gluBeginPolygon (GLUtesselator* tess);
extern void gluBeginSurface (GLUnurbs* nurb);
extern void gluBeginTrim (GLUnurbs* nurb);
#endif

#if 0
extern GLint gluBuild1DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
extern GLint gluBuild1DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data);
extern GLint gluBuild2DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
extern GLint gluBuild2DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data);
extern GLint gluBuild3DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
extern GLint gluBuild3DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
#endif

/* BIND_1_2(string,gluCheckExtension,string,string) */

#if 0
extern void gluCylinder (GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);
extern void gluDeleteNurbsRenderer (GLUnurbs* nurb);
extern void gluDeleteQuadric (GLUquadric* quad);
extern void gluDeleteTess (GLUtesselator* tess);
extern void gluDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops);
extern void gluEndCurve (GLUnurbs* nurb);
extern void gluEndPolygon (GLUtesselator* tess);
extern void gluEndSurface (GLUnurbs* nurb);
extern void gluEndTrim (GLUnurbs* nurb);
#endif

BIND_1_1(string,gluErrorString,int) 

#if 0
extern void gluGetNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat* data);
#endif

BIND_1_1(string,gluGetString,int)
#if 0
extern void gluGetTessProperty (GLUtesselator* tess, GLenum which, GLdouble* data);
extern void gluLoadSamplingMatrices (GLUnurbs* nurb, const GLfloat *model, const GLfloat *perspective, const GLint *view);
#endif

int l_gluLookAt(lua_State* L)
{
    gluLookAt(
            luaL_checknumber(L,1),
            luaL_checknumber(L,2),
            luaL_checknumber(L,3),
            luaL_checknumber(L,4),
            luaL_checknumber(L,5),
            luaL_checknumber(L,6),
            luaL_checknumber(L,7),
            luaL_checknumber(L,8),
            luaL_checknumber(L,9));
    return 0;
}

#if 0
extern GLUnurbs* gluNewNurbsRenderer (void);
extern GLUquadric* gluNewQuadric (void);
extern GLUtesselator* gluNewTess (void);
extern void gluNextContour (GLUtesselator* tess, GLenum type);
extern void gluNurbsCallback (GLUnurbs* nurb, GLenum which, _GLUfuncptr CallBackFunc);
extern void gluNurbsCallbackData (GLUnurbs* nurb, GLvoid* userData);
extern void gluNurbsCallbackDataEXT (GLUnurbs* nurb, GLvoid* userData);
extern void gluNurbsCurve (GLUnurbs* nurb, GLint knotCount, GLfloat *knots, GLint stride, GLfloat *control, GLint order, GLenum type);
extern void gluNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat value);
extern void gluNurbsSurface (GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride, GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type);
#endif

BIND_0_4(gluOrtho2D,number,number,number,number)

#if 0
extern void gluPartialDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops, GLdouble start, GLdouble sweep);
#endif

BIND_0_4(gluPerspective,number,number,number,number)

/* The viewport argument is optional. */
int l_gluPickMatrix(lua_State* L)
{
    int i;
    GLint viewport[4];
    if(lua_gettop(L)==8) {
        for(i=0;i<4;i++) { viewport[i] = luaL_checkint(L,5+i); }
    }
    else {
        glGetIntegerv(GL_VIEWPORT,viewport);
    }
    gluPickMatrix(
            luaL_checknumber(L,1),
            luaL_checknumber(L,2),
            luaL_checknumber(L,3),
            luaL_checknumber(L,4),
            viewport);
    return 0;
}

/* The model, proj, and view arguments are omitted. */
int l_gluProject(lua_State* L)
{
    GLint viewport[4];
    GLdouble model[16], proj[16], x,y,z;
   
    glGetDoublev(GL_MODELVIEW_MATRIX,model);
    glGetDoublev(GL_PROJECTION_MATRIX,proj);
    glGetIntegerv(GL_VIEWPORT,viewport);
    if(!gluProject(
                luaL_checknumber(L,1), 
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                model,proj,viewport, 
                &x,&y,&z)) 
    {
        luaL_error(L,"gluProject() failed");
    }
    lua_pushnumber(L,x);
    lua_pushnumber(L,y);
    lua_pushnumber(L,z);
    return 3;
}

#if 0
extern void gluPwlCurve (GLUnurbs* nurb, GLint count, GLfloat* data, GLint stride, GLenum type);
extern void gluQuadricCallback (GLUquadric* quad, GLenum which, _GLUfuncptr CallBackFunc);
extern void gluQuadricDrawStyle (GLUquadric* quad, GLenum draw);
extern void gluQuadricNormals (GLUquadric* quad, GLenum normal);
extern void gluQuadricOrientation (GLUquadric* quad, GLenum orientation);
extern void gluQuadricTexture (GLUquadric* quad, GLboolean texture);
extern GLint gluScaleImage (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut);
extern void gluSphere (GLUquadric* quad, GLdouble radius, GLint slices, GLint stacks);
extern void gluTessBeginContour (GLUtesselator* tess);
extern void gluTessBeginPolygon (GLUtesselator* tess, GLvoid* data);
extern void gluTessCallback (GLUtesselator* tess, GLenum which, _GLUfuncptr CallBackFunc);
extern void gluTessEndContour (GLUtesselator* tess);
extern void gluTessEndPolygon (GLUtesselator* tess);
extern void gluTessNormal (GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ);
extern void gluTessProperty (GLUtesselator* tess, GLenum which, GLdouble data);
extern void gluTessVertex (GLUtesselator* tess, GLdouble *location, GLvoid* data);
#endif

/* The model, proj, and view arguments are omitted. */
int l_gluUnProject(lua_State* L)
{
    GLint viewport[4];
    GLdouble model[16], proj[16], x,y,z;
   
    glGetDoublev(GL_MODELVIEW_MATRIX,model);
    glGetDoublev(GL_PROJECTION_MATRIX,proj);
    glGetIntegerv(GL_VIEWPORT,viewport);
    if(!gluUnProject(
                luaL_checknumber(L,1), 
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                model,proj,viewport, 
                &x,&y,&z)) 
    {
        luaL_error(L,"gluUnProject() failed");
    }
    lua_pushnumber(L,x);
    lua_pushnumber(L,y);
    lua_pushnumber(L,z);
    return 3;
}

#if 0
/* The model, proj, view, near, and far arguments are omitted. */
int l_gluUnProject4(lua_State* L)
{
    GLint viewport[4];
    GLdouble model[16], proj[16], near,far, x,y,z,w;
   
    glGetDoublev(GL_MODELVIEW_MATRIX,model);
    glGetDoublev(GL_PROJECTION_MATRIX,proj);
    glGetIntegerv(GL_VIEWPORT,viewport);
    if(!gluUnProject4(
                luaL_checknumber(L,1), 
                luaL_checknumber(L,2),
                luaL_checknumber(L,3),
                luaL_checknumber(L,4),
                model,proj,viewport, near,far,
                &x,&y,&z,&w)) 
    {
        luaL_error(L,"gluUnProject4() failed");
    }
    lua_pushnumber(L,x);
    lua_pushnumber(L,y);
    lua_pushnumber(L,z);
    lua_pushnumber(L,w);
    return 4;
}
#endif






#define ENTRY(f) { # f , l_ ## f },

static const struct luaL_reg glu_lib[] =
{
    /* ENTRY(gluCheckExtension) */
    ENTRY(gluErrorString)
    ENTRY(gluGetString)
    ENTRY(gluLookAt)
    ENTRY(gluOrtho2D)
    ENTRY(gluPerspective)
    ENTRY(gluPickMatrix)
    ENTRY(gluProject)
    ENTRY(gluUnProject)
    {NULL, NULL}
};


int luaopen_glu(lua_State* L)
{
    luaL_openlib(L, "glu", glu_lib, 0);
    return 1;
}


