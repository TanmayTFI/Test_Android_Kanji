/**
 * Kanji rendering and I/O engine - filesystem handler definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KRESOURCEFILEDIRECT_H
#define  _KANJI_KRESOURCEFILEDIRECT_H

/* Include definitions */
#include "KResource.h"

/**
 * Class for modeling a currently open file associated with a resource, that is present directly on the filesystem (not in a pakfile) - internal
 */

class KResourceFileDirect : public KResourceFile {
public:
   /** Constructor */
   KResourceFileDirect (FILE *f);

   /** Destructor */
   ~KResourceFileDirect ();

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

private:
   /** Open file */
   FILE *_f;
};

/**
 * Base class for modeling a source of files (filesystem or pakfile) - internal
 */

class KResourceArchiveDirect : public KResourceArchive {
public:
   /** Constructor */
   KResourceArchiveDirect ();

   /** Destructor */
   ~KResourceArchiveDirect ();

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
   virtual bool loadFile (const char *lpszFileName, unsigned char *&lpBuffer, size_t &nBufferSize);
};

#endif   /* _KANJI_KRESOURCEFILEDIRECT_H */
