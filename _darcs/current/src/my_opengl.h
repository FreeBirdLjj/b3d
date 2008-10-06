#ifdef WIN32
#define _CRT_TERMINATE_DEFINED
#endif

#ifdef DARWIN
# include <GLUT/glut.h>
# include <OpenGL/gl.h>
#else
# include <GL/glut.h>
#endif

