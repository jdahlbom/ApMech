## Compiler and linker settings
CXX = g++
LD = g++
CXXFLAGS = $(shell sdl-config --cflags) -g
LDFLAGS = $(shell sdl-config --libs) -L/usr/local/lib -lenet -lSDL_ttf
SRCDIR = src
OBJDIR = obj
OBJS = main.o servermain.o functions.o NablaControl.o \
	location.o netgameobject.o projectile.o starfield.o
NETOBJS = NetEvent.o NetMessage.o NetUser.o serializer.o netdata.o

vpath %.h src
vpath %.h src/net
vpath %.cpp src
vpath %.cpp src/net

## Compilation and linking occurs here
all: nablawing

nablawing: $(OBJS) $(NETOBJS)
	$(LD) -o nablawing $(OBJDIR)/*.o $(LDFLAGS)

#%.o: $(SRCDIR)/%.cpp
%.o: %.cpp
	mkdir -p $(OBJDIR); \
	$(CXX) -c $^ -o $(OBJDIR)/$@ $(CXXFLAGS)
##	$(CXX) -c -o $(OBJDIR)/$@ $< $(CXXFLAGS)


## Clean up
clean:
	rm -rf $(OBJDIR)
distclean:
	rm -rf $(OBJDIR) nablawing
