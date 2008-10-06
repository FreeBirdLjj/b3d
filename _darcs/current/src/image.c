/* Large hunks of code here have been lifted from /usr/share/libpng/example.c */

#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>

#include "my_lua.h"
#include "image.h"
#include "utils.h"
#include "my_opengl.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "jpeglib.h"

unsigned char* read_JPEG_file (lua_State* L, const char * filename, 
							   int *width, int *height);
static image_t* checkimage(lua_State* L);

int l_image_load(lua_State* L)
{
    const char* filename;
    image_t* img;

    filename=luaL_checkstring(L,1);

    img = (image_t*)lua_newuserdata(L,sizeof(image_t));

    /* Set the metatable of the image. */
    luaL_getmetatable(L,"brainmaps_image");  
    lua_setmetatable(L,-2);

	img->pixels=read_JPEG_file(L, filename, &img->nx, &img->ny);
	img->texture_is_valid=0;

    return 1; /* Tell Lua about the image we're returning on its stack. */
}


static image_t* checkimage(lua_State* L)
{
    image_t* img=(image_t*)luaL_checkudata(L,1,"brainmaps_image");
    luaL_argcheck(L, img!=NULL, 1, "`image' expected");
    return img;
}

static int l_image_draw_pixels(lua_State* L)
{
    image_t* im=checkimage(L);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(im->nx,im->ny,GL_RGBA,GL_UNSIGNED_BYTE,im->pixels);
    return 0;
}

#ifdef WIN32
double round(double x) { return floor(x+0.5); }
double log2(double x) { return log(x)/log(2.0); }
#endif

int is_power_of_two(int n) { return exp(round(log2(n)))==(double)n; }
int next_highest_power_of_two(int n) { return (int)exp(ceil(log2(n))); }

/* Not yet tested */
static void send_padded_texture_to_gl(int level, int w, int h, GLubyte* pixels)
{
    double filter[3][3] =
    {
        { 1,2,1 },
        { 2,4,2 },
        { 1,2,1 },
    };

    if(w==h && is_power_of_two(w)) {
        glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
        if(w>1) { /* Scale down and send again. */
            int w2,h2, yi,xi,ci;
            GLubyte* pix2;
            w2=w/2;
            h2=h/2;
            pix2=malloc(w2*h2*4);
            for(yi=0;yi<h2;yi++) {
                for(xi=0;xi<w2;xi++) {
                    for(ci=0;ci<4;ci++) {
                        double acc=0.0,denom=0.0;
                        int xbig,ybig,dx,dy;
                        for(dx=-1;dx<=1;dx++) {
                            xbig=2*xi+dx;
                            if(xbig<0||xbig>=w) { continue; }
                            for(dy=-1;dy<=1;dy++) {
                                double f=filter[dx-(-1)][dy-(-1)];
                                ybig=2*yi+dy;
                                if(ybig<0||ybig>=h) { continue; }
                                denom+=f;
                                acc+=f*pixels[ci+4*(xbig+w*ybig)];
                            }
                        }
                        acc/=denom;
                        pix2[ci+4*(xi+w2*yi)] = (GLubyte)acc;
                    }
                }
            }
            send_padded_texture_to_gl(level+1,w2,h2,pix2);
            free(pix2);
        }
    }
    else {
        /* Create a larger image whose size is the next highest power of
         * two. */
        int w2,h2, xi,yi,ci;
        GLubyte* pix2;
        w2=next_highest_power_of_two(max(w,h));
        h2=w2;
        pix2=malloc(h2*w2*4);
        for(yi=0;yi<h2;yi++) {
            for(xi=0;xi<w2;xi++) {
                for(ci=0;ci<4;ci++) {
                    pix2[ci+4*(xi+w2*yi)] = (xi>=w||yi>=h)?0:pixels[ci+4*(xi+w*yi)];
                }
            }
        }
        send_padded_texture_to_gl(level, w2, h2, pix2);
        free(pix2);
    }
}

static int l_image_bind_texture(lua_State* L)
{
    image_t* im=checkimage(L);
    if(im->texture_is_valid) {
        glBindTexture(GL_TEXTURE_2D, im->gl_texture_name); 
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glGenTextures(1, &im->gl_texture_name);
        glBindTexture(GL_TEXTURE_2D, im->gl_texture_name); 
        im->texture_is_valid=GL_TRUE;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        send_padded_texture_to_gl(0, im->nx, im->ny, im->pixels);
    }
    return 0;
}

static int l_image_get_size(lua_State* L)
{
    image_t* im=checkimage(L);
	lua_pushnumber(L,im->nx);
	lua_pushnumber(L,im->ny);
	return 2;
}

static const struct luaL_reg imagelib_f[] = {
    { "load", l_image_load }, 
    { NULL, NULL }
};

static const struct luaL_reg imagelib_m[] = {
    { "draw_pixels", l_image_draw_pixels },
    { "bind_texture", l_image_bind_texture },
	{ "get_size", l_image_get_size },
    { NULL, NULL }
};

static int image_gc(lua_State* L)
{
    image_t* im = checkimage(L);
    fprintf(stderr,"Freeing image %p\n",im);
    if(im) {
        if(im->texture_is_valid) { glDeleteTextures(1,&im->gl_texture_name); }
        free(im->pixels);
    }
    return 0;
}

