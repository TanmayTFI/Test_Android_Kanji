/**
 * Kanji rendering and I/O engine - resource file and pakfile definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KRESOURCE_H
#define  _KANJI_KRESOURCE_H

/* Include definitions */
#include "KObjectBase.h"
#include "KObjectListable.h"
#include "KList.h"
#include "KMiscTools.h"

/** Error codes */
typedef enum {
   K_RES_OK = 0,           /**< Success */
   K_RES_STATE = 1000,     /**< No file is currently open for this resource */
   K_RES_MODE,             /**< Unsupported opening mode */
   K_RES_IOERR,            /**< I/O error */
   K_RES_NOTFOUND,         /**< File doesn't exist */
   K_RES_BADFMT,           /**< Pakfile in the wrong format */
} KResourceResult;

/** Opening modes */
typedef enum {
   K_RES_CLOSED = 0,       /**< No file is currently open */
   K_RES_READ = 1000,      /**< Open for reading */
   K_RES_WRITE = 1001,     /**< Open for writing, create the file if it didn't exist, and destroy the contents if it did exist */
} KResourceMode;

/** Reference for seek() offset */
typedef enum {
   K_RES_BEGIN = 0,        /**< Consider offset as absolute from the beginning of the file */
   K_RES_CURRENT,          /**< Add offset to the current read offset in the file */
   K_RES_END,              /**< Add offset to the position of the last byte in the file, +1 */
} KResourceWhence;

/** Properties for addArchive() */
enum {
   K_RESARCHIVE_REQUIRED = 0, /**< This pakfile should exist; write to the log if it can't be found */
   K_RESARCHIVE_OPTIONAL = 1, /**< Silently ignore this archive if it can't be found; useful for resource packs and pakfiles that can override basic content */
};

/** Information provided by stat() */

typedef struct {
   bool bIsDirectory;      /**< true for a directory, false for a file */
   bool bInArchive;        /**< true if file is located in an archive, false if directly on the filesystem */
   time_t nModTime;        /**< Timestamp of last modification to the file */
   size_t nSize;           /**< File size, in bytes */
} KResourceStat;

/* Forward definitions */
class KResourceArchiveDirect;

/**
 * Base class for modeling a currently open file associated with a resource - internal
 */

class KResourceFile : public KObjectListable {
public:
   /** Destructor */
   virtual ~KResourceFile ();

   /**
    * Reposition read file offset for this file
    *
    * \param nWhence what the offset is relative to (K_RES_BEGIN, K_RES_CURRENT or K_RES_END)
    * \param nOffset new offset
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   virtual KResourceResult seek (KResourceWhence nWhence, int nOffset) = 0;

   /**
    * Get current read file offset for this file
    *
    * \param nOffset returned offset
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   virtual KResourceResult tell (int &nOffset) = 0;

   /**
    * Read from this file, at the current read/write file offset
    *
    * \param lpBuffer buffer to read into
    * \param nBytes number of bytes to be read
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   virtual KResourceResult read (void *lpBuffer, size_t nBytes) = 0;

   /**
    * Write to this file, at the current read/write file offset
    *
    * \param lpBuffer buffer to write from
    * \param nBytes number of bytes to be written
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   virtual KResourceResult write (const void *lpBuffer, size_t nBytes);

   /**
    * Read from this file, at the current read/write file offset, and return the number of bytes read
    *
    * \param lpBuffer buffer to read into
    * \param nBytes number of bytes to be read
    *
    * \return number of bytes actually read
    */
   virtual long readAndCount (void *lpBuffer, size_t nBytes) = 0;

   /**
    * Write to this file, at the current read/write file offset, and return the number of bytes written
    *
    * \param lpBuffer buffer to write from
    * \param nBytes number of bytes to be written
    *
    * \return number of bytes actually written
    */
   virtual long writeAndCount (const void *lpBuffer, size_t nBytes);
};

/**
 * Base class for modeling a source of files (filesystem or pakfile) - internal
 */

class KResourceArchive : public KObjectListable {
   friend class KResource;

public:
   /** Constructor */
   KResourceArchive ();

   /** Destructor */
   virtual ~KResourceArchive ();

