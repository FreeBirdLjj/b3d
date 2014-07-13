#ifndef __B3D_UTILS_H__
#define __B3D_UTILS_H__

#include <unistd.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef MIN
#define MIN(a, b)	({ typeof(a) _a = a; typeof(b) _b = b; _a<_b? _a : _b; })
#endif

#ifndef MAX
#define MAX(a, b)	({ typeof(a) _a = a; typeof(b) _b = b; _a>_b? _a : _b; })
#endif

void unpack_color(GLuint color, float *r, float *g, float *b, float *a);

#endif