int lua_openimage(lua_State* L)
{
    luaL_newmetatable(L, "brainmaps_image");

    /* Set the __gc field of the metatable so the images will be garbage
     * collected. */
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, image_gc);
    lua_settable(L,-3);

    /* p. 245 */
    lua_pushstring(L, "__index");
    lua_pushvalue(L,-2); /* pushes the metatable */
    lua_settable(L,-3); /* metatable.__index = metatable */

    luaL_openlib(L,NULL,imagelib_m,0);
    luaL_openlib(L,"image",imagelib_f,0);
    return 1;

}



/* The code below this line derived from example.c in the jpeglib source
 * distribution.  -ijt */


/*
 * example.c
 *
 * This file illustrates how to use the IJG code as a subroutine library
 * to read or write JPEG image files.  You should look at this code in
 * conjunction with the documentation file libjpeg.doc.
 *
 * This code will not do anything useful as-is, but it may be helpful as a
 * skeleton for constructing routines that call the JPEG library.  
 *
 * We present these routines in the same coding style used in the JPEG code
 * (ANSI function definitions, etc); but you are of course free to code your
 * routines in a different style if you prefer.
 */


/******************** JPEG DECOMPRESSION SAMPLE INTERFACE *******************/

/* This half of the example shows how to read data from the JPEG decompressor.
 * It's a bit more refined than the above [snipped out code -ijt], in that we
 * show:
 *   (a) how to modify the JPEG library's standard error-reporting behavior;
 *   (b) how to allocate workspace using the library's memory manager.
 *
 * Just to make this example a little different from the first one, we'll
 * assume that we do not intend to put the whole image into an in-memory
 * buffer, but to send it line-by-line someplace else.  We need a one-
 * scanline-high JSAMPLE array as a work buffer, and we will let the JPEG
 * memory manager allocate it for us.  This approach is actually quite useful
 * because we don't need to remember to deallocate the buffer separately: it
 * will go away automatically when the JPEG object is cleaned up.
 */


/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */


unsigned char*
read_JPEG_file (lua_State* L, const char * filename, int *width, int *height)
{
    unsigned char *ret;
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct my_error_mgr jerr;
    /* More stuff */
    FILE * infile;		/* source file */
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
     * so that the setjmp() error recovery below can assume the file is open.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to read binary files.
     */

    if ((infile = fopen(filename, "rb")) == NULL) {
        luaL_error(L,"Could not open %s.\n",filename);
        return NULL;
    }

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        luaL_error(L,"An error occurred while reading %s.\n",filename);
        return NULL;
    }
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */

    (void) jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
     *   (a) suspension is not possible with the stdio data source, and
     *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
     * See libjpeg.doc for more info.
     */

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */

    /* Step 5: Start decompressor */

    (void) jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    /* We may need to do some setup of our own at this point before reading
     * the data.  After jpeg_start_decompress() we have the correct scaled
     * output image dimensions available, as well as the output colormap
     * if we asked for color quantization.
     * In this example, we need to make an output work buffer of the right size.
     */ 
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
     * loop counter, so that we don't have to keep track ourselves.
     */
    {
        int w,h,xi,yi,ci;
        h=cinfo.output_height;
        w=cinfo.output_width;
        *width = w;
        *height = h;
        ret = malloc(4*w*h);
        if(cinfo.out_color_components!=3) {
            luaL_error(L,
"Only three-channel images are supported.  %s has %i.\n",
filename,cinfo.out_color_components);
            return NULL;
        }
		for(yi=0;yi<h;yi++) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            JSAMPLE* pixelrow = buffer[0];
			assert((int)cinfo.output_scanline==yi);
            jpeg_read_scanlines(&cinfo, buffer, 1);
            for(xi=0;xi<w;xi++) {
                for(ci=0;ci<3;ci++) {
                    ret[ci+4*(xi+w*yi)] = pixelrow[ci+3*xi];
                }
                ret[3+4*(xi+w*yi)]=255; /* 100% opaque */
            }
         }
    }

    /* Step 7: Finish decompression */

    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* After finish_decompress, we can close the input file.
     * Here we postpone it until after no more JPEG errors are possible,
     * so as to simplify the setjmp error logic above.  (Actually, I don't
     * think that jpeg_destroy can do an error exit, but why assume anything...)
     */
    fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
     * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
     */

    return ret;
}


/*
 * SOME FINE POINTS:
 *
 * In the above code, we ignored the return value of jpeg_read_scanlines,
 * which is the number of scanlines actually read.  We could get away with
 * this because we asked for only one line at a time and we weren't using
 * a suspending data source.  See libjpeg.doc for more info.
 *
 * We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
 * we should have done it beforehand to ensure that the space would be
 * counted against the JPEG max_memory setting.  In some systems the above
 * code would risk an out-of-memory error.  However, in general we don't
 * know the output image dimensions before jpeg_start_decompress(), unless we
 * call jpeg_calc_output_dimensions().  See libjpeg.doc for more about this.
 *
 * Scanlines are returned in the same order as they appear in the JPEG file,
 * which is standardly top-to-bottom.  If you must emit data bottom-to-top,
 * you can use one of the virtual arrays provided by the JPEG memory manager
 * to invert the data.  See wrbmp.c for an example.
 *
 * As with compression, some operating modes may require temporary files.
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 */

 
