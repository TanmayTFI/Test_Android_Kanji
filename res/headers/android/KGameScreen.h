/**
 * Kanji game engine - game screen and game scene definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KGAMESCREEN_H
#define  _KANJI_KGAMESCREEN_H

/* Include definitions */
#include "KObjectListable.h"
#include "KList.h"
#include "KWindow.h"
#include "KManagedGraphic.h"
#include "KManagedFont.h"
#include "KManagedSample.h"
#if defined(K_WIN32) || defined (K_MACOS)
#include "KManagedVideo.h"
#endif
#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
#include "KManagedModel.h"
#include "KManagedShader.h"
#endif
#include "KUIElement.h"

/* Forward definitions */
class KGameScreen;
class KSysThread;

/**
 * Game scene
 */

class KGameScene : public KObjectListable {
   friend class KGameScreen;

public:
   /** Constructor */
   KGameScene ();

   /** Destructor */
   ~KGameScene ();

   /**
    * Move this scene to the head of the list (rendered first, below the other scenes)
    */
   void moveToHead (void);

   /**
    * Move this scene to the tail of the list (rendered last, above the other scenes)
    */
   void moveToTail (void);

   /**
    * Move this scene before the specified scene (this scene will be rendered below the specified scene)
    *
    * \param lpBeforeScene scene to render above this scene
    */
   void moveBefore (KGameScene *lpBeforeScene);

   /**
    * Move this scene after the specified scene (this scene will be rendered above the specified scene)
    *
    * \param lpAfterScene scene to render below this scene
    */
   void moveAfter (KGameScene *lpAfterScene);

   /**
    * Get UI container element for this scene
    *
    * \return container
    */
   KUIElement *getUIContainer (void);

   /** Enable this scene (default) */
   void enable (void);

   /** Disable this scene. While disabled, the scene will not be updated or rendered */
   void disable (void);

   /** Update logic at a 10ms fixed rate */
   virtual void tick (void);

   /**
    * Update logic with a variable rate
    *
    * \param fElapsed number of milliseconds elapsed since last update
    */
   virtual void update (double fElapsed);

   /**
    * Render this scene. This method is called before rendering the UI elements for this scene
    */
   virtual void preRender (void);

   /**
    * Render this scene. This method is called after rendering the UI elements for this scene
    */
   virtual void render (void);

   /**
    * Get list of managed graphics
    *
    * \return list of managed graphics
    */
   KManagedGraphicList *getGraphics (void);

   /**
    * Get list of managed fonts
    *
    * \return list of managed fonts
    */
   KManagedFontList *getFonts (void);

   /**
    * Get list of managed sound effects
    *
    * \return list of managed sound effects
    */
   KManagedSampleList *getSamples (void);

#if defined(K_WIN32) || defined (K_MACOS)
   /**
    * Get list of managed videos
    *
    * \return list of managed videos
    */
   KManagedVideoList *getVideos (void);
#endif

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
   /**
    * Get list of managed shaders
    *
    * \return list of managed shaders
    */
   KManagedShaderList *getShaders (void);

   /**
    * Get list of managed 3d models
    *
    * \return list of managed 3d models
    */
   KManagedModelList *getModels (void);
#endif

protected:
   /**
    * Handle that this scene's resources need to be loaded
    */
   virtual void onLoad (void);

   /**
    * Handle that this scene's resources need to be uploaded to VRAM
    */
   virtual void onUpload (void);

   /**
    * Handle that this scene's resources need to be loaded
    */
   virtual void onUnload (void);

   /**
    * Handle that this scene just started displaying
    */
   virtual void onSceneEntered (void);

   /**
    * Handle that this scene just stopped displaying
    */
   virtual void onSceneExited (void);

   /** Game window */
   KWindow *_lpKWindow;

private:
   /** Game screen that this scene is currently a part of */
   KGameScreen *_lpParentScreen;

   /** Container for the elements of this scene */ 
   KUIElement *_lpSceneUIContainer;

   /** true if this scene is currently disabled */
   bool _bDisabled;
};

/** Game screen asset loading state */
enum {
   K_GAMESCREEN_IDLE,            /**< Not loaded yet */
   K_GAMESCREEN_LOADING,         /**< Currently loading in the background */
   K_GAMESCREEN_LOADED,          /**< Loaded to system memory */
   K_GAMESCREEN_UPLOADED,        /**< Loaded to VRAM */
};

/**
 * Game screen
 */

class KGameScreen : public KObjectListable {
   friend class KGameScene;

public:
   /**
    * Retrieve a screen from its name
    *
    * \param lpszName name of screen to be retrieved
    *
    * \return screen, or NULL if not found
    */
   static KGameScreen *getScreenByName (const char *lpszName);

   /** Constructor */
   KGameScreen ();

   /** Destructor */
   ~KGameScreen ();
   
   /** Free all scenes added to this game screen */
   void freeAllScenes (void);

   /**
    * Get this screen's name
    *
    * \return screen's name
    */
   const char *getName (void);

