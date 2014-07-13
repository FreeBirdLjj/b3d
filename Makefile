JPEG_VER = 9a
LIBJPEG_DIR = jpeg-$(JPEG_VER)
LIBJPEG = $(LIBJPEG_DIR)/.libs/libjpeg.a
LIBLUA_VER = 5.2.3
LIBLUA_DIR = lua-$(LIBLUA_VER)
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

CPPFLAGS = -DDEBUG -I$(LIBLUA_DIR)/src -I$(LIBJPEG_DIR)
CFLAGS = -g -O3
LOADLIBES = -L$(LIBLUA_DIR)/src -L$(LIBJPEG_DIR)/.libs
LDLIBS = -llua -ldl -lm -ljpeg $(GLLIBS)

BIN = bin/b3d

.PHONY: all clean

all: $(LIBLUA) $(LIBJPEG) $(BIN)

$(LIBLUA):
	cd $(LIBLUA_DIR)/src/ && make $(LUA_TARGET)

$(LIBJPEG):
	cd $(LIBJPEG_DIR)/ && ./configure --disable-shared && make

$(BIN): src/main.o src/mesh.o src/view.o src/my_lua.o src/utils.o src/globals.o src/lua_gl.o src/lua_glu.o src/lua_glut.o src/image.o
	mkdir -p bin/
	$(CC) $(CPPFLAGSLGS) $(CFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)

clean:
	-rm -fr src/*.o $(BIN)
