/**
 * Kanji rendering and I/O engine - .ini file reader definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KINIREADER_H
#define  _KANJI_KINIREADER_H

/* Include definitions */
#include "KObjectBase.h"
#include "KWindow.h"
#include "KHashTable.h"
#include "KHashTableLong.h"
#include "KList.h"

/** \cond private */

/** One entry in the list of section settings for text (literal) files */

class KIniTextSetting : public KObjectListable {
public:
   /** Key */
   const char *lpszSetting;

   /** Value */
   const char *lpszValue;
};

/** One entry in the list of sections for text (literal) files */

class KIniTextSection : public KObjectHashable {
public:
   /** Name */
   const char *lpszSection;

   /** List of settings */
   KList<KIniTextSetting> lSettings;
};

/** One entry in the hashtable for binary (compressed) files */

class KIniBinHashEntry : public KObjectHashableLong {
public:
   /** Value */
   const char *lpszValue;
};

/** \endcond */

/** Compression mode for KIniReader::writeIni() and KIniReader::compress() */
typedef enum {
   K_INI_TEXT = 0,               /**< Save ini in text form (for writeIni only) */
   K_INI_BINARY,                 /**< Save ini in binary form */
   K_INI_DEFLATE,                /**< Save ini in binary form, and deflate with zlib */
   K_INI_DEFLATE_AES,            /**< Save ini in binary form, deflate with zlib, and encrypt using AES */
} KIniCompressionMode;

/**
 * Class for loading and saving image files
 */

class KIniReader : public KObjectBase {
public:
   /** Constructor */
   KIniReader ();

   /** Destructor */
   ~KIniReader ();

   /** Reset contents */
   void clear (void);

   /**
    * Load specified .ini from a file
    *
    * \param lpszFileName name of .ini file to be loaded
    * \param lpEncryptionKey encryption key, when reading a .ini file with encryption
    * \param nEncryptionKeySize size of encryption key in bytes, when reading a .ini file with encryption
    *
    * \return true for success, false for failure
    */
   bool setIni (const char *lpszFileName, const char *lpEncryptionKey = NULL, size_t nEncryptionKeySize = 0);

   /**
    * Load specified .ini from a memory buffer
    *
    * \param lpIniBuffer buffer containing .ini text
    * \param nBufferLength length of buffer, in bytes
    * \param lpEncryptionKey encryption key, when reading a .ini file with encryption
    * \param nEncryptionKeySize size of encryption key in bytes, when reading a .ini file with encryption
    *
    * \return true for success, false for failure
    */
   bool setIni (const char *lpIniBuffer, long nBufferLength, const char *lpEncryptionKey = NULL, size_t nEncryptionKeySize = 0);

   /**
    * Check if the specified setting has a value defined
    *
    * \param lpszSection name of section to check setting from
    * \param lpszSetting name of setting
    *
    * \return true if setting has a value defined, false if not
    */
   bool hasValue (const char *lpszSection, const char *lpszSetting);

   /**
    * Read value of a setting in a specific section as an integer
    *
    * \param lpszSection name of section to read setting from
    * \param lpszSetting name of setting
    * \param nDefaultValue value to return, in case the section doesn't exist, setting doesn't exist within the section, or cannot be parsed
    *
    * \return setting's integer value
    */
   long getValue (const char *lpszSection, const char *lpszSetting, long nDefaultValue = 0);

   /**
    * Read value of a setting in a specific section as a floating point value
    *
    * \param lpszSection name of section to read setting from
    * \param lpszSetting name of setting
    * \param fDefaultValue value to return, in case the section doesn't exist, setting doesn't exist within the section, or cannot be parsed
    *
    * \return setting's integer value
    */
   float getFloatValue (const char *lpszSection, const char *lpszSetting, float fDefaultValue = 0.0f);

   /**
    * Read value of a setting in a specific section as a string
    *
    * \param lpszSection name of section to read setting from
    * \param lpszSetting name of setting
    * \param lpStrBuffer buffer where the string value will be stored, if successful,
    * \param nMaxStrBufferSize maximum size the string buffer can hold, including terminal 0
    *
    * \return true if value was read and a string was returned, false if not
    */
   bool getString (const char *lpszSection, const char *lpszSetting, char *lpStrBuffer, size_t nMaxStrBufferSize);

   /**
    * Write value of a setting in a specific section as an integer
    *
    * \param lpszSection name of section to write setting to
    * \param lpszSetting name of setting
    * \param nValue new value of setting
    *
    * \return true for success, false for failure (trying to write to a compressed .ini)
    */
   bool setValue (const char *lpszSection, const char *lpszSetting, long nValue);

