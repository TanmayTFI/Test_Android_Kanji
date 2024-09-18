/**
 * Kanji game engine - managed video definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDVIDEO_H
#define  _KANJI_KMANAGEDVIDEO_H

/* Include definitions */
#include "KPTK.h"
#include "KSoundBase.h"
#include "KVideoBase.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedVideoList;
class KVideo;

/** Managed video, with reference counting */

class KManagedVideo : public KObjectHashable {
   friend class KManagedVideoList;

private:
   /** Filename */
   char szFilePath[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** KVideo holding the color video */
   KVideoBase *lpVideo;

   /** KVideo holding the alpha channel video, NULL for none */
   KVideoBase *lpAlphaVideo;

   /** KSound for playing the sound stream, NULL for none */
   KSoundBase *lpSound;
};

/** List of managed videos */

class KManagedVideoList : public KList<KManagedVideo> {
public:
   /** Constructor */
   KManagedVideoList ();

   /** Destructor */
   ~KManagedVideoList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to relative filenames, so that the filename can be used as a key to retrieve a video
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Load specified video file or add a reference to it. The loaded video is automatically set to the paused state and resumeVideo() must be used to start playback.
    * If an alpha video is present, it is automatically loaded and used provided a filename for the alpha video is supplied or it is named properly (for instance,
    * cube.ogv for color and cube_a.ogv for the alpha channel).
    *
    * \param lpszFilename name of file to load video from
    * \param bLoopVideo true to loop the video indefinitely, false to play it once and report the end using isEndReached()
    * \param bPlayAudio true to play audio stream (if any), false not to
    * \param bSeekable true to make video seekable into with seekToTime() and seekToFrame(), false if not needed
    * \param lpszAlphaFilename name of file to load alpha channel video from, NULL to use default "_a" suffix
    * \param nFormat video format (KVIDEO_UNKNOWN to autodetect from the filename, default)
    *
    * \return video for success, NULL for failure
    */
   KVideo *loadVideo (const char *lpszFilename, bool bLoopVideo, bool bPlayAudio, bool bSeekable, const char *lpszAlphaFilename = NULL, long nFormat = -1) {
      KVideo *lpVideo;
      extern void KVideo_register (void);

      if (bPlayAudio) {
         extern void KSound_register (void);

         KSound_register ();  /* Make sure KSound is linked in and available */
      }

      KVideo_register ();   /* Make sure KVideo is linked in and available */
      lpVideo = loadVideoInternal (lpszFilename, bLoopVideo, bPlayAudio, bSeekable, lpszAlphaFilename, nFormat);
      return lpVideo;
   }

   /**
    * Get video from its filename
    *
    * \param lpszFilename filename of video to get
    *
    * \return video, NULL if not currently loaded
    */
   KVideo *getVideoByName (const char *lpszFilename);

   /**
    * Dereference or unload video by reference
    *
    * \param lpVideo video to be dereferenced or unloaded
    */
   void unloadVideoByRef (KVideo *lpVideo);

   /**
    * Dereference or unload video by filename
    *
    * \param lpszFilename filename of video to be dereferenced or unloaded
    */
   void unloadVideoByName (const char *lpszFilename);

   /**
    * Enumerate currently loaded videos in this list
    *
    * \param enumCallback method called back for each video with user data, the filename, reference count and KVideo
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KVideo *lpVideo), void *lpUserData);

   /** Free all loaded videos */
   void freeAll (void);

private:
   /**
    * Load specified video file or add a reference to it. The loaded video is automatically set to the paused state and resumeVideo() must be used to start playback.
    * If an alpha video is present, it is automatically loaded and used provided a filename for the alpha video is supplied or it is named properly (for instance,
    * cube.ogv for color and cube_a.ogv for the alpha channel).
    *
    * \param lpszFilename name of file to load video from
    * \param bLoopVideo true to loop the video indefinitely, false to play it once and report the end using isEndReached()
    * \param bPlayAudio true to play audio stream (if any), false not to
    * \param bSeekable true to make video seekable into with seekToTime() and seekToFrame(), false if not needed
    * \param lpszAlphaFilename name of file to load alpha channel video from, NULL to use default "_a" suffix
    * \param nFormat video format (KVIDEO_UNKNOWN to autodetect from the filename, default)
    *
    * \return video for success, NULL for failure
    */
   KVideo *loadVideoInternal (const char *lpszFilename, bool bLoopVideo, bool bPlayAudio, bool bSeekable, const char *lpszAlphaFilename, long nFormat);

   /** Thread safety lock for accessing this list of videos */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Buffer for composing alpha video pathnames */
   char _szAlphaPathBuffer[K_MAXPATH];

   /** Hashtable of all managed videos in this list */
   KHashTable _hash;
};

#endif   /* _KANJI_KMANAGEDVIDEO_H */
