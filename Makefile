PWD = $(shell pwd)

LIBJPEG_DIR = $(PWD)/jpeg
LIBJPEG = $(LIBJPEG_DIR)/.libs/libjpeg.a
LIBREADLINE_DIR = $(PWD)/readline
LIBREADLINE = $(LIBREADLINE_DIR)/libreadline.a
LIBLUA_DIR = $(PWD)/lua
LIBLUA = $(LIBLUA_DIR)/src/liblua.a

OS_NAME = $(shell uname)

LUA_TARGET =
GLLIBS =
ifeq ($(OS_NAME),Darwin)
LUA_TARGET := macosx
GLLIBS := -framework GLUT -framework OpenGL -framework Cocoa
else ifeq ($(OS_NAME),Linux)
LUA_TARGET := linux
GLLIBS := -lglut -lGL -lGLU
endif

CPPFLAGS = -I$(LIBLUA_DIR)/src -I$(LIBJPEG_DIR)
CFLAGS ?= -g -O3
LOADLIBES = -L$(LIBLUA_DIR)/src -L$(LIBJPEG_DIR)/.libs
LDLIBS = -llua -ldl -lm -ljpeg $(GLLIBS)

BIN = bin/b3d

.PHONY: all clean

all: $(LIBLUA) $(LIBJPEG) $(BIN)

$(LIBREADLINE):
	cd $(LIBREADLINE_DIR) && ./configure --disable-shared && $(MAKE) static

$(LIBLUA): $(LIBREADLINE)
	$(MAKE) -C $(LIBLUA_DIR)/src/ $(LUA_TARGET) CPPFLAGS=-I$(PWD) MYLDFLAGS=-L$(LIBREADLINE_DIR) MYLIBS=-ltermcap a

$(LIBJPEG):
	cd $(LIBJPEG_DIR)/ && ./configure --disable-shared && $(MAKE)

$(BIN): src/main.o src/mesh.o src/view.o src/my_lua.o src/utils.o src/globals.o src/lua_gl.o src/lua_glu.o src/lua_glut.o src/image.o
	mkdir -p bin/
	$(CC) $(CPPFLAGSLGS) $(CFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)

clean:
	$(RM) -r src/*.o bin/
