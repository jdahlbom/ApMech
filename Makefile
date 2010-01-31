## Compiler and linker settings
ARCH = $(shell uname -m)
INCLUDES = lib/
DEFINES = -g
LIBS = OGRE CEGUI-OGRE CEGUI
CXX = g++
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(DEFINES) -I$(INCLUDES) $(shell sdl-config --cflags) -I/usr/local/boost_1_39_0 -DSERIALIZER_WITH_OGRE -Wall
LD = gcc
LDFLAGS = $(shell pkg-config --libs $(LIBS)) $(shell sdl-config --libs) -lSDL_image -lenet -lboost_filesystem-mt -lexpat
SERVERLDFLAGS = $(shell pkg-config --libs OGRE) -lSDL_image -lenet -lboost_filesystem-mt -lexpat
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
	CombinedControls.o Gui.o LoginState.o LimboState.o ScoreListing.o \
	ObjectDataBase.o ObjectReader.o GameWorld.o TerrainHeightMap.o TurretControl.o ActionKeyMap.o \
	MainMenuState.o MenuStateViewport.o KeyConfMenuState.o MechData.o
SERVEROBJS = servermain.o utilities.o Server.o MovingObject.o RectBoundaries.o serializer.o netdata.o \
	functions.o NetUser.o NetEvent.o NetMessage.o Mech.o Projectile.o rotations.o \
	MovableControl.o WeaponControl.o TurretControl.o CombinedControls.o \
	ScoreListing.o ObjectDataBase.o GameWorld.o TerrainHeightMap.o \
	MechData.o ObjectReader.o MatchRules.o

## Compilation and linking occurs here
all: tempDirectories application server

tempDirectories:
	- mkdir -p $(OBJDIR)

application: $(OBJS)
	$(LD) $(OBJS:%=$(OBJDIR)/%) -o $(BIN) $(LDFLAGS)

server: $(SERVEROBJS)
	$(LD) $(SERVEROBJS:%=$(OBJDIR)/%) -o apserver $(SERVERLDFLAGS)

%.o: %.cpp
	$(CXX) -c $^ -o $(OBJDIR)/$@ $(CXXFLAGS)

rebuild: clean all

## Clean up
clean:
	rm -R -f $(OBJDIR)
	- rm -f $(BIN) apserver
	- rm -f Ogre.log CEGUI.log
