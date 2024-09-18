/**
 * Kanji rendering and I/O engine - base class for video services - internal
 *
 * (c) 2010
 */

#ifndef  _KANJI_KVIDEOBASE_H
#define  _KANJI_KVIDEOBASE_H

/** \cond private */

/* Include definitions */
#include "KObjectListable.h"

/* Forward declarations */
class KGraphic;
class KVideo;
class KSoundBase;

/**
 * Base class for playing videos in a Kanji graphic
 */

class KVideoBase : public KObjectListable {
public:
   /**
    * Open specified video file for playback
    *
    * \param lpszFileName name of file to load video from
    * \param bPreload true to entirely load the compressed video file in memory (and avoid disk access during playback),
    *                 false to stream from disk as needed
    * \param lpAlphaVideo second, open video stream to be attached to this one, whose luminance values provides alpha (transparency) information.
    * \param lpSoundStream sound instance to decode audio stream to, NULL to ignore any audio stream
    * \param bLoopVideo true to loop the video indefinitely, false to play it once and report the end using isEndReached()
    * \param bSeekable true to make video seekable into with seekToTime() and seekToFrame(), false if not needed
    * \param nFormat video format (KVIDEO_UNKNOWN to autodetect from the filename, default)
    * \param bPause true to not start playing video until resumeTime() is called; false to play immediately (default)
    *
    * \return true for success, false for failure
    */
   virtual bool openVideo (const char *lpszFileName, bool bPreload = false, KVideo *lpAlphaVideo = NULL, KSoundBase *lpSoundStream = NULL, bool bLoopVideo = false, bool bSeekable = false,
                           long nFormat = -1, bool bPause = false) = 0;

   /**
    * Get width of the video frame, in pixels
    *
    * \return width (0 if no video is currently open)
    */
   virtual float getFrameWidth (void) = 0;

   /**
    * Get height of the video frame, in pixels
    *
    * \return height (0 if no video is currently open)
    */
   virtual float getFrameHeight (void) = 0;

   /**
    * Get graphic used for rendering this video
    *
    * \return graphic (NULL if no video is currently open)
    */
   virtual KGraphic *getKGraphic (void) = 0;

   /** 
    * Read and decode the next frame in this video
    */
   virtual void readFrame (void) = 0;

   /**
    * Check if the end of the video stream has been reached yet
    *
    * \return true if end was reached, false if not
    */
   virtual bool isEndReached (void) = 0;

   /**
    * Check if at least a frame of video has been rendered to the graphic yet
    *
    * \return true if a frame has been rendered yet, false if not
    */
   virtual bool isVideoAvailable (void) = 0;

   /**
    * Start reading frames from the beginning of the video again
    */
   virtual void rewind (void) = 0;

   /**
    * Pause the video timer. This must be called if readFrames() is not going to
    * be called for a while, for instance because the game lost focus
    */
   virtual void pauseTime (void) = 0;

   /**
    * Resume the video timer
    */
   virtual void resumeTime (void) = 0;

   /**
    * Check if video is currently paused
    *
    * \return true if paused, false if not
    */
   virtual bool isVideoPaused (void) = 0;

   /**
    * Get current time since the beginning of playback, in seconds
    *
    * \return time in seconds
    */
   virtual double getTime (void) = 0;

   /**
    * Get total duration, in seconds. The video must have been opened with the bSeekable flag set
    *
    * \return video duration in seconds
    */
   virtual double getTotalTime (void) = 0;

   /**
    * Seek to the specified time index, in seconds. The video must have been opened with the bSeekable flag set
    *
    * \param fTime time index to seek to, in seconds
    */
   virtual void seekToTime (double fTime) = 0;

   /**
    * Get currently displayed frame
    *
    * \return current frame
    */
   virtual unsigned long getFrame (void) = 0;

   /**
    * Get total number of frames. The video must have been opened with the bSeekable flag set
    *
    * \return video duration as number of frames
    */
   virtual unsigned long getTotalFrames (void) = 0;

   /**
    * Seek to the specified frame. The video must have been opened with the bSeekable flag set
    *
    * \param nFrame frame number to seek to
    */
   virtual void seekToFrame (unsigned long nFrame) = 0;

   /**
    * Get sound stream associated with this video, if any
    *
    * \return sound stream, NULL for none
    */
   virtual KSoundBase *getSoundStream (void) = 0;
};

/** \endcond */

#endif   /* _KANJI_KVIDEOBASE_H */
