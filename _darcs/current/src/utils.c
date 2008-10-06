#include "utils.h"
#include "my_opengl.h"
#include <stdio.h>
#include <stdlib.h>

void unpack_color(GLuint color, float* r, float* g, float* b, float* a)
{
	*a=(color&0xff)/255.f;
	*b=((color>>8)&0xff)/255.f;
	*g=((color>>16)&0xff)/255.f;
	*r=((color>>24)&0xff)/255.f;
}

void b3d_fatal_error(const char msg[])
{
#ifdef WIN32
	MessageBox(NULL,msg,"Fatal error",MB_OK);
#else
	fprintf(stderr,"Fatal error: %s\n",msg);
#endif
	exit(-1);
}

void b3d_warn(const char msg[])
{
#ifdef WIN32
	MessageBox(NULL,msg,"Warning",MB_OK);
#else
	fprintf(stderr,"Warning: %s\n",msg);
#endif 
}

