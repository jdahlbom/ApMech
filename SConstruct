import os
import sys
#import glob

# Helper function for windows libs..
# Search expression means "a string that is found in the file name". Mostly "_d.lib"
def findLibs(dir, searchExpression):
	libArr = []
	for file in os.listdir(dir):
		if file.find(searchExpression) != -1:
			parts = file.split('.')
			libArr.append(parts[0])

	return libArr

# Setup some of our requirements

platform = Platform().name

if platform == 'darwin':

    # Ogre SDK location is given as a parameter to the build script:
    #
    # scons ogre="/path/to/ogreSDK"
    
    ogresdkLocation = ARGUMENTS.get('ogre')

    # if it's not defined, here's a good default value ;-)
    if not ogresdkLocation:
        ogresdkLocation = '/Users/ipuustin/Desktop/OgreSDK'
    
    tools = ["default", "gcc", "g++"]
    import bundlemacosx
    tools.append (bundlemacosx.TOOL_BUNDLE)
    env = Environment(tools=tools)

    env['FRAMEWORKS'] = ['Carbon', 'System', 'Ogre', 'CEGUI', 'Cg', 'OgreCEGUIRenderer']
    env['CPPPATH'] = [
            ogresdkLocation + '/Dependencies/Ogre.framework/Headers',
            ogresdkLocation + '/Dependencies/Cg.framework/Headers',
            ogresdkLocation + '/Dependencies/CEGUI.framework/Headers',
            ogresdkLocation + '/Samples/include',
            '/opt/local/include/boost-1_35/'
        ]
    env['CPPDEFINES'] = ['SERIALIZER_WITH_OGRE']
    env.Append(LIBS = ['enet'])
    env.Append(CCFLAGS = '-include Carbon/Carbon.h')
    env.Append(LINKFLAGS = '-F' + ogresdkLocation + '/build/Release -F' + ogresdkLocation + '/Dependencies')

elif platform == 'posix':
    env = Environment()
    env['CPPPATH'] = '/usr/local/boost_1_39_0'
    env['CPPDEFINES'] = ['SERIALIZER_WITH_OGRE']
    env.ParseConfig('pkg-config --cflags --libs OGRE CEGUI-OGRE CEGUI SDL_image')
    env.ParseConfig('pkg-config --libs enet')

elif platform == 'win32':
	env = Environment()
	#AR taken from http://www.scons.org/wiki/MicrosoftPlatformTool
	#Doesn't look like there is any documentation about it..
	sdkPath = "D:\\Program Files\\Microsoft Visual Studio 9.0\\VC"
	env['AR'] = '"'+ sdkPath +'\\bin\\lib.exe"'
	env['LINK'] = '"'+ sdkPath +'\\bin\\link.exe"'
	
	cpppath = Split("""
	C:\\devel\\apmech_ogre-1_6\\OgreMain\\include
	C:\\devel\\MS_SDKs_Windows_v6.0A\\Include
	C:\\devel\\cegui
	C:\\devel\\cegui\\ogreceguirenderer\\include
	C:\\devel\\enet\\include
	C:\devel\SDL-1.2.13\include
	""")
	env['CPPPATH'] = cpppath

	# Some of these libraries are compiled to DEBUG and RELEASE versions separately.
	libVersionedPath = Split("""
	C:\\devel\\apmech_ogre-1_6\\lib
	C:\\devel\\cegui\\lib
	""")
	
	libOtherPath = Split("""
	C:\\devel\\enet\\lib
	C:\\devel\\SDL-1.2.13\\lib
	""")
	
	libWinPath = Split("""
	C:\\devel\\MS_SDKs_Windows_v6.0A\\lib
	""")
	
	libCeguiDepPath = ['C:\\devel\\cegui\\cegui_mk2_0_6\\dependencies\\lib']
	
	env.Append(LIBPATH = libVersionedPath + libOtherPath + libCeguiDepPath + libWinPath)
	
	libs = []
	for dir in libVersionedPath:
		libs = libs + findLibs(dir, '_d.lib')
	for dir in libOtherPath:
		libs = libs + findLibs(dir, '.lib')
	for dir in libCeguiDepPath:
		libs = libs + findLibs(dir, '_d.lib')
	
	#for dir in libWinPath:
	#	libs = libs + findLibs(dir, '.lib')
	#	libs = libs + findLibs(dir, '.Lib')
	
	
	#CEGUI dependencies withoug _d.lib suffices
	libs.append('DevIL')
	libs.append('ILU')
	libs.append('ILUT')
	libs.append('xerces-c_3D')
	
	libsMain = Split("""
	enet
	SDL
	""")
	libsAll = libsMain + libs
	#print libsAll
	#sys.exit(0)
	
	env.Append(LIBS = libsAll)
	env.Append(CCFLAGS = '-DWIN32  /EHsc')

	
if platform == 'posix' or platform == 'darwin':
	env.ParseConfig('sdl-config --cflags')
	env.ParseConfig('sdl-config --libs')
	env.Append(LIBS = ['SDL_image'])
	
	env['CPPFLAGS'] = ["-g"]
# elif platform == 'win32':
	# Find sdl somehow?
	


print "Building Achtung Panzer Mech for platform '" + env['PLATFORM'] +"'..."

commonSources = Split("""
src/GameWorld.cpp
src/MovingObject.cpp
src/CombinedControls.cpp
src/MovableControl.cpp
src/WeaponControl.cpp
src/ObjectDataModel.cpp
src/RectBoundaries.cpp
src/ScoreListing.cpp
src/net/serializer.cpp
src/net/netdata.cpp
src/net/NetUser.cpp
src/net/NetEvent.cpp
src/net/NetMessage.cpp
src/functions.cpp
src/Mech.cpp
src/Projectile.cpp
src/math/rotations.cpp
src/TerrainHeightMap.cpp
""")

clientSources = Split("""
src/ActionKeyMap.cpp
src/GameState.cpp
src/LimboState.cpp
src/LoginState.cpp
src/PlayState.cpp
src/GameStateManager.cpp
src/Gui.cpp
src/main.cpp
src/MainMenuState.cpp
src/MenuStateViewport.cpp
src/ooinput/InputSystem.cpp
src/ooinput/SDLInputSystem.cpp
""")+commonSources

serverSources = Split("""
src/servermain.cpp
src/server/Server.cpp
""")+commonSources

client = env.Program(target='apmech', source=clientSources)
server = env.Program(target='apserver', source=serverSources)

# the binaries to be built by default
Default(client)
Default(server)

# make the OS X application bundle
if platform == 'darwin':
    dir = env.MakeBundle('APMech', client, info_plist='osx/Info.plist', typecode='APPL', signature='apmech', resources=['Media', 'osx/plugins.cfg', 'osx/resources.cfg', 'osx/terrain.cfg', 'osx/ogre.cfg'])
    env.Alias('install', dir)
