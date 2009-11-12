#include "LoginState.h"

#include <iostream>
#include <string>

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "GameStateManager.h"
#include "Gui.h"
#include "types.h"
#include "functions.h"
#include "net/netdata.h"

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
  float colorSelection = 0.0f;
  Ogre::ConfigFile loginConfig;
  try {
      loginConfig.load(bundlePath() + "login.cfg");
      ipAddress = loginConfig.getSetting("IPaddress");
      playerName = loginConfig.getSetting("PlayerName");
      if (!from_string<float>(colorSelection, loginConfig.getSetting("PlayerColor"), std::dec))
        colorSelection = 0.0f;
  } catch (Ogre::FileNotFoundException e) {}

  pGui->setupLoginWindow(ipAddress, playerName, colorSelection);
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
   default:
    break;      // Some other key: do nothing
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
    int ret;
    net::NetData *netdata = NULL;
    // First, save our login settings, overwriting the previous file
    std::ofstream lastLoginSettings("login.cfg", std::ios::trunc);
    lastLoginSettings << "IPaddress="<<ipAddress<<std::endl;
    lastLoginSettings << "PlayerName="<<playerName<<std::endl;
    lastLoginSettings << "PlayerColor="<<pGui->getColorSliderValue()<<std::endl;
    lastLoginSettings.close();

    // attempt to actually connect to the server
    netdata = new net::NetData(net::NetData::CLIENT);

    pGui->addConsoleItem("Connecting to server " + ipAddress);

    ret = netdata->connect(ipAddress, 50740);

    switch(ret) {
        case 1:
            // success
            pGui->addConsoleItem("Connection to server succeeded");
            break;
        default:
            std::stringstream out;
            out << ret;
            pGui->addConsoleItem("Connection to server failed: " + out.str());
            return;
    }

    netdata->me.nick = playerName;
    netdata->me.color = getColorFromPseudoHue(pGui->getColorSliderValue());
    netdata->me.changed = true;         // Mark this info for transmission
    netdata->sendChanges();             // Send changes without waiting!

    // Then, login.

    getStateManager()->loginToGame(netdata);
  }

void LoginState::requestQuit()
{
  requestShutdown();
}

} // namespace ap
