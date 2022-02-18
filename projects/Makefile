PROJECT_DIR = ..
LIB_DIR = $(PROJECT_DIR)/lib
LIBJPEG_DIR = $(LIB_DIR)/jpeg
LIBJPEG = $(LIBJPEG_DIR)/.libs/libjpeg.a
LIBLUA_DIR = $(LIB_DIR)/lua
LIBLUA = $(LIBLUA_DIR)/src/liblua.a

OS_NAME = $(shell uname)

LUA_TARGET =
GL_LDLIBS =
ifeq ($(OS_NAME),Darwin)
LUA_TARGET := macosx
GL_LDLIBS := -framework GLUT -framework OpenGL -framework Cocoa
else ifeq ($(OS_NAME),Linux)
LUA_TARGET := linux
GL_LDLIBS := -lglut -lGL -lGLU
endif

INC_DIR = $(PROJECT_DIR)/include
SRC_DIR = $(PROJECT_DIR)/src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
BIN_DIR = $(PROJECT_DIR)/bin
BIN = $(BIN_DIR)/b3d

INC = -I$(LIBLUA_DIR)/src -I$(LIBJPEG_DIR) -I$(INC_DIR)

CPPFLAGS = $(INC) $(MY_CPPFLAGS)
CFLAGS ?= -Ofast -funroll-loops -flto
LOADLIBES = -L$(LIBLUA_DIR)/src -L$(LIBJPEG_DIR)/.libs
LDLIBS = -llua -ldl -lm -ljpeg $(GL_LDLIBS)

.PHONY: all run clean distclean

all: $(LIBLUA) $(LIBJPEG) $(BIN)

$(LIBLUA):
	cd $(LIBLUA_DIR); $(MAKE) $(LUA_TARGET)

$(LIBJPEG):
	cd $(LIBJPEG_DIR)/ && ./configure --disable-shared && $(MAKE)

$(BIN): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGSLGS) $(CFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)

run: all
	$(BIN)

clean:
	$(RM) -r $(OBJS) $(BIN_DIR)

distclean: clean
	cd $(LIBLUA_DIR); $(MAKE) clean
	cd $(LIBJPEG_DIR); $(MAKE) distclean
