#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <unistd.h>
#include <GL/glut.h>

#ifndef min
#define min(a,b)	(a<b? a : b)
#endif

#ifndef max
#define max(a,b)	(a>b? a : b)
#endif

void unpack_color(GLuint color, float *r, float *g, float *b, float *a);

#endif

