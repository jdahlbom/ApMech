## Compiler and linker settings
INCLUDES = lib/
DEFINES = -g
LIBS = OGRE CEGUI-OGRE CEGUI
CXX = g++
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(DEFINES) -I$(INCLUDES) $(shell sdl-config --cflags) -I/usr/local/boost_1_39_0 -Wall
LD = gcc
LDFLAGS = $(shell pkg-config --libs $(LIBS)) $(shell sdl-config --libs)
OBJDIR = obj

## Files in subdirectories
OOINPUT_PATH = src/ooinput
OOINPUT_SRCS = $(OOINPUT_PATH)/InputSystem.cpp $(OOINPUT_PATH)/SDLInputSystem.cpp

BIN = apmech

## Files to compile
vpath %.h src
vpath %.h src/ooinput
vpath %.cpp src
vpath %.cpp src/ooinput

## Files to include in yourApp project
OBJS = InputSystem.o SDLInputSystem.o GameState.o PlayState.o GameStateManager.o main.o MovingObject.o

## Compilation and linking occurs here
all: tempDirectories application



tempDirectories:
	- mkdir $(OBJDIR)

application: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJDIR)/*.o -o $(BIN)

%.o: %.cpp
	$(CXX) -c $^ -o $(OBJDIR)/$@ $(CXXFLAGS)


## Clean up
clean:
	rm -R -f $(OBJDIR)
	- rm $(BIN)
	- rm Ogre.log CEGUI.log
