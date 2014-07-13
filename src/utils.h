#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <unistd.h>
#include <GL/glut.h>

#ifndef min
#define min(a, b)	({ typeof(a) _a = a; typeof(b) _b = b; _a<_b? _a : _b; })
#endif

#ifndef max
#define max(a, b)	({ typeof(a) _a = a; typeof(b) _b = b; _a>_b? _a : _b; })
#endif

void unpack_color(GLuint color, float *r, float *g, float *b, float *a);

#endif

