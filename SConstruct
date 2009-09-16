import os
import sys

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
    env.Append(LIBS = ['enet'])
    env.Append(CCFLAGS = '-include Carbon/Carbon.h')
    env.Append(LINKFLAGS = '-F' + ogresdkLocation + '/build/Release -F' + ogresdkLocation + '/Dependencies')

elif platform == 'posix':
    env = Environment()
    env['CPPPATH'] = '/usr/local/boost_1_39_0'
    env.ParseConfig('pkg-config --cflags --libs OGRE CEGUI-OGRE CEGUI')
    env.ParseConfig('pkg-config --libs enet')

env.ParseConfig('sdl-config --cflags')
env.ParseConfig('sdl-config --libs')

env['CPPFLAGS'] = ["-g"]

print "Building Achtung Panzer Mech for platform '" + env['PLATFORM'] +"'..."

commonSources = Split("""
src/MovingObject.cpp
src/CombinedControls.cpp
src/MovableControl.cpp
src/WeaponControl.cpp
src/RectBoundaries.cpp
src/net/serializer.cpp
src/net/netdata.cpp
src/net/NetUser.cpp
src/net/NetEvent.cpp
src/net/NetMessage.cpp
src/functions.cpp
src/Mech.cpp
src/Projectile.cpp
src/math/rotations.cpp
""")

clientSources = Split("""
src/GameState.cpp
src/PlayState.cpp
src/GameStateManager.cpp
src/main.cpp
src/ooinput/InputSystem.cpp
src/ooinput/SDLInputSystem.cpp
""")+commonSources

serverSources = Split("""
src/servermain.cpp
src/server/Server.cpp
""")+commonSources

crudeSources = Split("""
src/GameState.cpp
src/PlayState.cpp
src/GameStateManager.cpp
src/SDLGameStateManager.cpp
src/mainSDL.cpp
src/ooinput/InputSystem.cpp
src/ooinput/SDLInputSystem.cpp
""")+commonSources

client = env.Program(target='apmech', source=clientSources)
server = env.Program(target='apserver', source=serverSources)
crudeClient = env.Program(target='apcrude', source=crudeSources)

# the binaries to be built by default
Default(client)
Default(server)
Default(crudeClient)

# make the OS X application bundle
if platform == 'darwin':
    dir = env.MakeBundle('APMech', client, info_plist='osx/Info.plist', typecode='APPL', signature='apmech', resources=['Media', 'osx/plugins.cfg', 'osx/resources.cfg', 'osx/terrain.cfg', 'osx/ogre.cfg'])
    env.Alias('install', dir)
