/**
 * Kanji rendering and I/O engine - Android top-level object creation definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KPTK_H
#define  _KANJI_KPTK_H

/** Define that the game is rendering to Kanji */
#define  __KANJI__

/* Include definitions */
#include "KObjectBase.h"
#include "KObjectListable.h"
#include "KList.h"
#include "KMath2d.h"
#include "KMath3d.h"
#include "KSysThread.h"
#include "KSysLock.h"
#include "KSysDebug.h"
#include "KSysAndroid.h"
#include "KWindow.h"
#include "KMiscTools.h"
#include "KResource.h"
#include "KIniReader.h"
#include "KImage.h"
#include "KGraphic.h"
#include "KBatch.h"
#include "KPixelShader.h"
#include "KText.h"
#include "KTrueText.h"
#include "KInput.h"
#include "KGestureArea.h"
#include "KSetting.h"

/** Write a message to the logfile */
#define K_LOG(...) KPTK::logMessage(__VA_ARGS__)

/** Game window's renderer type */
enum Erenderer {
   K_DIRECTX,        /**< Direct3D 8 - on win32 only */
   K_OPENGL,         /**< OpenGL */
   K_SOFT,           /**< Software rendering - on win32 only */
   K_DIRECTX9,       /**< Direct3D 9 - on win32 and winrt only */
   K_DIRECTX11,      /**< Direct3D 11 - on win32 and winrt only */
};

/** Maximum number of buffered log message characters before the logfile is opened */
#define  K_MAXLOGBUFFER 8192

/* Forward declarations */
class KSoundBase;
class KVideoBase;

/**
 * Class for creating Kanji objects and managing top-level features
 */

class KPTK : public KObjectBase {
   friend class KWindow;
   
public:
   /**
    * Enable logging Kanji's and game's own information to a file. If this method isn't called, the use of K_LOG() is ignored
    *
    * \param lpszFileName name of file to log to
    * \param lpszGameName name of the game using Kanji for rendering
    * \param lpszGameVersion version of the game using Kanji for rendering
    * \param bLogToConsole true to write log entries to the system console as well as the specified file, false to only write to the specified file
    */
   static void enableLog (const char *lpszFileName, const char *lpszGameName, const char *lpszGameVersion, bool bLogToConsole = false);

   /**
    * Write a message to the logfile, if enabled
    *
    * \param lpszFormat formatted message using the ansi character set, using a printf-like syntax
    * \param ... arguments for the formatted message, if any
    */
   static void logMessage (const char *lpszFormat, ...);

   /**
    * Write a message to the logfile, if enabled
    *
    * \param lpszFormat formatted message using the ansi character set, using a printf-like syntax
    * \param param arguments for the formatted message
    */
   static void vlogMessage (const char *lpszFormat, va_list param);
   
   /**
    * Check if an instance of the game is already running. If it does, bring it to the foreground and report so
    *
    * \param lpszGameId unique game identifier string (for instance an UUID generated by the developer)
    *
    * \return true if a running instance was found and brought to the player's attention, false if none was found
    */
   static bool checkGameInstance (const char *lpszGameId);

   /**
    * Create KWindow
    *
    * \param nRenderer renderer to use (K_OPENGL)
    * \param bCreateDepthBuffer true to create a depth buffer (Z-buffer) for the window, false not to
    * \param bCreateStencilBuffer true to create a stencil buffer for the window, false not to
    *
    * \return created window; createGameWindow() must subsequently be called on the returned object
    */
   static KWindow *createKWindow (Erenderer nRenderer, bool bCreateDepthBuffer = false, bool bCreateStencilBuffer = false);

   /**
    * Create KGraphic
    *
    * \return new graphic
    */
   static KGraphic *createKGraphic (void);

   /**
    * Create KBatch
    *
    * \return new batching buffer
    */
   static KBatch *createKBatch (void);

   /**
    * Create KPixelShader
    *
    * \return new pixel shader
    */
   static KPixelShader *createKPixelShader (void);
   
   /**
    * Register method for creating a KSound object - internal
    *
    * \param lpNewKSound method for creating a new KSound object
    *
    * \private
    */
   static void registerNewKSound (KSoundBase *(*lpNewKSound)(void));
   
   /**
    * Create a new KSound object if possible - internal
    *
    * \return new KSound object, NULL if no method for creating one was registered
    *
    * \private
    */
   static KSoundBase *createKSound (void);
   
   /**
    * Register method for creating a KVideo object - internal
    *
    * \param lpNewKVideo method for creating a new KVideo object
    *
    * \private
    */
   static void registerNewKVideo (KVideoBase *(*lpNewKVideo)(void));
   
   /**
    * Create a new KVideo object if possible - internal
    *
    * \return new KVideo object, NULL if no method for creating one was registered
    *
    * \private
    */
   static KVideoBase *createKVideo (void);
   
   /** Free all lingering resources when not using Kanji anymore. This method may be called by the game just before exiting,
    * to check for memory leaks, otherwise it is not necessary. */
   static void cleanup (void);

   /**
    * Get game window
    *
    * \return game window, NULL if not created
    */
   static KWindow *getGameWindow (void) { return g_lpKWindow; }

   /** Renderer being used for the game window */
   static Erenderer _renderer;

   /** Log file stream, NULL if not open */
   static FILE *_fLog;

   /** Buffer for composing internal filenames */
   static char _szFileNameBuf[K_MAXPATH];

private:
   /**
    * Close logfile and disable logging, as the game is about to exit
    */
   static void disableLog (void);

   /** Game window, if created */
   static KWindow *g_lpKWindow;
   
   /** true to log to the system console as well as a file */
   static bool g_bLogToConsole;
   
   /** Number of log message characters buffered before the logfile was opened */
   static size_t g_nBufferedLogSize;

   /** Buffered log messages, before the logfile was opened */
   static char g_szBufferedLog[K_MAXLOGBUFFER];

   /** Singleton of KMiscTools */
   static KMiscTools g_miscTools;
};

#endif   /* _KANJI_KPTK_H */