   /**
    * Read value of a setting in a specific section as a floating point value
    *
    * \param lpszSection name of section to write setting to
    * \param lpszSetting name of setting
    * \param fValue new value of setting
    *
    * \return true for success, false for failure (trying to write to a compressed .ini)
    */
   bool setFloatValue (const char *lpszSection, const char *lpszSetting, float fValue);

   /**
    * Write value of a setting in a specific section using the specified printf-like format
    *
    * \param lpszSection name of section to write setting to
    * \param lpszSetting name of setting
    * \param lpszFormat printf-like format for value
    * \param ... value
    *
    * \return true for success, false for failure (trying to write to a compressed .ini)
    */
   bool setFormattedValue (const char *lpszSection, const char *lpszSetting, const char *lpszFormat, ...);

   /**
    * Read value of a setting in a specific section as a string
    *
    * \param lpszSection name of section to write setting to
    * \param lpszSetting name of setting
    * \param lpszValue new value of setting
    *
    * \return true for success, false for failure (trying to write to a compressed .ini)
    */
   bool setString (const char *lpszSection, const char *lpszSetting, const char *lpszValue);

   /**
    * Write this .ini to a file
    *
    * \param lpszFileName filename of text ini file to write to
    * \param nCompressionMode compression mode (K_INI_xxx)
    * \param lpEncryptionKey encryption key, for modes with encryption
    * \param nEncryptionKeySize size of encryption key in bytes, for modes with encryption
    *
    * \return true for success, false for failure
    */
   bool writeIni (const char *lpszFileName, KIniCompressionMode nCompressionMode = K_INI_BINARY, const char *lpEncryptionKey = NULL, size_t nEncryptionKeySize = 0);

   /**
    * Compress text ini file to a binary form, if possible
    *
    * \param lpszFileName filename of text ini file to compress
    * \param nCompressionMode compression mode (K_INI_xxx)
    * \param lpEncryptionKey encryption key, for modes with encryption
    * \param nEncryptionKeySize size of encryption key in bytes, for modes with encryption
    *
    * \return true for success, false for failure
    */
   static bool compress (const char *lpszFileName, KIniCompressionMode nCompressionMode = K_INI_BINARY, const char *lpEncryptionKey = NULL, size_t nEncryptionKeySize = 0);

   /**
    * Parse string as a double using '.' as the decimal separator, regardless of the current locale
    *
    * \param lpszString string to parse
    *
    * \return double
    */
   static double atofNoLocale (const char *lpszString);

private:
   /** Parse text file contents for faster lookup */
   void parseText (void);

   /**
    * Allocate memory for string, copy it and return copy
    *
    * \param lpszString string to copy
    *
    * \return allocated copy
    */
   char *allocCopyString (const char *lpszString);

   /**
    * Parse binary (compressed) file contents
    *
    * \param lpEncryptionKey encryption key, when reading a .ini file with encryption
    * \param nEncryptionKeySize size of encryption key in bytes, when reading a .ini file with encryption
    */
   void parseBinary (const char *lpEncryptionKey, size_t nEncryptionKeySize);

   /**
    * Compress ini text in memory to a binary form, if possible, and save it as a file
    *
    * \param lpszSourceText ini text to compress
    * \param nCompressionMode compression mode (K_INI_xxx)
    * \param lpEncryptionKey encryption key, for modes with encryption
    * \param nEncryptionKeySize size of encryption key in bytes, for modes with encryption
    *
    * \return true for success, false for failure
    */
   static bool compressBuffer (const char *lpszSourceText, const char *lpszFileName, KIniCompressionMode nCompressionMode, const char *lpEncryptionKey, size_t nEncryptionKeySize);

   /**
    * Compute hash value for a section and key
    *
    * \param nSalt salt value to use for hashing
    * \param lpszSection section name
    * \param lpszKey key name
    *
    * \return hash
    */
   static long hashIniSectionKey (long nSalt, const char *lpszSection, const char *lpszKey);

   /** Buffer containing loaded .ini file's data */
   char *_lpIniData;

   /** Size of buffer containing loaded .ini file's data */
   unsigned long _nIniDataSize;

   /** true if file is compressed as a binary, false if file contains text */
   bool _bIsBinary;

   /** Buffer for getting integral and float values from text */
   char _szValueBuf[256];

   /** For text files, list of sections */
   KList<KIniTextSection> _lTextSections;

   /** For text files, hashtable of sections */
   KHashTable _textHashSections;

   /** For binary files, salt for the hash values */
   long _nBinSalt;

   /** For binary files, parsed entries */
   KIniBinHashEntry *_binEntry;

   /** For binary files, hashtable of all keys */
   KHashTableLong _binHashTable;
};

#endif   /* _KANJI_KINIREADER_H */
