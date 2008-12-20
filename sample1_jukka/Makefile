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

BIN = SampleApp

## Files to include in yourApp project
OBJS = SampleApp.o SampleFrameListener.o

## Compilation and linking occurs here
all: tempDirectories application
	$(LD) $(LDFLAGS) $(OBJDIR)/*.o -o $(BIN)

tempDirectories:
	- mkdir $(OBJDIR)

application: $(OBJS)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $(OBJDIR)/$@ $< $(CXXFLAGS)


## Clean up
clean:
	rm -R -f $(OBJDIR)
	- rm $(BIN)