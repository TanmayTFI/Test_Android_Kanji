/**
 * Kanji rendering and I/O engine - Android system functions definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMISCTOOLS_H
#define  _KANJI_KMISCTOOLS_H

/* Include definitions */
#include "KObjectBase.h"
#include "KSysLock.h"
#include "KSysThread.h"

/* Forward declarations */
class KWindow;
class KWindowGLES;
class KWindowGLES2;
class KPTK;

/**
 * Method called back by enumerateFolder
 *
 * \param lpszEntryName name of file or directory
 * \param bIsFolder true if entry is a directory, false if it is a file
 * \param lpCallBackData user data passed to enumerateFolder
 *
 * \return true to continue until all files and directories have been listed, false to stop enumeration
 */
typedef bool (*enumProc) (const char *lpszEntryName, bool bIsFolder, void *lpCallBackData);

/** Time returned by KMiscTools::getLocalTime() */

typedef struct {
   long nDay;        /**< Current day of the month */
   long nMonth;      /**< Current month */
   long nYear;       /**< Current year */
   long nHour;       /**< Current hour */
   long nMinute;     /**< Current minute */
   long nSecond;     /**< Current second */
} KMiscToolsTime;

/**
 * Class for accessing operating system functions such as timing and file paths
 */

class KMiscTools : public KObjectBase {
   friend class KWindow;
   friend class KWindowGLES;
   friend class KWindowGLES2;
   friend class KPTK;

public:
   /** Constructor */
   KMiscTools ();

   /** Destructor */
   ~KMiscTools ();

   /** Initialize system functions - does not need to be called by the game */
   static void initMiscTools (void);

   /** Initialize clock - does not need to be called by the game */
   static void initMillisecond (void);

   /**
    * On MacOSX, set whether game files are stored in the application bundle resources, or at the same level as the application.app folder (default). This
    * method is ignored on other operating systems
    *
    * \param bInBundle true to get game files from the app bundle; false to get them at the same level as the .app folder
    */
   static void setFileSource (bool bInBundle);
   
   /**
    * Convert slash and backslash directory seperators in the specified path to the proper character for the operating system
    *
    * \param lpszPath path to be converted
    */
   static void cleanPath (char *lpszPath);

   /**
    * Expand the specified relative pathname into a fully qualified pathname that can be used to open files
    *
    * \param lpszFileName relative pathname (for instance "data/background.bmp" or "data\images\image.png")
    *
    * \return fully qualified pathname
    */
   static char *makeFilePath (const char *lpszFileName);

   /**
    * Expand the specified relative pathname into a fully qualified pathname that can be used to open files
    *
    * \param lpszFileName relative pathname (for instance "data/background.bmp" or "data\images\image.png")
    * \param lpBuffer buffer to compose fully qualified pathname into
    * \param nBufferSize size of buffer, in bytes
    *
    * \return fully qualified pathname
    */
   static char *makeFilePathInBuffer (const char *lpszFileName, char *lpBuffer, size_t nBufferSize);
   
   /**
    * Get the full path to the game folder (where the game executable is located)
    *
    * \return game folder path
    */
   static const char *getGameFolder (void);

   /**
    * Get the full path to the logged in user's profile, where user-specific data can be saved and restored
    *
    * \return user folder path
    */
   static const char *getUserFolder (void);

   /**
    * Get the full path to the global preferences folder, where system-wide data can be saved and restored
    *
    * \return preferences folder path
    */
   static const char *getPreferencesFolder (void);

   /**
    * Get name of the user that started the game
    *
    * \return name
    */
   static const char *getUserName (void);
   
   /**
    * Get operating system's name and version as a text string, mainly useful for bug reports
    *
    * \return operating system name and version
    */
   static const char *getOSVersion (void);
   
   /**
    * Get a clock as a number of milliseconds. The absolute value is meaningless and should only be used as a delta
    * between two calls to getMilliseconds(), in order to measure elapsed time.
    *
    * \return clock in milliseconds
    */
   static unsigned long getMilliseconds (void);

   /**
    * Get a value that can be used to seed a random generator. The value itself is meaningless.
    *
    * \return seed value
    */
   static unsigned long getSeedValue (void);
   
   /**
    * Get current year
    *
    * \return current year (for instance 2010)
    */
   static long getYear (void);
   
