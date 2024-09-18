/**
 * Kanji video engine - video player definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KVIDEO_H
#define  _KANJI_KVIDEO_H

/* Include definitions */
#include "KObjectListable.h"
#include "KList.h"
#include "KSysThread.h"
#include "KSysLock.h"
#include "KGraphic.h"
#include "KResource.h"
#include "KSoundBase.h"
#include "KVideoBase.h"
#include <list>
#include <map>

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/** Supported video formats */
enum {
   KVIDEO_UNKNOWN = -1,    /**< Unknown format */
   KVIDEO_THEORA = 0,      /**< Video in ogg theora format (.ogg/.ogv) */
};

/** Number of read-ahead video buffers */
#define  _KVIDEO_NBUFFERS   8

/* Forward declarations */
typedef struct _KVideoStreamState KVideoStreamState;
class KPixelShader;

/** Type for the current 64-bit position of an audio buffer into the decompressed audio data */
#ifdef _WIN32
typedef __int64 KVideoDataPos;
#else
typedef long long KVideoDataPos;
#endif

/* Convert from YUV to RGB in a shader wherever possible */
#if defined (K_WIN32) || defined (K_MACOS) || defined (K_IPHONE) || defined (K_WINRT) || defined (K_ANDROID) || defined (K_LINUX)
#define _KVIDEO_SUPPORT_SHADERS
#endif

/**
 * Class for playing videos in a Kanji graphic
 */

class KVideo : public KVideoBase {
public:
   /** Constructor */
   KVideo ();

   /** Destructor */
   ~KVideo ();

