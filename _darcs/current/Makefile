os:=$(shell uname)
CFLAGS:=-g -Ilua-5.0.2/include -Ifreeglut-2.4.0/include/

LIBS:=-llua -llualib -lm -ljpeg

GLLIBS:=-lglut -lGL -lGLU  # default
ifeq ($(os),Darwin)
CFLAGS+=-DDARWIN  
GLLIBS:=-framework GLUT -framework OpenGL -framework Cocoa
endif
ifeq ($(os),MINGW32_NT-5.1)
GLLIBS:=c:/lib/freeglut_static.lib -lopengl32 -lglu32
endif

LIBS+=$(GLLIBS)

ofiles:=src/main.o src/mesh.o src/view.o src/client.o src/my_lua.o src/utils.o\
    src/globals.o src/lua_gl.o src/lua_glu.o src/lua_glut.o src/lbitlib.o \
	src/image.o
prog:=src/b3d

$(prog): $(ofiles) 
	gcc -o $(prog) $(ofiles) $(LIBS)

clean:
	rm -f $(ofiles) $(prog) 

src/client.c: src/config.h src/globals.h src/utils.h 
src/globals.c: src/view.h src/mesh.h src/globals.h src/my_lua.h 
src/image.c: src/my_lua.h src/image.h 
src/lbitlib.c: 
src/lua_gl.c: src/my_opengl.h src/my_lua.h src/lua_bind.h 
src/lua_glu.c: src/my_lua.h src/my_opengl.h src/lua_bind.h 
src/lua_glut.c: src/my_opengl.h src/my_lua.h src/lua_bind.h 
src/main.c: src/view.h src/mesh.h src/globals.h src/my_lua.h 
src/mesh.c: src/globals.h src/utils.h src/mesh.h 
src/my_lua.c: src/mesh.h src/my_lua.h src/globals.h src/utils.h src/view.h src/client.h src/lua_gl.h src/lua_glu.h src/lua_glut.h 
src/utils.c: src/my_opengl.h 
src/view.c: src/globals.h src/utils.h src/view.h src/client.h src/my_lua.h 
src/zlib.c: src/zlib.h 
src/client.h: 
src/config.h: 
src/globals.h: src/mesh.h src/view.h 
src/image.h: src/my_opengl.h 
src/lua_bind.h: 
src/lua_gl.h: 
src/lua_glu.h: 
src/lua_glut.h: 
src/mesh.h: src/my_lua.h src/my_opengl.h 
src/my_lua.h: 
src/my_opengl.h: 
src/utils.h: 
src/view.h: src/my_opengl.h src/mesh.h src/my_lua.h 
src/zlib.h: 
