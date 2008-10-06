#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#ifdef WIN32
#include <windows.h>
/* #include <mapiwin.h> */
#include <direct.h> /* for chdir */
#define snprintf _snprintf
#define chdir _chdir
#define usleep Sleep
#else
#include <unistd.h>
#endif

#include "my_opengl.h"

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

void unpack_color(GLuint color, float* r, float* g, float* b, float* a);
void b3d_fatal_error(const char msg[]);
void b3d_warn(const char msg[]);
const char* get_filename(const char* types[], int ntypes);


#endif
