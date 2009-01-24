## Compiler and linker settings
CXX = g++
LD = g++
CXXFLAGS = $(shell sdl-config --cflags) -g
LDFLAGS = $(shell sdl-config --libs) -L/usr/local/lib -lenet
SRCDIR = src
OBJDIR = obj
OBJS = netdata.o main.o servermain.o functions.o netuser.o \
	netgameobject.o location.o projectile.o starfield.o

## Compilation and linking occurs here
all: netdata

netdata: $(OBJS)
	$(LD) -o netdata $(OBJDIR)/*.o $(LDFLAGS)

%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR); \
	$(CXX) -c -o $(OBJDIR)/$@ $< $(CXXFLAGS)


## Clean up
clean:
	rm -rf $(OBJDIR)
distclean:
	rm -rf $(OBJDIR) netdata
