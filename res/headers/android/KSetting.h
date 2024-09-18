/**
 * Kanji rendering and I/O engine - global settings management definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSETTING_H
#define  _KANJI_KSETTING_H

/* Include definitions */
#include "KObjectBase.h"
#include "KDictionary.h"

/* Forward declarations */
class KWindowD3D;
class KWindowD3D9;
class KWindowD3D11;
class KWindowGL;
class KWindowGLES;
class KWindowGLES2;
class KResource;

/**
 * Class managing global settings for the game
 */

class KSetting {
   friend class KWindowD3D;
   friend class KWindowD3D9;
   friend class KWindowD3D11;
   friend class KWindowGL;
   friend class KWindowGLES;
   friend class KWindowGLES2;
   friend class KResource;

public:
   /**
    * Set path of file that contains the game's settings
    *
    * \param lpszPath full path to file
    */
   static void initSettings (const char *lpszPath);
   
   /**
    * Read setting as a string
    *
    * \param lpszKey key
    * \param lpszDefaultValue default value
    *
    * \return value
    */
   static const char *readString (const char *lpszKey, const char *lpszDefaultValue = NULL);

   /**
    * Write setting as a string
    *
    * \param lpszKey key
    * \param lpszValue value
    */
   static void writeString (const char *lpszKey, const char *lpszValue);

   /**
    * Read setting as an integer
    *
    * \param lpszKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   static int readInt (const char *lpszKey, long nDefaultValue = 0);

   /**
    * Write setting as aninteger
    *
    * \param lpszKey key
    * \param nValue value
    */
   static void writeInt (const char *lpszKey, int nValue);

   /**
    * Read setting as a long integer
    *
    * \param lpszKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   static long readLong (const char *lpszKey, long nDefaultValue = 0);

   /**
    * Write setting as a long integer
    *
    * \param lpszKey key
    * \param nValue value
    */
   static void writeLong (const char *lpszKey, long nValue);

   /**
    * Read setting as a 64-bit integer
    *
    * \param lpszKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   static kdictionary_int64_t readInt64 (const char *lpszKey, kdictionary_int64_t nDefaultValue = 0);

   /**
    * Write setting as a 64-bit integer
    *
    * \param lpszKey key
    * \param nValue value
    */
   static void writeInt64 (const char *lpszKey, kdictionary_int64_t nValue);

   /**
    * Read setting as a float
    *
    * \param lpszKey key
    * \param fDefautValue default value
    *
    * \return value
    */
   static float readFloat (const char *lpszKey, float fDefautValue = 0.0f);

   /**
    * Write setting as a float
    *
    * \param lpszKey key
    * \param fValue value
    */
   static void writeFloat (const char *lpszKey, float fValue);

   /**
    * Read setting as a float
    *
    * \param lpszKey key
    * \param fDefautValue default value
    *
    * \return value
    */
   static double readDouble (const char *lpszKey, double fDefautValue = 0.0f);

   /**
    * Write setting as a float
    *
    * \param lpszKey key
    * \param fValue value
    */
   static void writeDouble (const char *lpszKey, double fValue);

   /**
    * Read setting as a boolean
    *
    * \param lpszKey key
    * \param bDefaultValue default value
    *
    * \return value
    */
   static bool readBool (const char *lpszKey, bool bDefaultValue = false);

   /**
    * Write setting as a boolean
    *
    * \param lpszKey key
    * \param bValue value
    */
   static void writeBool (const char *lpszKey, bool bValue);

   /**
    * Save all settings. This method only needs to be called manually if you want to make sure settings are atomically saved; otherwise,
    * settings are always persisted when the app is backgrounded or exited
    */
   static void saveSettings (void);

private:
   /**
    * Attempt to load settings
    *
    * \param lpszPath lpszPath full path to settings file
    *
    * \return true if loaded, false if failed
    */
   static bool loadSettings (const char *lpszPath);

   /**
    * Add string to checksum being accumulated
    *
    * \param nCurrentSum current checksum
    * \param lpszString string to add
    *
    * \return new checksum
    */
   static int addToChecksum(int nCurrentHash, const char *lpszString);

   /**
    * Free all resources used by settings
    */
   static void freeSettings (void);
};

#endif   /* _KANJI_KSETTING_H */

