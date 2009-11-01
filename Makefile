## Compiler and linker settings
ARCH = $(shell uname -m)
INCLUDES = lib/
DEFINES = -g
LIBS = OGRE CEGUI-OGRE CEGUI
CXX = g++
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(DEFINES) -I$(INCLUDES) $(shell sdl-config --cflags) -I/usr/local/boost_1_39_0 -Wall
LD = gcc
LDFLAGS = $(shell pkg-config --libs $(LIBS)) $(shell sdl-config --libs) -lenet
OBJDIR = obj

## Files in subdirectories
OOINPUT_PATH = src/ooinput
OOINPUT_SRCS = $(OOINPUT_PATH)/InputSystem.cpp $(OOINPUT_PATH)/SDLInputSystem.cpp

BIN = apmech

## Files to compile
vpath %.h src
vpath %.h src/ooinput
vpath %.h src/net
vpath %.hpp src/net
vpath %.hpp src/math
vpath %.hpp src/server
vpath %.cpp src
vpath %.cpp src/ooinput
vpath %.cpp src/net
vpath %.cpp src/math
vpath %.cpp src/server
vpath %.o $(OBJDIR)

## Files to include in yourApp project

OBJS = InputSystem.o SDLInputSystem.o GameState.o PlayState.o GameStateManager.o main.o \
	MovingObject.o serializer.o RectBoundaries.o netdata.o functions.o NetUser.o NetEvent.o \
	NetMessage.o Mech.o Projectile.o rotations.o MovableControl.o WeaponControl.o \
	CombinedControls.o Gui.o LoginState.o ScoreListing.o ObjectDataModel.o
SERVEROBJS = servermain.o Server.o MovingObject.o RectBoundaries.o serializer.o netdata.o \
	functions.o NetUser.o NetEvent.o NetMessage.o Mech.o Projectile.o rotations.o \
	MovableControl.o WeaponControl.o CombinedControls.o ScoreListing.o

## Compilation and linking occurs here
all: tempDirectories application server

tempDirectories:
	- mkdir -p $(OBJDIR)

application: $(OBJS)
	$(LD) $(OBJS:%=$(OBJDIR)/%) -o $(BIN) $(LDFLAGS)

server: $(SERVEROBJS)
	$(LD) $(SERVEROBJS:%=$(OBJDIR)/%) -o apserver $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $^ -o $(OBJDIR)/$@ $(CXXFLAGS)

rebuild: clean all

## Clean up
clean:
	rm -R -f $(OBJDIR)
	- rm -f $(BIN) apserver
	- rm -f Ogre.log CEGUI.log
