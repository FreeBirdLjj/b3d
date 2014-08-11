#include "utils.h"

void unpack_color(GLuint color, GLfloat *r, GLfloat *g, GLfloat *b, GLfloat *a){
	GLubyte *t = (GLubyte *)&color;

	*r = t[3]/255.0f;
	*g = t[2]/255.0f;
	*b = t[1]/255.0f;
	*a = t[0]/255.0f;
}

