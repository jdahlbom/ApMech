BIN=createmech
CXX=g++
CFLAGS=-Wall -g
LDFLAGS=

OBJFILES=generatedC.o stdsoap2.o main.o
OBJDIR=obj
OBJS = $(patsubst %,$(OBJDIR)/%,$(OBJFILES))

VPATH = src:generated:gsoap

all: ${BIN}

${OBJDIR}/%.o: %.cpp
	${CXX} -c -o $@ $< ${CFLAGS}

${BIN}: ${OBJS} 
	${CXX} -o $@ ${OBJS} ${LDFLAGS}

clean:
	rm -f ${OBJS} ${BIN}

generate:
	cd generated; make; cd ..
	
generated_clean:
	cd generated; make clean; cd ..