   /**
    * Detect video format from the supplied filename's extension (.ogv)
    *
    * \param lpszFileName filename to detect image format from
    *
    * \return detected format (KVIDEO_xxx), or KVIDEO_UNKNOWN if unsupported
    */
   static long detectFormat (const char *lpszFileName);

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
                           long nFormat = -1, bool bPause = false);

   /**
    * Open specified video from a memory buffer for playback
    *
    * \param lpBuffer buffer containing compressed video stream
    * \param nBufferSize length of buffer, in bytes
    * \param nFormat format of the video data (KVIDEO_xxx)
    * \param lpAlphaVideo second, open video stream to be attached to this one, whose luminance values provides alpha (transparency) information.
    * \param lpSoundStream sound instance to decode audio stream to, NULL to ignore any audio stream
    * \param bLoopVideo true to loop the video indefinitely, false to play it once and report the end using isEndReached()
    * \param bSeekable true to make video seekable into with seekToTime() and seekToFrame(), false if not needed
    * \param bPause true to not start playing video until resumeTime() is called; false to play immediately (default)
    *
    * \return true for success, false for failure
    */
   bool openVideoFromPointer (const unsigned char *lpBuffer, size_t nBufferSize, long nFormat, KVideo *lpAlphaVideo = NULL, KSoundBase *lpSoundStream = NULL, bool bLoopVideo = false,
                              bool bSeekable = false, bool bPause = false);

   /** Close currently open video and free all resources it used */
   void closeVideo (void);

   /**
    * Get width of the video frame, in pixels
    *
    * \return width (0 if no video is currently open)
    */
   virtual float getFrameWidth (void);

   /**
    * Get height of the video frame, in pixels
    *
    * \return height (0 if no video is currently open)
    */
   virtual float getFrameHeight (void);

   /**
    * Get graphic used for rendering this video
    *
    * \return graphic (NULL if no video is currently open)
    */
   virtual KGraphic *getKGraphic (void) { return _lpVideoGraphic; }

   /** 
    * Read and decode the next frame in this video
    */
   virtual void readFrame (void);

   /**
    * Check if the end of the video stream has been reached yet
    *
    * \return true if end was reached, false if not
    */
   virtual bool isEndReached (void);

   /**
    * Check if at least a frame of video has been rendered to the graphic yet
    *
    * \return true if a frame has been rendered yet, false if not
    */
   virtual bool isVideoAvailable (void);

   /**
    * Start reading frames from the beginning of the video again
    */
   virtual void rewind (void);

   /**
    * Get the number of successfully decoded frames so far
    *
    * \return number of decoded frames
    */
   long getDecodedFrames (void);

   /**
    * Get the number of dropped frames so far
    *
    * \return number of dropped frames
    */
   long getDroppedFrames (void);

   /**
    * Pause the video timer. This must be called if readFrames() is not going to
    * be called for a while, for instance because the game lost focus
    */
   virtual void pauseTime (void);

   /**
    * Resume the video timer
    */
   virtual void resumeTime (void);

   /**
    * Check if video is currently paused
    *
    * \return true if paused, false if not
    */
   virtual bool isVideoPaused (void);

   /**
    * Get current time since the beginning of playback, in seconds
    *
    * \return time in seconds
    */
   virtual double getTime (void);

   /**
    * Get total duration, in seconds. The video must have been opened with the bSeekable flag set
    *
    * \return video duration in seconds
    */
   virtual double getTotalTime (void);

   /**
    * Seek to the specified time index, in seconds. The video must have been opened with the bSeekable flag set
    *
    * \param fTime time index to seek to, in seconds
    */
   virtual void seekToTime (double fTime);

   /**
    * Get currently displayed frame
    *
    * \return current frame
    */
   virtual unsigned long getFrame (void);

   /**
    * Get total number of frames. The video must have been opened with the bSeekable flag set
    *
    * \return video duration as number of frames
    */
   virtual unsigned long getTotalFrames (void);

   /**
    * Seek to the specified frame. The video must have been opened with the bSeekable flag set
    *
    * \param nFrame frame number to seek to
    */
   virtual void seekToFrame (unsigned long nFrame);

   /**
    * Get playback speed multiplier for this video
    *
    * \return speed multiplier (defaults to 1.0)
    */
   virtual double getSpeed (void);

   /**
    * Set playback speed multiplier for this video
    *
    * \param fSpeed new speed multiplier (1.0 for normal playback speed)
    */
   virtual void setSpeed (double fSpeed);

   /**
    * Get sound stream associated with this video, if any
    *
    * \return sound stream, NULL for none
    */
   virtual KSoundBase *getSoundStream (void);

   /**
    * For alpha channel streams, set mapping from alpha values to texture buffers
    *
    * \param fMultiplier multiplier, so that buffer_output = (((alpha_value) - 128) * fMultiplier) + 128 + fOffset
    * \param fOffset offset, so that buffer_output = (((alpha_value) - 128) * fMultiplier) + 128 + fOffset
    */
   void setAlphaMapping (float fMultiplier, float fOffset);

   /** Enable the use of pixel shaders to accelerate video decoding, if they are supported by the renderer */
   static void enablePixelShader (void);

   /**
    * Get the pixel shader that must be set when blitting the video data
    *
    * \return pixel shader to set, NULL if none is used
    */
   static KPixelShader *getPixelShader (void);

   /**
    * Get the amount of memory currently used for videos
    *
    * \return total in bytes
    */
   static size_t getTotalVideoBytes (void);
   
   /**
    * Free all lingering resources when not using video anymore. This method may be called by the game just before exiting,
    * to check for memory leaks, otherwise it is not necessary.
    */
   static void cleanup (void);

