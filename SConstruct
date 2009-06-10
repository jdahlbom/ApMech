import os
import sys

# Setup some of our requirements

env = Environment()

print "Building Achtung Panzer Mech for platform '" + env['PLATFORM'] +"'"

if env['PLATFORM'] == 'darwin':
    env['FRAMEWORKS'] = ['Carbon', 'System', 'Ogre', 'CEGUI', 'Cg', 'OgreCEGUIRenderer']
    env['CPPPATH'] = [
            '/System/Library/Frameworks/Ogre.framework/Headers',
            '/System/Library/Frameworks/CEGUI.framework/Headers',
            '/System/Library/Frameworks/Cg.framework/Headers',
            '/Users/ipuustin/Desktop/OgreSDK/Samples/include',
            '/opt/local/include/boost-1_35/'
        ]
elif env['PLATFORM'] == 'posix':
    env['CPPPATH'] = '/usr/local/boost_1_39_0'
    env.ParseConfig('pkg-config --cflags --libs OGRE CEGUI-OGRE CEGUI')

env.ParseConfig('sdl-config --cflags')
env.ParseConfig('sdl-config --libs')

sources = Split("""
src/GameState.cpp
src/PlayState.cpp
src/MovingObject.cpp
src/GameStateManager.cpp
src/main.cpp
src/ooinput/InputSystem.cpp
src/ooinput/TestListener.cpp
src/ooinput/SDLInputSystem.cpp
""")

t = env.Program(target='apmech', source=sources)
Default(t)
