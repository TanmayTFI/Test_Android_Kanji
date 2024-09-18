/**
 * Kanji game engine - managed text font definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDFONT_H
#define  _KANJI_KMANAGEDFONT_H

/* Include definitions */
#include "KPTK.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedFontList;
class KTrueText;

/** Managed text font, with reference counting */

class KManagedFont : public KObjectHashable {
   friend class KManagedFontList;

private:
   /** Filename */
   char szFilePath[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** KTrueText holding the font */
   KTrueText *lpFont;

   /** Font height in pixels */
   long nHeightPix;

   /** Unique identifier */
   long nIdentifier;
};

/** List of managed fonts */

class KManagedFontList : public KList<KManagedFont> {
public:
   /** Constructor */
   KManagedFontList ();

   /** Destructor */
   ~KManagedFontList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to relative filenames, so that the filename can be used as a key to retrieve a font
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Load text font from a file or add a reference to it. The text font is fully decompressed in memory
    *
    * \param lpszFilename name of file to load text font from
    * \param nHeightPix height of the font in pixels
    * \param bDeferredRendering true to enable deferred rendering (see KTrueText::setDeferredRendering()), false not to (default)
    * \param nIdentifier unique identifier; used to load multiple copies of a font of the same height, but applying different effects for instance
    * \param bUseKerningPairs true to use kerning pairs if the information is available in the font, false not to (default)
    * \param nCompatibilityLevel font compatibility level (defaults to 0 to support older games)
    * \param bHiQuality true to load font graphics as truecolor, false to load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the font graphics, false not to
    *
    * \return text font, or NULL if loading failed
    */
   KTrueText *loadFont (const char *lpszFilename, long nHeightPix, bool bDeferredRendering = false, long nIdentifier = 0, bool bUseKerningPairs = false, long nCompatibilityLevel = 0,
                        bool bHiQuality = true, bool bGenerateMipMaps = false);

   /**
    * Get text font from its filename and height
    *
    * \param lpszFilename filename of text font to get
    * \param nHeightPix height of the font in pixels
    * \param nIdentifier unique identifier; used to load multiple copies of a font of the same height, but applying different effects for instance
    *
    * \return text font, NULL if not currently loaded
    */
   KTrueText *getFontByName (const char *lpszFilename, long nHeightPix, long nIdentifier = 0);

   /**
    * Dereference or unload text font by reference
    *
    * \param lpFont text font to be dereferenced or unloaded
    */
   void unloadFontByRef (KTrueText *lpFont);

   /**
    * Dereference or unload text font by filename
    *
    * \param lpszFilename filename of text font to be dereferenced or unloaded
    * \param nHeightPix height of the font in pixels
    * \param nIdentifier unique identifier; used to load multiple copies of a font of the same height, but applying different effects for instance
    */
   void unloadFontByName (const char *lpszFilename, long nHeightPix, long nIdentifier = 0);

   /**
    * Enumerate currently loaded text fonts in this list
    *
    * \param enumCallback method called back for each text font with user data, the filename, reference count, KTrueText and font height
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KTrueText *lpFont, long nHeightPix), void *lpUserData);

   /** Free all loaded text fonts */
   void freeAll (void);

private:
   /** Thread safety lock for accessing this list of text fonts */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Hashtable of all managed text fonts in this list */
   KHashTable _hash;
};

#endif   /* _KANJI_KMANAGEDFONT_H */
