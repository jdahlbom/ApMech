## Compiler and linker settings
INCLUDES = lib/
DEFINES = -g
LIBS = OGRE CEGUI-OGRE CEGUI OIS
CXX = gcc
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(DEFINES) -I$(INCLUDES)
LD = gcc
LDFLAGS = $(shell pkg-config --libs $(LIBS))
SRCDIR = src
OBJDIR = obj
APP_NAME = apmech

## Files to include in yourApp project
APPLICATION_FILES = apmech.o gameworld.o mech.o

## Compilation and linking occurs here
all: app_files
	$(LD) $(LDFLAGS) $(APPLICATION_FILES) -o $(APP_NAME)

app_files: $(APPLICATION_FILES)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $(OBJDIR)/$@ $< $(CXXFLAGS)


## Clean up
clean:
	-rm -f $(OBJDIR)/* $(APP_NAME)