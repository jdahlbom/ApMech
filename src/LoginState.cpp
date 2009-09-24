#include "LoginState.h"

#include <iostream>
#include <string>

#include <Ogre.h>

#include "GameStateManager.h"
#include "Gui.h"
#include "types.h"

namespace ap {

LoginState::LoginState( GameStateManager *gameStateManager,
			Ogre::SceneManager *sceneManager,
		      ap::Gui *gui) :
  pGui(gui),
  pSceneManager(sceneManager)
{
    initStateManager(gameStateManager);
}

LoginState::~LoginState() {}

void LoginState::enter( void ) {
  createGUIWindow();
}

void LoginState::exit( void ) {
  terminateGUIWindow();
}

//-----------------------------------------------------------------------------

void LoginState::createGUIWindow()
{
  assert(pGui);

    // Load the previous login settings from login.cfg, if one exists
  std::string ipAddress(""), playerName("nameless newbie");
  Ogre::ConfigFile loginConfig;
  try {
      loginConfig.load("login.cfg");
      ipAddress = loginConfig.getSetting("IPaddress");
      playerName = loginConfig.getSetting("PlayerName");
  } catch (Ogre::FileNotFoundException e) {}

  pGui->setupLoginWindow(ipAddress, playerName);
  pGui->setLoginReceiver(this);
  setupOgreViewport(pSceneManager);
}

void LoginState::terminateGUIWindow()
{
  assert(pGui);
  pGui->exitLoginWindow();
  removeCamera(pSceneManager);
}

  void LoginState::setupOgreViewport(Ogre::SceneManager *sceneMgr)
{
  Ogre::Root *pRoot = Ogre::Root::getSingletonPtr();
  mCamera = sceneMgr->createCamera( "LoginCamera" );

  // REQUIRES for the RenderTarget named "ApMech" to exist..
  Ogre::RenderTarget *renderWindow = pRoot->getRenderTarget("ApMech");
  assert(renderWindow);

  Ogre::ColourValue bgColor(0.0f, 0.0f, 0.0f);
  renderWindow->addViewport(mCamera)->setBackgroundColour(bgColor);
}

  void LoginState::removeCamera(Ogre::SceneManager *sceneMgr)
  {
    sceneMgr->destroyCamera(mCamera);

    Ogre::Root *pRoot = Ogre::Root::getSingletonPtr();
    pRoot->getRenderTarget("ApMech")->removeAllViewports();
  }

bool LoginState::keyPressed( const ap::ooinput::KeyEvent &e ) {
  if (pGui->keyPressed(e)) {
    return 1;
  }

  switch( e.key ) {
  case ooinput::AP_K_ESCAPE:
    this->requestShutdown();
    return 1;
  }
  return 0;
}

bool LoginState::keyReleased( const ap::ooinput::KeyEvent &e ) {
  if(pGui->keyReleased(e)) {
    return 1;
  }
  return 0;
}

  bool LoginState::mousePressed(const ap::ooinput::MouseClickedEvent &e)
  {
    return pGui->mousePressed(e);
  }

  bool LoginState::mouseReleased(const ap::ooinput::MouseClickedEvent &e)
  {
    return pGui->mouseReleased(e);
  }

  bool LoginState::mouseMoved(const ap::ooinput::MouseMovedEvent &e)
  {
    return pGui->mouseMoved(e);
  }

void LoginState::attemptLogin(const std::string &ipAddress, const std::string &playerName)
  {
    // First, save our login settings, overwriting the previous file
    std::ofstream lastLoginSettings("login.cfg", std::ios::trunc);
    lastLoginSettings << "IPaddress="<<ipAddress<<std::endl;
    lastLoginSettings << "PlayerName="<<playerName<<std::endl;
    lastLoginSettings.close();
    // Then, login.
    getStateManager()->loginToGame(ipAddress, playerName);
  }

void LoginState::requestQuit()
{
  requestShutdown();
}

} // namespace ap
