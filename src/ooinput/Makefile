## Compiler and linker settings
INCLUDES = lib/
DEFINES = -g
LIBS = OGRE
CXX = gcc
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(shell sdl-config --cflags) $(DEFINES) -I$(INCLUDES) -Wall
LD = gcc
LDFLAGS = $(shell pkg-config --libs $(LIBS)) $(shell sdl-config --libs)
SRCDIR = .
OBJDIR = .

BIN = ooinput

## Files to include in yourApp project
OBJS = main.o InputSystem.o SDLInputSystem.o TestListener.o

## Compilation for use as a source subdirectory
all: 

## Compiation and linking for the input test application
ootest: tempDirectories application
	$(LD) $(LDFLAGS) $(OBJDIR)/*.o -o $(BIN)

tempDirectories:
	- mkdir $(OBJDIR)

application: $(OBJS)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $(OBJDIR)/$@ ${SRCDIR}/$< $(CXXFLAGS)


## Clean up
clean:
	rm -R -f $(OBJDIR)/*.o
	- rm $(BIN)
	- rm Ogre.log CEGUI.log
