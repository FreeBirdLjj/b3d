#include "utils.h"
#include <GL/glut.h>

void unpack_color(GLuint color, float *r, float *g, float *b, float *a){
	GLubyte *t = (GLubyte *)&color;
	*a = *t++/255.0f;
	*b = *t++/255.0f;
	*g = *t++/255.0f;
	*r = *t/255.0f;
}