   /**
    * Get current local date and time
    *
    * \return local date and time
    */
   static KMiscToolsTime getLocalTime (void);
   
   /**
    * Busy wait for the specified amount of milliseconds
    *
    * \param nMillis millseconds to busy wait for
    * \param bPollEvents true to poll system events while busy waiting, false not to
    */
   static void pause (long nMillis, bool bPollEvents = false);

   /**
    * Open the specified file
    *
    * \param lpszFileName full path to file to be opened
    * \param bMaximized true to open the file in a maximized explorer or finder window
    */
   static void launchFile (const char *lpszFileName, bool bMaximized = false);
   
   /**
    * Open the specified URL
    *
    * \param lpszUrl URL to be opened ("http://..." for instance)
    * \param bMaximized true to open the URL in a maximized browser window
    */
   static void launchURL (const char *lpszUrl, bool bMaximized = true);

   /**
    * Execute http get request. The response will be provided by a K_EVENT_HTTP_REQUEST event
    *
    * \param nRequestId identifier to use for the request
    * \param lpszUrl full URL of the http get request
    * \param lpszResponseFilePath path of file to write the response to, NULL to receive the response in a memory buffer
    *        (passed by the K_EVENT_HTTP_REQUEST event)
    * \param bBypassCache true to bypass cache (default), false to allow returning cached data
    */
   static void doHttpGet (long nRequestId, const char *lpszUrl, const char *lpszResponseFilePath = NULL,
                          bool bBypassCache = true);
   
   /**
    * Execute http post request. The response will be provided by a K_EVENT_HTTP_REQUEST event
    *
    * \param nRequestId identifier to use for the request
    * \param lpszUrl full URL of the http post request
    * \param lpPayloadData payload to send as application/x-www-form-urlencoded
    * \param nPayloadSize size of payload, in bytes
    * \param lpszResponseFilePath path of file to write the response to, NULL to receive the response in a memory buffer
    *        (passed by the K_EVENT_HTTP_REQUEST event)
    * \param bBypassCache true to bypass cache (default), false to allow returning cached data
    */
   static void doHttpPost (long nRequestId, const char *lpszUrl, const void *lpPayloadData, size_t nPayloadSize,
                           const char *lpszResponseFilePath = NULL, bool bBypassCache = true);
   
   /**
    * Execute http delete request
    *
    * \param nRequestId identifier to use for the request
    * \param lpszUrl full URL of the http delete request
    * \param lpszResponseFilePath path of file to write the response to, NULL to receive the response in a memory buffer
    *        (passed by the K_EVENT_HTTP_REQUEST event)
    * \param bBypassCache true to bypass cache (default), false to allow returning cached data
    */
   static void doHttpDelete (long nRequestId, const char *lpszUrl, const char *lpszResponseFilePath = NULL,
                             bool bBypassCache = true);
   
   /**
    * Display an alert box to the player
    *
    * \param lpszTitle alert box's caption
    * \param lpszText alert box's body
    */
   static void messageBox (const char *lpszTitle, const char *lpszText);

   /**
    * Display an alert box to the player with a body and one or two custom buttons
    *
    * \param lpszTitle alert box's caption
    * \param lpszText alert box's body
    * \param lpszButton1 text for button 1
    * \param lpszButton2 text for button 2 (NULL for a single button alert box)
    *
    * \return 0 if button 1 was pressed, 1 if button 2 was pressed
    */
   static long alertBox (const char *lpszTitle, const char *lpszText, const char *lpszButton1, const char *lpszButton2);
   
   /**
    * Play alert sound or vibrate
    */
   static void signalUser (void);
   
   /**
    * Play MP3 in the background using the operating system services. This is the preferred method of playing background music on
    * iPhone/iPad using the audio decoding hardware. It is supported on windows/mac for simulation purposes but KSound should be used
    * in production instead. The file cannot be stored in a pakfile.
    *
    * \param lpszFileName name of file to be played (cannot be stored in a pakfile)
    * \param nVolume initial volume, 0..100
    * \param bLoop true to loop, false not to
    * \param bDoNoWait true to wait until music starts playing before returning, false to return immediately and let music start in the background
    */
   static void playBackgroundMusic (const char *lpszFileName, long nVolume, bool bLoop, bool bDoNoWait);
   
   /**
    * Set new volume for background music
    *
    * \param nVolume new volume, 0..100
    */
   static void setBackgroundMusicVolume (long nVolume);
      