   /**
    * Try opening specified pakfile using this archive format
    *
    * \param lpszArchiveFilename name of pakfile to be opened
    * \param nProperties insertion properties (K_RESARCHIVE_xxx)
    * \param lpszPassword pakfile password, if any
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   virtual KResourceResult openArchive (const char *lpszArchiveFilename, long nProperties, const char *lpszPassword) = 0;

   /**
    * List the files and folders in this archive in the specified path
    *
    * \param lpszPathName path to examine
    * \param lpEnumCallBack method called back for every entry found
    * \param lpCallBackData data passed back to the callback method
    *
    * \return true to continue listing other pakfiles, false to stop enumerating
    */
   virtual bool enumerateFolder (const char *lpszPathName, enumProc lpEnumCallBack, void *lpCallBackData = NULL) = 0;

   /**
    * Get information on the specified file
    *
    * \param lpszFileName name of file to get information about
    * \param lpStat information filled by this method if successful
    *
    * \return true for success, false for failure
    */
   virtual bool statFile (const char *lpszFileName, KResourceStat *lpStat) = 0;

   /**
    * Attempt to open specified resource file in this pakfile, and create an instance for the opened resource file
    *
    * \param lpszFileName name of resource file to be opened
    *
    * \return instance if successfully opened for reading in this pakfile, NULL if not
    */
   virtual KResourceFile *openFile (const char *lpszFileName) = 0;

   /**
    * Attempt to create specified resource file in this pakfile and open it for writing, and create an instance for the opened resource file
    *
    * \param lpszFileName name of resource file to be created
    *
    * \return instance if successfully created and opened for writing in this pakfile, NULL if not
    */
   virtual KResourceFile *createFile (const char *lpszFileName);

   /**
    * Load file in its entirety
    *
    * \param lpszFileName name of file to be loaded
    * \param lpBuffer returned buffer holding file's contents, if successful
    * \param nBufferSize returned size of buffer, if successful
    *
    * \return true for success, false for failure
    */
   virtual bool loadFile (const char *lpszFileName, unsigned char *&lpBuffer, size_t &nBufferSize) = 0;

   /**
    * Add specified file to the list of pakfiles that are tried for resources before looking resources up directly on the
    * filesystem. Pakfiles are traversed in order of insertion (the first pakfile added has priority over the others). Any pakfile
    * has priority over a filesystem lookup. Mirrored from KResource for source compatibility purposes.
    *
    * \param lpszFileName name of pakfile to be added
    * \param nProperties insertion properties (K_RESARCHIVE_xxx)
    * \param lpszPassword pakfile password, if any
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   static KResourceResult addArchive (const char *lpszFileName, long nProperties = K_RESARCHIVE_REQUIRED, const char *lpszPassword = NULL);

private:
   /** Name of pakfile backing this archive, empty for none */
   char _szArchiveFilename[K_MAXPATH];
};

/**
 * Base class for walking through all possible sources of files 
 */

class KResourceArchiveManager : public KObjectListable {
public:
   /** Destructor */
   virtual ~KResourceArchiveManager ();

   /**
    * Instanciate an archive
    *
    * \return archive
    */
   virtual KResourceArchive *createArchive (void) = 0;
};

/**
 * Class for opening files transparently either inside pakfiles or on the filesystem. The class is used throughout Kanji and can be used by the game to load
 * its own file formats as well.
 */

class KResource : public KObjectBase {
   friend class KMiscTools;

public:
   /** Constructor */
   KResource ();

   /** Destructor */
   ~KResource ();

   /**
    * Enable or disable logging all files opened through KResource::open()
    *
    * \param bEnableLogging true to enable logging file opened through KResource , false to disable it
    */
   static void logOpens (bool bEnableLogging);

   /**
    * Add a pakfile manager to the list tried by addArchive() when opening a pakfile. Used for supporting custom pakfile formats
    *
    * \param lpManager new manager
    */
   static void addArchiveManager (KResourceArchiveManager *lpManager);

   /**
    * Add specified file to the list of pakfiles that are tried for resources before looking resources up directly on the
    * filesystem. Pakfiles are traversed in order of insertion (the first pakfile added has priority over the others). Any pakfile
    * has priority over a filesystem lookup.
    *
    * \param lpszFileName name of pakfile to be added
    * \param nProperties insertion properties
    * \param lpszPassword pakfile password, if any
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   static KResourceResult addArchive (const char *lpszFileName, long nProperties = K_RESARCHIVE_REQUIRED, const char *lpszPassword = NULL);

   /**
    * List the files and folders at the specified path, in all registered pakfiles. To enumerate on the filesystem, use KMiscTools::enumerateFolder
    *
    * \param lpszPathName path to examine
    * \param lpEnumCallBack method called back for every entry found
    * \param lpCallBackData data passed back to the callback method
    */
   static void enumerateFolderInArchives (const char *lpszPathName, enumProc lpEnumCallBack, void *lpCallBackData = NULL);

