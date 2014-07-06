JPEG_VER = 9a
LIBJPEG_DIR = jpeg-$(JPEG_VER)
LIBJPEG = $(LIBJPEG_DIR)/.libs/libjpeg.a
LIBLUA_VER = 5.2.3
LIBLUA_DIR = lua-$(LIBLUA_VER)
LIBLUA = $(LIBLUA_DIR)/src/liblua.a

CPPFLAGS = -I$(LIBLUA_DIR)/src -I$(LIBJPEG_DIR)
CFLAGS = -g -O3
LDLIBS = -L$(LIBLUA_DIR)/src -L$(LIBJPEG_DIR)/.libs -llua -lm -ljpeg -lglut -lGL -lGLU -ldl

BIN = bin/b3d

.PHONY: all
all: $(LIBLUA) $(LIBJPEG) $(BIN)

$(LIBLUA):
	cd $(LIBLUA_DIR)/src/; make linux

$(LIBJPEG):
	cd $(LIBJPEG_DIR)/; make

$(BIN): src/main.o src/mesh.o src/view.o src/my_lua.o src/utils.o src/globals.o src/lua_gl.o src/lua_glu.o src/lua_glut.o src/image.o
	@mkdir -p bin/
	$(CC) $(CPPFLAGSLGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	-rm -fr src/*.o $(BIN)
