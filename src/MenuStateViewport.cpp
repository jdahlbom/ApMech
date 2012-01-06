#include "MenuStateViewport.h"

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

namespace ap {

  MenuStateViewport::MenuStateViewport(Ogre::SceneManager *sceneMgr):
    mCamera(NULL),
    pSceneMgr(sceneMgr)
  {
    assert(pSceneMgr != NULL);
    Ogre::Root *pRoot = Ogre::Root::getSingletonPtr();
    assert(pRoot);

    mCamera = sceneMgr->createCamera( "MenuCamera" );
    
    // REQUIRES for the RenderTarget named "ApMech" to exist..
    Ogre::RenderTarget *renderWindow = pRoot->getRenderTarget("ApMech");
    assert(renderWindow);
    
    Ogre::ColourValue bgColor(0.0f, 0.0f, 0.0f);
    mMenuViewport = renderWindow->addViewport(mCamera);
    mMenuViewport->setBackgroundColour(bgColor);
  }
  MenuStateViewport::~MenuStateViewport()
  {
    pSceneMgr->destroyCamera(mCamera);

    Ogre::Root *pRoot = Ogre::Root::getSingletonPtr();
    assert(pRoot);
    pRoot->getRenderTarget("ApMech")->removeAllViewports();    

    // Ogre::Root::getSingletonPtr()->getRenderTarget("ApMech")->removeViewport(mMenuViewport->getZOrder());
  }
  
} // namespace ap