   /**
    * Get information on the specified file
    *
    * \param lpszFileName name of file to get information about
    * \param lpStat information filled by this method if successful
    *
    * \return true for success, false for failure
    */
   static bool stat (const char *lpszFileName, KResourceStat *lpStat);

   /**
    * Load file in its entirety, using KResource
    *
    * \param lpszFileName name of file to be loaded
    * \param lpBuffer returned buffer holding file's contents, if successful
    * \param nBufferSize returned size of buffer, if successful
    *
    * \return true for success, false for failure
    */
   static bool loadResource (const char *lpszFileName, unsigned char *&lpBuffer, size_t &nBufferSize);

   /**
    * Open file either in a pakfile or on the filesystem
    *
    * \param lpszFileName filename (relative if it is to be looked up in pakfiles)
    * \param nOpenMode opening mode (K_RES_READ or K_RES_WRITE)
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   KResourceResult open (const char *lpszFileName, KResourceMode nOpenMode);

   /** Close this file */
   void close (void);

   /**
    * Reposition read file offset for this file
    *
    * \param nWhence what the offset is relative to (K_RES_BEGIN, K_RES_CURRENT or K_RES_END)
    * \param nOffset new offset
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   KResourceResult seek (KResourceWhence nWhence, int nOffset);

   /**
    * Get current read file offset for this file
    *
    * \param nPos returned offset
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   KResourceResult tell (int &nPos);

   /**
    * Read from this file, at the current read/write file offset
    *
    * \param lpBuffer buffer to read into
    * \param nBytes number of bytes to be read
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   KResourceResult read (void *lpBuffer, size_t nBytes);

   /**
    * Write to this file, at the current read/write file offset
    *
    * \param lpBuffer buffer to write from
    * \param nBytes number of bytes to be written
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   KResourceResult write (const void *lpBuffer, size_t nBytes);

   /**
    * Read from this file, at the current read/write file offset, and return the number of bytes read
    *
    * \param lpBuffer buffer to read into
    * \param nBytes number of bytes to be read
    *
    * \return number of bytes actually read
    */
   long readAndCount (void *lpBuffer, size_t nBytes);

   /**
    * Write to this file, at the current read/write file offset, and return the number of bytes written
    *
    * \param lpBuffer buffer to write from
    * \param nBytes number of bytes to be written
    *
    * \return number of bytes actually written
    */
   long writeAndCount (const void *lpBuffer, size_t nBytes);

   /**
    * Write char to this file
    *
    * \param c char to write
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   KResourceResult putChar (const char c);

   /**
    * Write formatted text to this file
    *
    * \param lpszFormat formatted string using the ansi character set, using a printf-like syntax
    * \param ... arguments for the formatted string, if any
    *
    * \return number of bytes actually written
    */
   void print (const char *lpszFormat, ...);

   /**
    * Check error indicator for this file
    *
    * \return true if an error occured on this file, false if not
    */
   bool checkError (void);

   /** Reset error indicator for this file */
   void clearError (void);

private:
   /** Initialize class - internal */
   static void initialize (void);

   /** Clean up all resources used by class - internal */
   static void cleanup (void);

   /** Open file associated with this resource, NULL if not currently open */
   KResourceFile *_lpResFile;

   /** true if an error occured on this file */
   bool _bError;

   /** true to enable logging of files opened through KResource, false not to */
   static bool g_bEnableLogging;

   /** true if packfile managers have been added */
   static bool g_bManagersInitialized;

   /** List of all pakfile managers */
   static KList<KResourceArchiveManager> g_lArchiveManagers;

   /** List of all pakfiles added by the game, where files are tentatively opened before trying the filesystem */
   static KList<KResourceArchive> g_lArchives;

   /** Archive for direct access to the filesystem */
   static KResourceArchiveDirect *g_lpArchiveDirect;
};

#endif   /* _KANJI_KRESOURCE_H */
