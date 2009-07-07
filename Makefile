## Compiler and linker settings
INCLUDES = lib/
DEFINES = -g
LIBS = OGRE CEGUI-OGRE CEGUI
CXX = g++
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(DEFINES) -I$(INCLUDES) $(shell sdl-config --cflags) -I/usr/local/boost_1_39_0 -Wall
LD = g++
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
vpath %.cpp src
vpath %.cpp src/ooinput
vpath %.cpp src/net

## Files to include in yourApp project

OBJS = InputSystem.o SDLInputSystem.o GameState.o PlayState.o GameStateManager.o main.o MovingObject.o serializer.o RectBoundaries.o netdata.o functions.o NetUser.o NetEvent.o Mech.o Projectile.o

## Compilation and linking occurs here
all: tempDirectories application



tempDirectories:
	- mkdir -p $(OBJDIR)

application: $(OBJS)
	$(LD) $(OBJDIR)/*.o -o $(BIN) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $^ -o $(OBJDIR)/$@ $(CXXFLAGS)


## Clean up
clean:
	rm -R -f $(OBJDIR)
	- rm $(BIN)
	- rm Ogre.log CEGUI.log
