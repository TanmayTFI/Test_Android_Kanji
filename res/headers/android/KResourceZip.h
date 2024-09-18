/**
 * Kanji rendering and I/O engine - ZIP pakfile handler definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KRESOURCEFILEZIP_H
#define  _KANJI_KRESOURCEFILEZIP_H

/* Include definitions */
#include "KResource.h"
#include "KSysLock.h"
#include "KHashTable.h"

/* Forward declarations */
typedef void *k_unzFile;
class KResourceArchiveZipEntry;

/**
 * Class for modeling a currently open file associated with a resource, that is present directly on the filesystem (not in a pakfile) - internal
 */

class KResourceFileZip : public KResourceFile {
public:
   /** Constructor */
   KResourceFileZip (KResourceArchiveZipEntry *lpEntry);

   /** Destructor */
   ~KResourceFileZip ();

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
    * Read from this file, at the current read/write file offset, and return the number of bytes read
    *
    * \param lpBuffer buffer to read into
    * \param nBytes number of bytes to be read
    *
    * \return number of bytes actually read
    */
   long readAndCount (void *lpBuffer, size_t nBytes);

private:
   /** Zipfile entry with the file contents */
   KResourceArchiveZipEntry *_lpEntry;

   /** File size */
   long _nSize;

   /** Current offset within file */
   long _nOffset;
};

/**
 * Base class for modeling a source of files (filesystem or pakfile) - internal
 */

class KResourceArchiveZip : public KResourceArchive {
   friend class KResourceFileZip;

public:
   /** Constructor */
   KResourceArchiveZip ();

   /** Destructor */
   ~KResourceArchiveZip ();

   /**
    * Try opening specified pakfile using this archive format
    *
    * \param lpszArchiveFilename name of pakfile to be opened
    * \param nProperties insertion properties
    * \param lpszPassword pakfile password, if any
    *
    * \return K_RES_OK if successful, or a K_RES_xxx error code
    */
   virtual KResourceResult openArchive (const char *lpszArchiveFilename, long nProperties, const char *lpszPassword);

   /**
    * List the files and folders in this archive in the specified path
    *
    * \param lpszPathName path to examine
    * \param lpEnumCallBack method called back for every entry found
    * \param lpCallBackData data passed back to the callback method
    *
    * \return true to continue listing other pakfiles, false to stop enumerating
    */
   virtual bool enumerateFolder (const char *lpszPathName, enumProc lpEnumCallBack, void *lpCallBackData = NULL);

   /**
    * Get information on the specified file
    *
    * \param lpszFileName name of file to get information about
    * \param lpStat information filled by this method if successful
    *
    * \return true for success, false for failure
    */
   virtual bool statFile (const char *lpszFileName, KResourceStat *lpStat);

   /**
    * Attempt to open specified resource file from the filesystem, and create an instance for the opened resource file
    *
    * \param lpszFileName name of resource file to be opened
    *
    * \return instance if successfully opened for reading from the filesystem, NULL if not
    */
   virtual KResourceFile *openFile (const char *lpszFileName);

   /**
    * Load file in its entirety
    *
    * \param lpszFileName name of file to be loaded
    * \param lpBuffer returned buffer holding file's contents, if successful
    * \param nBufferSize returned size of buffer, if successful
    *
    * \return true for success, false for failure
    */
   virtual bool loadFile (const char *lpszFileName, unsigned char *&lpBuffer, size_t &nBufferSize);

private:
   /** Free all entries and close archive */  
   void cleanup (void);
   
   /** Zip file open callback */
   static void *zipOpenFile (void *opaque, const char* filename, int mode);
   
   /** Zip file read callback */
   static unsigned long zipReadFile (void *opaque, void *stream, void *buf, unsigned long size);
   
   /** Zip file write callback */
   static unsigned long zipWriteFile (void *opaque, void *stream, const void *buf, unsigned long size);
   
   /** Zip file tell callback */
   static long zipTellFile (void *opaque, void *stream);
   
   /** Zip file seek callback */
   static long zipSeekFile (void *opaque, void *stream, unsigned long offset, int origin);
   
   /** Zip file close callback */
   static int zipCloseFile (void *opaque, void *stream);
   
   /** Zip file error testing callback */
   static int zipErrorFile (void *opaque, void *stream);
      
   /** Open zip file */
   k_unzFile _lpZipFile;

   /** Zip password */
   char _szPassword[256];

   /** List of all files in the archive */
   KList<KResourceArchiveZipEntry> _lEntries;

   /** Hashtable of all files in the archive */
   KHashTable _hashEntries;

   /** Lock for serializing access */
   static KSysLock *g_lpLock;

   /** Number of instances of KResourceArchiveZip */
   static long g_nInstances;
};

/**
 * Class for registering zip pakfiles as a source of files - internal
 */

class KResourceArchiveManagerZip : public KResourceArchiveManager {
public:
   /**
    * Instanciate an archive
    *
    * \return archive
    */
   virtual KResourceArchive *createArchive (void);
};

#endif   /* _KANJI_KRESOURCEFILEZIP_H */
