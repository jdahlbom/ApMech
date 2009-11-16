#include "MainMenuState.h"

#include <iostream>
#include <string>

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "GameStateManager.h"
#include "Gui.h"
#include "MenuStateViewport.h"
#include "types.h"
#include "functions.h"
#include "net/netdata.h"

namespace ap {

MainMenuState::MainMenuState( GameStateManager *gameStateManager,
			Ogre::SceneManager *sceneManager,
			ap::Gui *gui) :
  pGui(gui),
  pSceneManager(sceneManager)
{
    initStateManager(gameStateManager);
}

MainMenuState::~MainMenuState() {}

void MainMenuState::enter( void ) {
  createGUIWindow();
}

void MainMenuState::exit( void ) {
  terminateGUIWindow();
}

//-----------------------------------------------------------------------------

void MainMenuState::createGUIWindow()
{
  assert(pGui);

  pGui->setupMainMenuWindow();
  pGui->setMainMenuReceiver(this);

  assert(pSceneManager);
  mViewport = new MenuStateViewport(pSceneManager);
}

void MainMenuState::terminateGUIWindow()
{
  assert(pGui);
  pGui->hideMainMenuWindow();
  delete(mViewport);
}


bool MainMenuState::keyPressed( const ap::ooinput::KeyEvent &e ) {
  if (pGui->keyPressed(e)) {
    return true;
  }

  switch( e.key ) {
   case ooinput::AP_K_ESCAPE:
    this->requestShutdown();
    return 1;
   default:
    break;      // Some other key: do nothing
  }
  return false;
}

bool MainMenuState::keyReleased( const ap::ooinput::KeyEvent &e ) {
  if(pGui->keyReleased(e)) {
    return true;
  }
  return false;
}

bool MainMenuState::mousePressed(const ap::ooinput::MouseClickedEvent &e)
{
  return pGui->mousePressed(e);
}

bool MainMenuState::mouseReleased(const ap::ooinput::MouseClickedEvent &e)
{
  return pGui->mouseReleased(e);
}

bool MainMenuState::mouseMoved(const ap::ooinput::MouseMovedEvent &e)
{
  return pGui->mouseMoved(e);
}

// ----GuiMainMenuReceiver ------
  void MainMenuState::receiveQuitGame() {
    requestShutdown();
  }

  void MainMenuState::receiveConfigureKeyboard() {
    this->getStateManager()->enterKeyConfMenu();
  }

  void MainMenuState::receivePlayGame() {
    this->getStateManager()->enterLoginMenu();
  }

} // namespace ap
