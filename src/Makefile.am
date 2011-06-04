SUBDIRS = math ooinput net server .
bin_PROGRAMS = apmech apserver

MY_COMMONSOURCES = GameWorld.cpp \
		MovingObject.cpp \
		Mech.cpp \
		Projectile.cpp \
		RectBoundaries.cpp \
		ScoreListing.cpp \
		CombinedControls.cpp \
		TurretControl.cpp \
		WeaponControl.cpp \
		MovableControl.cpp \
		MechData.cpp \
		TerrainHeightMap.cpp \
		functions.cpp

apmech_SOURCES = GameState.cpp \
		GameStateManager.cpp \
		LimboState.cpp \
		LoginState.cpp \
		PlayState.cpp \
		Gui.cpp \
		KeyConfMenuState.cpp \
		ActionKeyMap.cpp \
		MainMenuState.cpp \
		MenuStateViewport.cpp \
		main.cpp \
		${MY_COMMONSOURCES}

apmech_CXXFLAGS = -DSERIALIZER_WITH_OGRE \
		${OGRE_CFLAGS} \
		${CEGUI_CFLAGS} \
		${CEGUI_OGRE_CFLAGS} \
		${SDL_CFLAGS} \
		${SDLIMAGE_CFLAGS} \
		${ENET_CFLAGS}

apmech_LDADD =  math/libapmath.a \
		ooinput/libapinput.a \
		net/libapnet.a \
		${OGRE_LIBS} \
		${CEGUI_LIBS} \
		${CEGUI_OGRE_LIBS} \
		${SDL_LIBS} \
		${ENET_LIBS} \
		${SDLIMAGE_LIBS} \
		-lboost_filesystem \
		-lboost_system

apserver_SOURCES = ObjectDataBase.cpp \
		ObjectReader.cpp \
		servermain.cpp \
		${MY_COMMONSOURCES}

apserver_CXXFLAGS = -DSERIALIZER_WITH_OGRE \
		${OGRE_CFLAGS} \
		${SDLIMAGE_CFLAGS} \
		${ENET_CFLAGS}

apserver_LDADD = math/libapmath.a \
		net/libapnet.a \
	 	server/libapserver.a \
		${OGRE_LIBS} \
		${SDLIMAGE_LIBS} \
		${ENET_LIBS} \
		-lexpat \
		-lboost_filesystem \
		-lboost_system

