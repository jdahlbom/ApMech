/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#ifndef AP_GAMESTATE_H
#define AP_GAMESTATE_H

#include "ooinput/KeyboardListener.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

namespace ap {

class GameStateManager;

class GameState: public ooinput::KeyboardListener{
public:
    virtual ~GameState( void ) { }

    void initStateManager(GameStateManager *gameStateManager);

    virtual void enter( void )  = 0;
    virtual void exit( void )   = 0;

    virtual void pause( void )  = 0;
    virtual void resume( void ) = 0;
    virtual void update( unsigned long lTimeElapsed ) = 0;

    virtual bool keyPressed( const ooinput::KeyEvent &e )  = 0;
    virtual bool keyReleased( const ooinput::KeyEvent &e ) = 0;

    void changeState( GameState *state );
    void pushState( GameState *state );
    void popState( void );
    void requestShutdown( void );

protected:
    GameStateManager *pGameStateManager;

};

} // namespace ap

#endif
