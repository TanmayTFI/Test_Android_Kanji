/**
 * Kanji game engine - game class definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KGAME_H
#define  _KANJI_KGAME_H

/* Include definitions */
#include "KPTK.h"
#include "KManagedGraphic.h"
#include "KManagedFont.h"
#include "KManagedBitmapFont.h"
#include "KManagedSample.h"
#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_ANDROID) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_LINUX)
#ifndef K_DISABLE_VIDEO
#include "KManagedVideo.h"
#endif
#endif
#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_ANDROID) || defined (K_LINUX)
#include "KManagedShader.h"
#endif
#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
#include "KManagedModel.h"
#endif
#include "KCounter.h"
#include "KAnimator.h"
#include "KChecksum.h"
#include "KRandom.h"
#include "KRandomSet.h"
#include "KBezier.h"
#include "KRichText.h"
#include "KJSON.h"
#include "KBase64.h"
#include "KUIElement.h"
#include "KUILayout.h"
#include "KUIGradient.h"
#include "KUIImage.h"
#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_ANDROID) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_LINUX)
#ifndef K_DISABLE_VIDEO
#include "KUIVideo.h"
#endif
#endif
#include "KUIText.h"
#include "KUIRichText.h"
#include "KUIButton.h"
#include "KUISelector.h"
#include "KUISlider.h"
#include "KUIEditField.h"
#include "KUIEditMulti.h"
#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
#include "KModel.h"
#endif
#include "KStage.h"
#include "KGameScreen.h"
#include "KSignalManager.h"

/** Assert condition and exit the game if the condition isn't met */
#define KAssert(__cond) do { if (!(__cond)) KSysDebug::assertionFailed (__FILE__,__LINE__); } while (0)

/** Shortcut macro for getting a string from the strings table */
#define KStr(__szId) KGame::getGame()->getString (__szId)

/**
 * Game class, derive your own from it. Not required to use the UI elements or any other Kanji class.
 */

class KGame : public KObjectBase {
public:
   /**
    * Get game singleton
    *
    * \return game
    */
   static KGame *getGame (void) { return g_lpGame; }

   /** Constructor */
   KGame ();

   /** Destructor */
   ~KGame ();

   /**
    * Initialize, run and clean the game up
    */
   void main (void);

   /**
    * Initialize the game 
    *
    * \return true if successful, false if failed
    */
   bool initialize (void);

   /**
    * Shut the game down
    */
   void cleanup (void);

   /**
    * Get the name of this game
    *
    * \return game's name
    */
   virtual const char *getGameName (void) = 0;

   /**
    * Get the name of this game, as a safe string that can be used to create folders and files
    *
    * \return safe game's name
    */
   virtual const char *getSafeGameName (void) = 0;

   /**
    * Get the name of this game's development studio, as a safe string that can be used to create folders and files
    *
    * \return safe game's developer name
    */
   virtual const char *getSafeDeveloperName (void) = 0;

   /**
    * Get the version of the game that this engine is running
    *
    * \return game's version
    */
   virtual const char *getGameVersion (void) = 0;

   /**
    * Load settings and create game window. Derive and implement this method
    *
    * \return game window, or NULL to exit
    */
   virtual KWindow *createGameWindow (void) = 0;

   /**
    * Load and initialize game-specific assets before entering the game loop. Derive and implement this method if needed
    *
    * \return true if successful, false if failed
    */
   virtual bool loadGame (void);

   /**
    * Run the game. Derive and implement this method
    */
   virtual void run (void) = 0;

   /**
    * Update logic and render a frame. Derive and implement this method if needed
    */
   virtual void renderFrame (void);

   /**
    * Unload all game-specific assets. Derive and implement this method if needed
    */
   virtual void unloadGame (void);

   /**
    * Close game window and save settings. Derive and implement this method
    */
   virtual void destroyGameWindow (KWindow *lpKWindow) = 0;

   /**
    * Enable or disable catching crashes and offering the user to report them, as well as assertion failures
    *
    * \param bEnable true to enable, false to disable
    * \param lpszSupportEmail e-mail address to send crash reports to
    */
   void enableCrashReporter (bool bEnable, const char *lpszSupportEmail);

   /**
    * Get folder where game state and settings can be loaded and saved from. The folder is automatically created from
    * the system preferences folder plus the safe developer and game's names
    *
    * \return full path to the state folder
    */
   const char *getStateFolder (void);

   /**
    * Read strings table in tabbed CSV format, then initialize KText and KTrueText with the string table's encoding
    *
    * \param lpszFileName name of file containing the game's strings
    */
   void readStringTable (const char *lpszFileName);

   /**
    * Write strings table in tabbed CSV format
    *
    * \param lpszFileName name of file to write game's strings to
    */
   void writeStringTable (const char *lpszFileName);