   /**
    * Stop currently playing background music, if any
    */
   static void stopBackgroundMusic (void);
   
   /**
    * Check if background music is currently playing
    *
    * \return true if playing, false if not
    */
   static bool isBackgroundMusicPlaying (void);
   
   /**
    * Create the folder at the specified path
    *
    * \param lpszPath path of folder to be created
    *
    * \return true for success, false for failure
    */
   static bool createDirectory (const char *lpszPath);

   /**
    * Remove the file at the specified path
    *
    * \param lpszPath path of file to be removed
    *
    * \return true for success, false for failure
    */
   static bool removeFile (const char *lpszPath);
   
   /**
    * Remove the specified file
    *
    * \param lpszOldPath path of file to be renamed
    * \param lpszNewPath new name
    *
    * \return true for success, false for failure
    */
   static bool renameFile (const char *lpszOldPath, const char *lpszNewPath);
   
   /**
    * Remove the folder at the specified path
    *
    * \param lpszPath path of folder to be removed
    *
    * \return true for success, false for failure
    */
   static bool removeDirectory (const char *lpszPath);
   
   /**
    * Check if the specified path exists on the filesystem
    *
    * \param lpszPath path to be checked
    *
    * \return true if the path exists, false if it doesn't
    */
   static bool fileExists (const char *lpszPath);

   /**
    * List the files and folders in the specified path
    *
    * \param lpszPath path to examine
    * \param lpEnumCallBack method called back for every entry found
    * \param lpCallBackData data passed back to the callback method
    */
   static void enumerateFolder (const char *lpszPath, enumProc lpEnumCallBack, void *lpCallBackData = NULL);
   
   /**
    * Switch endianness of the specified short value on big-endian machines, do nothing on little-endian machines
    *
    * \param nValue short whose endianness is to be flipped
    *
    * \return short with flipped endianness on big-endian machines, same short on little-endian machines
    */
   static short flipShort (short nValue) { return nValue; }

   /**
    * Switch endianness of the specified int value on big-endian machines, do nothing on little-endian machines
    *
    * \param nValue int whose endianness is to be flipped
    *
    * \return int with flipped endianness on big-endian machines, same int on little-endian machines
    */
   static int flipInt (int nValue) { return nValue; }
   
   /**
    * Switch endianness of the specified long value on big-endian machines, do nothing on little-endian machines
    *
    * \param nValue long whose endianness is to be flipped
    *
    * \return long with flipped endianness on big-endian machines, same long on little-endian machines
    */
   static long flipLong (long nValue) { return nValue; }

   /**
    * Switch endianness of the specified float value on big-endian machines, do nothing on little-endian machines
    *
    * \param fValue float whose endianness is to be flipped
    *
    * \return float with flipped endianness on big-endian machines, same float on little-endian machines
    */
   static float flipFloat (float fValue) { return fValue; }
   
   /**
    * Switch endianness of the specified double value on big-endian machines, do nothing on little-endian machines
    *
    * \param fValue double whose endianness is to be flipped
    *
    * \return double with flipped endianness on big-endian machines, same double on little-endian machines
    */
   static double flipDouble (double fValue) { return fValue; }

   /**
    * Free all resources - internal
    *
    * \private
    */
   static void cleanup (void);

   /** Suspend currently playing background music, when audio session is interrupted - internal */
   static void suspendBackgroundMusic (void);
   
   /** Resume currently playing background music, when audio session is restored - internal */
   static void resumeBackgroundMusic (void);
   
private:
   /**
    * Set game window
    *
    * \param lpKWindow game window
    */
   static void setKWindow (KWindow *lpKWindow);
   
   /**
    * Pause or resume background music, if it is currently playing
    *
    * \param bPause true to pause, false to resume
    */
   static void pauseBackgroundMusic (bool bPause);

   /** true if KMiscTools has been initialized yet */
   static bool g_bToolsInitialized;

   /** true if clock has been initialized yet */
   static bool g_bClockInitialized;

   /** Starting clock */
   static long g_nStartClock;

   /** Game window */
   static KWindow *g_lpKWindow;
   
   /** Android OS version */
   static char g_szAndroidOSVersion[256];

   /** Buffer for composing paths */
   static char g_szPathBuffer[K_MAXPATH];
};

#endif   /* _KANJI_KMISCTOOLS_H */