private:
   /**
    * Get actual system time since the beginning of playback, uncompensated for audio sync, in milliseconds
    *
    * \return time in millis
    */
   double getInternalTime (void);

   /**
    * Read data from the video resource into the ogg sync buffer
    *
    * \return number of bytes read
    */
   size_t readStream (void);

   /**
    * Seek to the specified position in the video resource
    *
    * \param nPos absolute position to seek to
    */
   void seekStream (size_t nPos);

   /**
    * Enqueue specified page into the appropriate ogg stream
    *
    * \param lpPage page to be enqueued
    */
   void queuePage (void *lpPage);

   /**
    * For alpha channel streams, update table for mapping values from video to buffer
    */
   void updateAlphaTable (void);

   /**
    * Write available video frame to buffer
    *
    * \param nBufferIdx index of buffer to write frame to
    */
   void writeVideoFrameToBuffer (long nBufferIdx);

   /**
    * Write available video frame to buffer, as alpha channel information
    *
    * \param nBufferIdx index of buffer to write frame to
    */
   void writeVideoFrameToBufferAsAlpha (long nBufferIdx);

   /**
    * Copy decoded buffer to the Kanji texture used for rendering
    *
    * \param nBufferIdx index of buffer to copy from
    */
   void copyBufferToTexture (long nBufferIdx);

   /**
    * Copy decoded buffer to the Kanji texture used for rendering, using a second stream for alpha channel information
    *
    * \param nBufferIdx index of buffer to copy from, in this stream
    * \param nAlphaBufferIdx index of buffer to copy from, in the second stream
    */
   void copyBufferToTextureWithAlpha (long nBufferIdx, long nAlphaBufferIdx);

   /**
    * Main function for the frame decoder thread
    *
    * \param lpParam pointer parameter (KVideo instance)
    * \param nParam integer parameter (unused)
    */
   static void decodeThreadMain (void *lpParam, long nParam);

   /**
    * Decode one frame of video into the specified buffer
    *
    * \param nVideoBufferIdx video buffer to write to
    * \param nAudioBufferIdx audio buffer to write to
    */
   void decodeFrame (long nVideoBufferIdx, long nAudioBufferIdx);

   /**
    * Supply decoded audio data to Kanji
    *
    * \param lpUserData KVideo instance
    * \param lpBuffer where to write the audio data
    * \param nMaxSize maximum size to be written
    *
    * \return size actually written
    */
   static size_t getAudioStreamPacket(void *lpUserData, char *lpBuffer, size_t nMaxSize);

   /** Frame decoder thread */
   KSysThread *_lpDecodeThread;

   /** Lock serializing access to resources shared between the main thread and the frame decoder thread */
   KSysLock *_lpLock;

   /** true if decoding thread needs to terminate as soon as possible */
   volatile bool _bThreadTerminate;

   /** Current clock, as read by the main thread for both threads */
   volatile double _fCurrentTime;

   /** Currently shown frame, as the absolute frame number in the video */
   unsigned long _nCurrentFrame;

   /** Sound instance to decode audio stream to if present, NULL when ignoring audio streams */
   KSoundBase *_lpSoundStream;

   /** true if the audio stream has started playing already */
   bool _bSoundStreamPlaying;

   /** Resource containing the compressed video */
   KResource _resSrcVideo;

   /** If the compressed video is preloaded, compressed video data */
   unsigned char *_lpSrcVideo;

   /** If the compressed video is preloaded, size of the compressed video data in bytes */
   size_t _nSrcVideoBytes;

   /** If the compressed video is preloaded, current offset into the compressed video data */
   size_t _nSrcVideoCurOffset;

   /** Second video stream that provides alpha channel data to this video stream's color, or NULL for none */
   KVideo *_lpAlphaVideo;

   /** true to decode as alpha channel data, false to decode as color data */
   bool _bDecodeAsAlpha;

   /** true to loop the video indefinitely, false to play it once and report the end using isEndReached() */
   bool _bLoopVideo;

   /** true if a video is currently open for reading, false if not */
   bool _bOpened;

   /** true if no video data is pending in Theora buffers */
   volatile bool _bVideoDataConsumed;

   /** true if no audio data is pending in Vorbis buffers */
   volatile bool _bAudioDataConsumed;

   /** true if no more data can be read from the video file */
   volatile bool _bEof;

   /** Position to seek to (-1 if none) */
   volatile size_t _nSeekToPos;

   /** true if all video frames have been read by the game */
   bool _bEndReached;

   /** true if at least one frame of video has been rendered to the graphic yet */
   bool _bVideoAvailable;

   /** Kanji graphic for rendering the video to */
   KGraphic *_lpVideoGraphic;

   /** While reading, starting system time in milliseconds */
   unsigned long _nStartMillis;

   /** While reading, last system time read, in milliseconds */
   unsigned long _nLastNowMillis;

   /** While reading, system time at which the video time was paused */
   unsigned long _nPausedMillis;

   /** Ogg Theora state for this video stream */
   KVideoStreamState *_lpState;

   /** Number of theora header pages found */
   long _nTheoraPages;

   /** Number of vorbis header pages found */
   long _nVorbisPages;

   /** Current post-processing level */
   int _nPostProcLevel;

   /** Maximum supported post-processing level */
   int _nMaxPostProcLevel;

   /** Post-processing increase to be applied to current level */
   int _nPostProcIncrease;

   /** When looping seamlessly, index of the loop of frames that is being read by the game */
   long _nReadLoopIdx;

   /** When looping seamlessly, index of the loop of frames that is being decoded by frame decoder thread */
   long _nDecodeLoopIdx;

   /** Duration of the video stream, in milliseconds (known after the video has been played once), used for looping */
   double _fVideoStreamDuration;

   /** Total number of bytes decoded in the audio stream */
   size_t _nAudioStreamBytes;

   /** Duration of the video stream, in seconds (reported to the game, used for seeking) */
   double _fTotalTime;

   /** Number of frames in the video stream (reported to the game) */
   unsigned long _nTotalFrames;

   /** List of indices of video buffers that currently contain decoded frames */
   std::list<long> _lReadyVideoBuf;

   /** List of indices of video buffers that are currently free */
   std::list<long> _lFreeVideoBuf;

   /** Map of frame times for seeking to a specific time index */
   std::map<double,size_t> _lSeekFrameTime;

   /** Map of frame numbers for seeking to a specific frame */
   std::map<unsigned long,size_t> _lSeekFrameNum;

   /** Speed multiplier for this video */
   double _fSpeed;

   /** Multiplication factor for mapping values to buffer, for alpha channel streams */
   float _fAlphaMultiplier;

   /** Offset for mapping values to buffer, for alpha channel streams */
   float _fAlphaOffset;

   /** Table for mapping alpha channel values to buffer, for alpha channel streams */
   unsigned int *_nAlphaTable;

   /** Timestamp of currently ready video buffer */
   double _fVideoBufTime[_KVIDEO_NBUFFERS];

   /** For each ready video buffer, index of the loop that the frame is part of */ 
   long _nVideoBufLoopIdx[_KVIDEO_NBUFFERS];

   /** For each ready video buffer, absolute frame number in the video file of each frame */
   unsigned long _nVideoBufFrame[_KVIDEO_NBUFFERS];

   /** For each ready video buffer, frame sequence number; used to keep color and alpha videos in sync when frames are dropped */
   long _nVideoBufSequence[_KVIDEO_NBUFFERS];

   /** Buffer for sending the video frames to Kanji */
   unsigned int *_lpVideoBuffer[_KVIDEO_NBUFFERS];

   /** Number of frames decoded since we started reading this video */
   long _nDecodedFrames;

   /** Number of frames dropped since we started reading this video */
   long _nDroppedFrames;

   /** List of indices of audio buffers that currently contain decoded data */
   std::list<long> _lReadyAudioBuf;

   /** List of indices of audio buffers that are currently free */
   std::list<long> _lFreeAudioBuf;

   /** Number of audio samples per buffer */
   long _nAudioSamplesPerBuf;

   /** Buffer for sending the audio data to Kanji */
   unsigned short *_lpAudioBuffer[_KVIDEO_NBUFFERS];

   /** For each audio buffer, total bytes in the buffer */
   size_t _nAudioBufSize[_KVIDEO_NBUFFERS];

   /** For each audio buffer, bytes left to sent to Kanji */
   size_t _nAudioBytesLeft[_KVIDEO_NBUFFERS];

   /** For each audio buffer, when ready, position of the first byte of data into the decompressed audio */
   KVideoDataPos _nAudioDataPos[_KVIDEO_NBUFFERS];

   /** For each ready audio buffer, index of the loop that the frame is part of */ 
   long _nAudioBufLoopIdx[_KVIDEO_NBUFFERS];

   /** When both a video and audio streams are present, difference between system clock and audio clock */
   double _fAudioTimeDelta;
   
   /** When both a video and audio streams are present, true if audio time delta is known yet */
   bool _bAudioSyncEstablished;

   /** Additional difference between system clock and returned clock, used for seeking */
   double _fSeekTimeDelta;

   /** Name of file containing video */
   char _szFileName[K_MAXPATH];

   /** List of all instanciated videos */
   static KList<KVideo> g_lVideos;

   /** true if shader is enabled if supported */
   static bool g_bEnablePixelShader;
   
#ifdef _KVIDEO_SUPPORT_SHADERS
   /** true if message has been logged about a supported shader yet */
   static bool g_bShaderMessageLogged;

   /** YUV to RGB pixel shader */
   static KPixelShader *g_lpYUVShader;
#endif
};

#endif   /* _KANJI_KVIDEO_H */
