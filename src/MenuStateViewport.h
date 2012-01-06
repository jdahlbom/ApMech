#ifndef AP_MENUSTATEVIEWPORT_H
#define AP_MENUSTATEVIEWPORT_H

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

namespace ap {

  /**
   * Provides easy plug-in menu state camera and viewport management.
   *
   * Instead of having every state duplicate the method or inherit
   * common functionality, use a has-a relationship to this functionality
   * class.
   *
   * Usage: Create with a valid scene manager upon entering a class.
   *        Delete upon leaving a class.
   */
  class MenuStateViewport {
  public:
    MenuStateViewport(Ogre::SceneManager *sceneMgr);
    ~MenuStateViewport();

  private:
    Ogre::Camera       *mCamera;
    Ogre::SceneManager *pSceneMgr;
    Ogre::Viewport     *mMenuViewport;
  };
} // namespace ap


#endif