   /**
    * Get this screen's name
    *
    * \param lpszName new name
    */
   void setName (const char *lpszName);

   /**
    * Add game scene to this screen, to the tail (rendered in front)
    *
    * \param lpScene scene to be added
    */
   void addScene (KGameScene *lpScene);

   /**
    * Remove scene from this screen
    *
    * \param lpScene scene to be removed
    */
   void removeScene (KGameScene *lpScene);

   /**
    * Add overlay scene to all game screens, to the tail (rendered in front)
    *
    * \param lpOverlayScene overlay scene to be added
    */
   static void addOverlayScene (KGameScene *lpOverlayScene);

   /**
    * Remove overlay scene from all game screens
    *
    * \param lpOverlayScene overlay scene to be removed
    */
   static void removeOverlayScene (KGameScene *lpOverlayScene);

   /**
    * Start loading this game screen in the background
    */
   void loadScreen (void);

   /**
    * Check if this game screen is done loading in the background yet
    *
    * \return true if loaded, false if not
    */
   bool hasLoaded (void);

   /**
    * Switch to this game screen. If the screen hasn't been loaded in the background at all yet, or if it is loading, the screen first finishes loading
    */
   void goToScreen ();

   /**
    * Run the update and render loops for all game screens until the game window is closed. A screen must have been set with goToScreen()
    *
    * \param lpWindowEventCallback callback for window events (K_EVENT_xxx), NULL for none
    */
   static void run (ptkNativeCallBak lpWindowEventCallback);

   /** Free all game screens and all game scenes added to them */
   static void cleanup (void);

   /**
    * Get list of managed graphics
    *
    * \return list of managed graphics
    */
   KManagedGraphicList *getGraphics (void);

   /**
    * Get list of managed fonts
    *
    * \return list of managed fonts
    */
   KManagedFontList *getFonts (void);

   /**
    * Get list of managed sound effects
    *
    * \return list of managed sound effects
    */
   KManagedSampleList *getSamples (void);

#if defined(K_WIN32) || defined (K_MACOS)
   /**
    * Get list of managed videos
    *
    * \return list of managed videos
    */
   KManagedVideoList *getVideos (void);
#endif

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
   /**
    * Get list of managed shaders
    *
    * \return list of managed shaders
    */
   KManagedShaderList *getShaders (void);

   /**
    * Get list of managed 3d models
    *
    * \return list of managed 3d models
    */
   KManagedModelList *getModels (void);
#endif

protected:
   /**
    * Handle that this screen's resources need to be loaded
    */
   virtual void onLoad (void);

   /**
    * Handle that this screen's resources need to be uploaded to VRAM
    */
   virtual void onUpload (void);

   /**
    * Handle that this scene's resources need to be loaded
    */
   virtual void onUnload (void);

   /**
    * Handle that this screen just started displaying
    */
   virtual void onScreenEntered (void);

   /**
    * Handle that this screen just stopped displaying
    */
   virtual void onScreenExited (void);

private:
   /**
    * Wait until this game screen's assets have been fully loaded
    */
   void waitForLoading (void);

   /**
    * Upload this game screen's assets to VRAM
    */
   void upload (void);

   /**
    * Unload this game screen's assets
    */
   void unload (void);

   /**
    * Enter this screen
    */
   void entered (void);

   /**
    * Exit this screen
    */
   void exited (void);

   /**
    * Entry point of backgroung loading thread
    *
    * \param lpParam game screen instance
    * \param nParam unused
    */
   static void loadThreadMain (void *lpParam, long nParam);

   /**
    * Window event handler used while in KGameScreen::run(). Passes UI events to KUIElement and passes the rest to the game-defined handler
    *
    * \param lpKEvent event
    *
    * \return true to keep processing, false not to
    */
   static bool gameScreenEventHandler (KEvent *lpKEvent);

   /** Container for the elements of all the scenes attached to this screen */ 
   KUIElement *_lpScreenUIContainer;

   /** List of game scenes, ordered from back to front */
   KList<KGameScene> _lScenes;

   /** Thread for loading assets in the background */
   KSysThread *_lpLoadingThread;

   /** true if this screen's assets have been loaded yet */
   volatile long _nLoadState;

   /** Name of this screen */
   char _szName[256];

   /** true if UI has been initialized yet */
   static bool g_bUIInitialized;

   /** Callback for handling KWindow events, NULL if none is set */
   static ptkNativeCallBak g_lpWindowEventCallback;

   /** Currently active game screen */
   static KGameScreen *g_lpCurScreen;

   /** Game screen to switch to */
   static KGameScreen *g_lpNextScreen;

   /** List of all game screens */
   static KList<KGameScreen> g_lScreens;

   /** Number of background loading threads that are currently running */
   static long g_nLoadingThreads;

   /** Game screen holding all overlay scenes */
   static KGameScreen g_overlayScreen;
};

#endif   /* _KANJI_KGAMESCREEN_H */
