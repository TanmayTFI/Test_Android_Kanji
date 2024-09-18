/**
 * Kanji rendering and I/O engine - tar pakfile handler definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KRESOURCEFILETAR_H
#define  _KANJI_KRESOURCEFILETAR_H

/* Include definitions */
#include "KResource.h"

/** Size of a tar header and record */
#define  K_TAR_RECORDSIZE  512

/** Link type: normal file */
#define  K_TAR_LINK_NORMAL    '0'

/** Link type: normal file (old encoding) */
#define  K_TAR_LINK_OLDNORMAL 0

/** Link type: directory */
#define  K_TAR_LINK_DIRECTORY '5'

/** Tar entry header */
typedef union {
   /** Entry header */
   struct {
      /* Original tar format */
      char cFileName[100];
      char cMode[8];
      char cOwnerId[8];
      char cGroupId[8];
      char cFileSize[12];
      char cLastChangeTime[12];
      char cChecksum[8];
      char cLinkType;
      char cLinkedName[100];

      /* UStar extensions */
      char cUStarSignature[6];
      char cUStarVersion[2];
      char cOwnerName[32];
      char cGroupName[32];
      char cDevMajor[8];
      char cDevMinor[8];
      char cFileNamePrefix[155];
   } header;

   /** Equivalent in bytes, to pad to the right size */
   char cBytes[K_TAR_RECORDSIZE];
} KTarHeader;

/** File or directory present in the tar archive */
class KTarEntry : public KObjectListable {
public:
   /** Name of this entry */
   char szFileName[256];

   /** Offset of the first byte of the file in the archive */
   long nStartOffset;

   /** Size of the file in bytes */
   long nSize;

   /** Last modification time */
   time_t nLastChangeTime;

   /** true if entry is a directory, false if it is a file */
   bool bIsDirectory;
};

/**
 * Class for modeling a currently open file associated with a resource, that is stored in a tape archive (tar) pakfile - internal
 */

class KResourceFileTar : public KResourceFile {
public:
   /** Constructor */
   KResourceFileTar (FILE *f, long nBase, long nSize);

   /** Destructor */
   ~KResourceFileTar ();

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
   /** Open pakfile */
   FILE *_f;

   /** Offset of first byte of this file within the tar pakfile */
   long _nBase;

   /** File size */
   long _nSize;

   /** Current offset within file */
   long _nOffset;
};

/**
 * Class representing a tar pakfile - internal
 */

class KResourceArchiveTar : public KResourceArchive {
public:
   /** Constructor */
   KResourceArchiveTar ();

   /** Destructor */
   ~KResourceArchiveTar ();

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

   /** Open tar archive */
   FILE *_f;

   /** List of available resource files within the archive */
   KList<KTarEntry> _lEntries;
};


/**
 * Class for registering tar pakfiles as a source of files - internal
 */

class KResourceArchiveManagerTar : public KResourceArchiveManager {
public:
   /**
    * Instanciate an archive
    *
    * \return archive
    */
   virtual KResourceArchive *createArchive (void);
};

#endif   /* _KANJI_KRESOURCEFILETAR_H */
