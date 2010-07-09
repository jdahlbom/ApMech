noinst_LIBRARIES = libapnet.a
libapnet_a_SOURCES = netdata.cpp \
			NetEvent.cpp \
			NetMessage.cpp \
			NetUser.cpp \
			serializer.cpp

libapnet_a_CXXFLAGS = -DSERIALIZER_WITH_OGRE \
			${OGRE_CFLAGS} \
			${ENET_CFLAGS}
