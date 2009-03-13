
LIBS = CEGUI
INCLUDES = /usr/include/SDL
DEFINES = -g
CXXFLAGS = ${shell pkg-config --cflags ${LIBS}} ${DEFINES} -I${INCLUDES} -Wall
LDFLAGS = ${shell pkg-config --libs CEGUI} ${shell sdl-config --libs SDL} -lGL -lGLU -lCEGUIOpenGLRenderer 
BIN = jonnin

all:
	gcc ${CXXFLAGS} -c main.cpp
	gcc ${LDFLAGS} main.o -o jonnin

clean:
	rm -f *.o
	rm -f ${BIN}