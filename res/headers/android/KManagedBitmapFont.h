/**
 * Kanji game engine - managed bitmap text font definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDBITMAPFONT_H
#define  _KANJI_KMANAGEDBITMAPFONT_H

/* Include definitions */
#include "KPTK.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedBitmapFontList;
class KTrueText;

/** Managed bitmap text font, with reference counting */

class KManagedBitmapFont : public KObjectHashable {
   friend class KManagedBitmapFontList;

private:
   /** Filename */
   char szFilePath[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** KText holding the font */
   KText *lpFont;
};

/** List of managed bitmap fonts */

class KManagedBitmapFontList : public KList<KManagedBitmapFont> {
public:
   /** Constructor */
   KManagedBitmapFontList ();

   /** Destructor */
   ~KManagedBitmapFontList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to relative filenames, so that the filename can be used as a key to retrieve a font
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Load bitmap text font from a file or add a reference to it. The text font is fully decompressed in memory
    *
    * \param lpszFilename name of file to load text font from
    * \param lpszGraphicFilename name of file to load first font graphic from (for instance font1.png)
    * \param nCompatibilityLevel font compatibility level (defaults to 0 to support older games)
    * \param newKText method for creating a instance of a class derived from KText if no reference to the font already exists, NULL for default (create KText)
    * \param bHiQuality true to load font graphics as truecolor, false to load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the font graphics, false not to
    *
    * \return bitmap text font, or NULL if loading failed
    */
   KText *loadFont (const char *lpszFilename, const char *lpszGraphicFilename = NULL, long nCompatibilityLevel = 0, KText *(*newKText)(void) = NULL,
                    bool bHiQuality = true, bool bGenerateMipMaps = false);

   /**
    * Get bitmap text font from its filename and height
    *
    * \param lpszFilename filename of text font to get
    *
    * \return bitmap text font, NULL if not currently loaded
    */
   KText *getFontByName (const char *lpszFilename);

   /**
    * Dereference or unload bitmap text font by reference
    *
    * \param lpFont bitmap text font to be dereferenced or unloaded
    */
   void unloadFontByRef (KText *lpFont);

   /**
    * Dereference or unload bitmap text font by filename
    *
    * \param lpszFilename filename of bitmap text font to be dereferenced or unloaded
    */
   void unloadFontByName (const char *lpszFilename);

   /**
    * Enumerate currently loaded bitmap text fonts in this list
    *
    * \param enumCallback method called back for each bitmap text font with user data, the filename, reference count, KText and font height
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KText *lpFont), void *lpUserData);

   /** Free all loaded bitmap text fonts */
   void freeAll (void);

private:
   /** Thread safety lock for accessing this list of bitmap text fonts */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Hashtable of all managed bitmap text fonts in this list */
   KHashTable _hash;
};

#endif   /* _KANJI_KMANAGEDBITMAPFONT_H */