   /**
    * Get encoding used for all text in the string table
    *
    * \return text encoding for the string table
    */
   long getTextEncoding (void) { return _nTextEncoding; }

   /**
    * Get number of strings in the string table
    *
    * \return number of strings
    */
   long getStringCount (void) { return _nStrings; }

   /**
    * Get ID at the specified index
    *
    * \param nStringIdx index of ID to get
    *
    * \return string ID, or NULL if out of bounds
    */
   const char *getStringId (long nStringIdx);

   /**
    * Get string from the string table, looking it up by ID. The strings table must be read first
    *
    * \param lpszStringId string ID (using ansi encoding)
    *
    * \return string value, using the string table's encoding, or NULL if the string doesn't exist
    */
   virtual const char *getString (const char *lpszStringId);

   /**
    * Get string from the string table, looking it up by ID. The strings table must be read first
    *
    * \param lpszStringId string ID (using ansi encoding)
    *
    * \return string value, using the string table's encoding, or the default value if the string doesn't exist
    */
   virtual const char *getStringSafe (const char *lpszStringId, const char *lpszDefaultValue = "");

   /**
    * Get string from the string table at the specified index. The strings table must be read first
    *
    * \param nStringIdx index of string to get
    *
    * \return string value, using the string table's encoding
    */
   virtual const char *getStringByIndex (long nStringIdx);

   /**
    * Get built-in string, such as fatal error messages. Derive to implement your own messages if needed
    *
    * \param lpszStringId built-in string ID (using ansi encoding)
    *
    * \return string value, using utf-8 encoding
    */
   virtual const char *getBuiltinString (const char *lpszStringId);

   /**
    * Set string in the string table. If the ID exists, the text value is updated, otherwise the ID is created. Built-in strings cannot be set
    *
    * \param lpszStringId string ID to create or update
    * \param lpszText new value
    */
   void setString (const char *lpszStringId, const char *lpszText);

   /**
    * Remove string in the string table, specified by its ID, if it exists
    *
    * \param lpszStringId ID of string to be removed
    */
   void removeString (const char *lpszStringId);

   /**
    * Get game window
    *
    * \return game window
    */
   KWindow *getKWindow (void) { return _lpKWindow; }

   /**
    * Reset frame clocking, just before entering the game loop
    */
   void resetFrameLoop (void);

   /**
    * Begin rendering a frame
    */
   void beginFrame (void);

   /**
    * Get scaling factor applied to the elapsed time since the last frame
    *
    * \return current time scaling factor
    */
   double getTimeScale (void) { return _fTimeScale; }

   /**
    * Set scaling factor applied to the elapsed time since the last frame
    *
    * \param fScale new time scaling factor
    */
   void setTimeScale (double fScale) { _fTimeScale = fScale; }

   /**
    * Get number of milliseconds that elapsed since the last frame
    */
   double getElapsedTime (void) { return _fFrameTime; }

   /**
    * Get number of 10ms logic ticks to be executed for this frame
    */
   long getTickCount (void) { return _nFrameTicks; }

   /**
    * End rendering a frame, present it, and flip front and back buffers
    *
    * \param bWaitForFocus true to wait for window focus when flipping back buffer, false to continue running
    */
   void endFrame (bool bWaitForFocus = true);

   /**
    * Handle end of frame. This method is called just before presenting and can be derived to implement
    * a software cursor or to display overlaid debug information common to all game screens, for instance.
    */
   virtual void onEndFrame (void);

   /**
    * Get list of managed graphics
    *
    * \return list of managed graphics
    */
   KManagedGraphicList *getGraphics (void) { return &_lGraphics; }

   /**
    * Get list of managed vector fonts
    *
    * \return list of managed vector fonts
    */
   KManagedFontList *getFonts (void) { return &_lFonts; }

   /**
    * Get list of managed bitmap fonts
    *
    * \return list of managed bitmap fonts
    */
   KManagedBitmapFontList *getBitmapFonts (void) { return &_lBitmapFonts; }

   /**
    * Get list of managed sound effects
    *
    * \return list of managed sound effects
    */
   KManagedSampleList *getSamples (void) { return &_lSamples; }

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_ANDROID) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_LINUX)
#ifndef K_DISABLE_VIDEO
   /**
    * Get list of managed videos
    *
    * \return list of managed videos
    */
   KManagedVideoList *getVideos (void) { return &_lVideos; }
#endif
#endif

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_ANDROID) || defined (K_LINUX)
   /**
    * Get list of managed shaders
    *
    * \return list of managed shaders
    */
   KManagedShaderList *getShaders (void) { return &_lShaders; }
#endif

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
   /**
    * Get list of managed 3d models
    *
    * \return list of managed 3d models
    */
   KManagedModelList *getModels (void) { return &_lModels; }
