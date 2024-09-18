/**
 * Kanji game engine - managed sound effect definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDSAMPLE_H
#define  _KANJI_KMANAGEDSAMPLE_H

/* Include definitions */
#include "KPTK.h"
#include "KSoundBase.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedSampleList;
class KSound;

/** Managed sound effect, with reference counting */

class KManagedSample : public KObjectHashable {
   friend class KManagedSampleList;

private:
   /** Filename */
   char szFilePath[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** KSound holding the sample */
   KSoundBase *lpSample;
};

/** List of managed sound effects */

class KManagedSampleList : public KList<KManagedSample> {
public:
   /** Constructor */
   KManagedSampleList ();

   /** Destructor */
   ~KManagedSampleList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to relative filenames, so that the filename can be used as a key to retrieve a sound effect
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Load sound effect from a file or add a reference to it. The sound effect is fully decompressed in memory
    *
    * \param lpszFilename name of file to load sound effect from
    * \param nVolume initial volume (0..100)
    * \param bLoop true to loop sound indefinity when playing, false to only play once and then stop
    * \param nMaxChannels number of times that this sound can be played in parallel on different audio channels; if more playSample() calls are issued than
    *                     this number, one of the samples will be stopped and re-started
    *
    * \return sound effect, or NULL if loading failed
    */
   KSound *loadSample (const char *lpszFilename, long nVolume = 100, bool bLoop = false, long nMaxChannels = 1) {
      KSound *lpSample;
      extern void KSound_register (void);

      KSound_register ();   /* Make sure KSound is linked in and available */
      lpSample = loadSampleInternal (lpszFilename, nVolume, bLoop, nMaxChannels);
      return lpSample;
   }

   /**
    * Get sound effect from its filename
    *
    * \param lpszFilename filename of sound effect to get
    *
    * \return sound effect, NULL if not currently loaded
    */
   KSound *getSampleByName (const char *lpszFilename);

   /**
    * Dereference or unload sound effect by reference
    *
    * \param lpSample sound effect to be dereferenced or unloaded
    */
   void unloadSampleByRef (KSound *lpSample);

   /**
    * Dereference or unload sound effect by filename
    *
    * \param lpszFilename filename of sound effect to be dereferenced or unloaded
    */
   void unloadSampleByName (const char *lpszFilename);

   /**
    * Enumerate currently loaded sound effects in this list
    *
    * \param enumCallback method called back for each sound effect with user data, the filename, reference count and KSound
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KSound *lpSample), void *lpUserData);

   /** Free all loaded sound effects */
   void freeAll (void);

private:
   /**
    * Load sound effect from a file or add a reference to it. The sound effect is fully decompressed in memory
    *
    * \param lpszPrefix path prepended to the filename, so that the filename can be used as a key to retrieve the sound effect
    * \param lpszFilename name of file to load sound effect from
    * \param nVolume initial volume (0..100)
    * \param bLoop true to loop sound indefinity when playing, false to only play once and then stop
    * \param nMaxChannels number of times that this sound can be played in parallel on different audio channels; if more playSample() calls are issued than
    *                     this number, one of the samples will be stopped and re-started
    *
    * \return sound effect, or NULL if loading failed
    */
   KSound *loadSampleInternal (const char *lpszFilename, long nVolume, bool bLoop, long nMaxChannels);

   /** Thread safety lock for accessing this list of sound effects */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Hashtable of all managed sound effects in this list */
   KHashTable _hash;
};

#endif   /* _KANJI_KMANAGEDSAMPLE_H */