#endif

   /**
    * Display an error to the user and exit due to a missing or corrupted file. This method does not return
    *
    * \param lpszFileName name of missing or corrupted file that causes the game to exit
    */
   virtual void abortOnFile (const char *lpszFileName);

   /**
    * Compose path from prefix and filename
    *
    * \param lpszPath prefix; appended if the filename isn't already an absolute path
    * \param lpszFileName filename
    * \param lpBuffer where to store combined path
    * \param nBufferSize maximum size of buffer, in bytes
    */
   static void composePath (const char *lpszPath, const char *lpszFileName, char *lpBuffer, size_t nBufferSize);

   /** Game window */
   KWindow *_lpKWindow;

private:
   /** Free resources used by the strings table */
   void freeStringTable ();

   /**
    * Method called back when a crash or an assertion failure happens
    *
    * \param bIsAssertion true for an assertion failure, false for an exception
    * \param nExceptionCode for an exception, OS exception code
    * \param lpszExceptionName for an exception, textual name of exception code
    * \param lpExceptionAddr for an exception, exception address
    * \param bHasParam for an exception, true if optional exception parameter is provided
    * \param lpParam for an exception, optional parameter
    * \param lpszAssertionFileName for an assertion failure, sourcecode filename where the assertion failed
    * \param nAssertionLine for an assertion failure, line number in filename where the assertion failed
    * \param nStackDepth number of entries in stack trace
    * \param lpStackReturnAddr array of return addresses in stack trace
    * \param lpszStackModule array of module names in stack trace
    */
   static void crashHandler (bool bIsAssertion,
                             unsigned long nExceptionCode, const char *lpszExceptionName, bool bHasParam, void *lpParam,
                             const char *lpszAssertionFileName, long nAssertionLine,
                             long nStackDepth, void *lpStackReturnAddr[], char *lpszStackModule[]);

   /**
    * Enquote crash report so as to put it in the body of an e-mail
    *
    * \param lpszMessage source report
    * \param lpEnquoteBuf where the enquoted message will be stored
    * \param nEnquoteBufSize maximum size of the enquoted message, including terminal 0
    */
   static void enquoteMessage (const char *lpszMessage, char *lpEnquoteBuf, size_t nEnquoteBufSize);

   /** Full path to the game state folder */
   char _szGameStatePath[K_MAXPATH];

   /** Encoding of all strings in the string table */
   long _nTextEncoding;

   /** Number of loaded identifier/value pairs in the string table */
   long _nStrings;

   /** Index of first ID that isn't part of the initial table */
   long _nMinAllocatedStringId;

   /** Loaded string identifiers, using ansi encoding */
   char **_lpszStringId;

   /** Loaded string values, using the encoding of the string table file */
   char **_lpszStringVal;

   /** For each string value, true if it was seperately allocated, false if it part of the initial buffer */
   bool *_bStringValAllocated;

   /** Buffer containing initially loaded identifiers and values */
   char *_szStringIdBuffer;

   /** Buffer containing initially loaded identifiers and values */
   char *_szStringValueBuffer;

   /** Current clock for logic ticks */
   double _fLogicClock;

   /** Scaling factor applied to the reported time deltas */
   double _fTimeScale;

   /** Time elapsed during the last frame */
   double _fFrameTime;

   /** Number of logic ticks that need to be done for the current frame */
   long _nFrameTicks;

   /** Email address where crash and assertion failure reports are sent */
   char _szSupportEmail[256];

   /** List of managed graphics */
   KManagedGraphicList _lGraphics;

   /** List of managed vector fonts */
   KManagedFontList _lFonts;

   /** List of managed bitmap fonts */
   KManagedBitmapFontList _lBitmapFonts;

   /** List of managed sound effects */
   KManagedSampleList _lSamples;

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_ANDROID) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_LINUX)
#ifndef K_DISABLE_VIDEO
   /** List of managed videos */
   KManagedVideoList _lVideos;
#endif
#endif

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_WINRT) || defined (K_BLACKBERRY) || defined (K_ANDROID) || defined (K_LINUX)
   /** List of managed shaders */
   KManagedShaderList _lShaders;
#endif

#if defined(K_WIN32) || defined (K_MACOS) || defined (K_IPHONE)
   /** List of managed 3d models */
   KManagedModelList _lModels;
#endif

   /** Built-in string identifiers, NULL-terminated */
   static const char *g_lpszBuiltinStringId[];

   /** Built-in string values */
   static const char *g_lpszBuiltinStringVal[];

   /** Buffer for composing error messages */
   static char g_szErrMessage[1024];

   /** Buffer for sending crash reports by e-mail */
   static char g_szEnquotedErrMessage[1024];

   /** Game singleton */
   static KGame *g_lpGame;
};

#endif   /* _KANJI_KGAME_H */
