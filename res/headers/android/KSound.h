/**
 * Kanji audio engine - class for music and sound effects playback using Android, definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSOUND_H
#define  _KANJI_KSOUND_H

/* Include definitions */
#include "KObjectListable.h"
#include "KList.h"
#include "KSysThread.h"
#include "KSysLock.h"
#include "KSoundBase.h"
#include "AndroidIo.h"

/** Sound effect format */
enum {
   SAMPLE_UNKNOWN = -1, /**< Unknown format */
   SAMPLE_WAV = 0,      /**< Sound effect in wav format */
   SAMPLE_OGG = 1,      /**< Sound effect in ogg vorbis format */
   SAMPLE_CAF = 2,      /**< Sound effect in caf format */
};

/** Maximum number of channels per sound */
#define  _KSOUND_MAXCHANNELS  16

/** Size of an uncompressed sound effect above which the sound is streamed rather than static.
 * Android's AudioFlinger has an mixing buffer size limit. */
#define  _KSOUND_ANDROID_STREAM_THRESHOLD 120000

/** Buffer size for loadStream, and for playing large sound effects */
#define  _KSOUND_STREAM_BUFFER_SIZE (32768)

/* Forward declarations */
typedef struct OggVorbis_File OggVorbis_File;
typedef long long ogg_int64_t;

/**
 * Class for playing music and sound effects using DirectSound
 */

class KSound : public KSoundBase {
public:
   /** Constructor */
   KSound ();

   /** Destructor */
   ~KSound ();

   /**
    * Detect sound effect format from the supplied filename's extension (.wav, .ogg)
    *
    * \param lpszFileName filename to detect image format from
    *
    * \return detected format (SAMPLE_xxx), or SAMPLE_UNKNOWN if unsupported
    */
   static long detectFormat (const char *lpszFileName);

   /**
    * Load ogg vorbis music from a file into this instance. The music remains compressed and the compressed music is streamed from memory upon playback
    *
    * \param lpszFileName name of file to load music from
    *
    * \return true for success, false for failure
    */
   bool loadStream (const char *lpszFileName);

   /**
    * Copy ogg vorbis music from a memory buffer into this instance. The music remains compressed and the compressed music is streamed from memory upon playback
    *
    * \param lpBuffer memory buffer to copy music from
    * \param nBufferSize size of memory buffer containing encoded music, in bytes
    *
    * \return true for success, false for failure
    */
   bool loadStreamFromPtr (const char *lpBuffer, unsigned long nBufferSize);
   
   /**
    * Load sound effect from a file into this instance. The sound effect is fully decompressed in memory
    *
    * \param lpszFileName name of file to load sound effect from
    * \param nVolume initial volume (0..100)
    * \param bLoop true to loop sound indefinity when playing, false to only play once and then stop
    * \param nMaxChannels number of times that this sound can be played in parallel on different audio channels; if more playSample() calls are issued than
    *                     this number, one of the samples will be stopped and re-started
    *
    * \return true for success, false for failure
    */
   bool loadSample (const char *lpszFileName, int nVolume = 100, bool bLoop = false, short nMaxChannels = 1);

   /**
    * Decode sound effect from a memory buffer into this instance. The sound effect is fully decompressed in memory
    *
    * \param lpBuffer memory buffer to decode sound effect from
    * \param nBufferSize size of memory buffer containing encoded sound effect, in bytes
    * \param nFormat format of the encoded sound effect (SAMPLE_xxx)
    * \param nVolume initial volume (0..100)
    * \param bLoop true to loop sound indefinity when playing, false to only play once and then stop
    * \param nMaxChannels number of times that this sound can be played in parallel on different audio channels; if more playSample() calls are issued than
    *                     this number, one of the samples will be stopped and re-started
    *
    * \return true for success, false for failure
    */
   bool loadSampleFromPtr (const char *lpBuffer, unsigned long nBufferSize, long nFormat, int nVolume = 100, bool bLoop = false, short nMaxChannels = 1);

   /** Free all resources used by this music or sound effect */
   void freeSound (void);

   /**
    * Set whether playing that sounds allows for collecting low-priority sounds if resources run out
    *
    * \param bHighPriority true for high priority sound, false for low priority
    */
   void setHighPriority (bool bHighPriority);

   /**
    * Set whether this sound is always played statically, even if it is large
    *
    * \param bForceStatic true to use static sound, false to stream if it is large (default)
    */
   void setForceStatic (bool bForceStatic);

   /**
    * Set whether to resume tbis sound after suspending app, even if doesn't loop
    *
    * \param bAlwaysResume true to always resume, false not to resume non-looping sound (default)
    */
   void setAlwaysResume (bool bAlwaysResume);
   
   /**
    * Set new volume for music or sound effect
    *
    * \param nVolume new volume, 0..100 for silent to full volume
    */
   void setVolume (long nVolume);

   /**
    * Set new pitch for this sound effect
    *
    * \param fPitch pitch (1.0f for normal playback)
    */
   void setPitch (float fPitch);
   
   /**
    * Start playing this sound effect
    *
    * \param bSetLoopMode true to use a different looping mode than what was passed to loadSample(), false to use the same looping mode (default)
    * \param bLoopSample when bSetLoopMode is true: true to loop the sample indefinitely, false to play once and then stop. ignored if bSetLoopMode is false
    */
   void playSample (bool bSetLoopMode = false, bool bLoopSample = false);

   /**
    * Stop playing this sound effect
    */
   void stopSample (void);

   /**
    * Start playing this music
    *
    * \param bLoopStream true to loop music indefinitely, false to play once and then stop
    */
   virtual void playStream (bool bLoopStream = true);

   /**
    * Stop playing this music
    */
   virtual void stopStream (void);
   
   /**
    * Pause or resume playing this music or sound effect
    *
    * \param bPause true to pause, false to resume
    */
   virtual void pause (bool bPause);

   /**
    * Check if this music or sound effect is currently playing
    *
    * \return true if playing, false if not
    */
   bool isPlaying (void);

   /**
    * Initialize streaming music for this instance using an external data source. The music can subsequently be played by playStream()
    *
    * \param nChannels number of audio channels (1-2)
    * \param nBitsPerSample number of bits per sample (8, 16)
    * \param nSamplesPerSec number of samples per second (for instance 44100)
    * \param lpRewindStream method called to rewind the music stream to the beginning, when starting the stream and when looping, if looping is requested
    * \param lpGetStreamPacket method called to get an audio packet for this music stream
    * \param lpUserData data passed back to the rewind and get methods
    * \param nBufferSize size of audio buffer to use for streaming, in bytes
    *
    * \return true for success, false for failure
    */
   virtual bool initStream (long nChannels, long nBitsPerSample, long nSamplesPerSec,
                            kanjiRewindStreamCallback lpRewindStream, kanjiGetStreamPacketCallback lpGetStreamPacket, void *lpUserData, long nBufferSize = 32768);

   /**
    * Get current time since the beginning of playback for this music, in milliseconds
    *
    * \return time in millis
    */
   double getStreamTime (void);

   /**
    * If this music loops indefinitely, check if it has restarted at the beginning since the last time this
    * method was called
    *
    * \return true if looped, false if not
    */
   bool hasStreamLooped (void);

   /**
    * Set global mixing volume for music and sound effects
    *
    * \param nGlobalStreamVolume global volume for all music, 0..100, multiplied with the individual music volumes
    * \param nGlobalSampleVolume global volume for all sound effects, 0..100, multiplied with the individual sound effect volumes
    */
   static void setGlobalVolumes (int nGlobalStreamVolume, int nGlobalSampleVolume);

   /** Suspend all sound activity, for instance after losing focus */
   static void suspend (void);

   /** Resume sound activity, for instance after regaining focus */
   static void resume (void);

   /**
    * Get the amount of memory currently used for sounds
    *
    * \return total in bytes
    */
   static size_t getTotalSoundBytes (void);

   /**
    * Free all lingering resources when not using audio anymore. This method may be called by the game just before exiting,
    * to check for memory leaks, otherwise it is not necessary.
    */
   static void cleanup (void);
   
   /**
    * Check if a sound output is actually available
    *
    * \return true if sound output is available, false if not
    */
   static bool isSoundDeviceAvailable (void);
   
   /**
    * Collect sounds that have stopped playing - internal
    *
    * \param bForce true to collect sounds even if they are playing
    *
    * \return true if at least one sound was collected, false if not
    */
   static bool collectSounds (bool bForce);

   /**
    * Handle periodic notification for streamed sounds - internal
    *
    * \param nKey stream key in the hashtable
    */    
   static void handlePeriodicNotification (long nKey);
   
private:
   /**
    * Set vorbis music up from a memory buffer into this instance. The music remains compressed and the compressed music is streamed from memory upon playback. This is called
    * by loadStream() and loadStreamFromPtr() after copying the music data
    *
    * \param lpBuffer memory buffer to copy music from
    * \param nBufferSize size of memory buffer containing encoded music, in bytes
    *
    * \return true for success, false for failure
    */
   bool loadStreamFromPtrInternal (const char *lpBuffer, unsigned long nBufferSize);
   
   /**
    * Parse a WAV sound file header
    *
    * \param lpBuffer memory buffer to decode sound file from
    * \param nBufferSize size of memory buffer containing encoded sound file, in bytes
    *
    * \return true if successful, false if not
    */
   bool openWav (const char *lpBuffer, unsigned long nBufferSize);

   /**
    * Read and decompress sound sample from WAV sound file
    *
    * \param lpDstBuffer buffer to read sound sample into, updated
    * \param nDstSize current buffer size, updated
    *
    * \return true if successful, false if not
    */
   bool readWav (void *&lpDstBuffer, size_t &nDstSize);

   /**
    * Close a WAV file after decoding it
    */
   void closeWav (void);

   /**
    * Parse an Ogg Vorbis sound file header
    *
    * \param lpBuffer memory buffer to decode sound file from
    * \param nBufferSize size of memory buffer containing encoded sound file, in bytes
    *
    * \return true if successful, false if not
    */
   bool openOgg (const char *lpBuffer, unsigned long nBufferLen);

   /**
    * Read and decompress sound sample from Ogg Vorbis sound file
    *
    * \param lpDstBuffer buffer to read sound sample into, updated
    * \param nDstSize current buffer size, updated
    *
    * \return true if successful, false if not
    */
   bool readOgg (void *&lpDstBuffer, size_t &nDstSize);

   /**
    * Close an ogg file after decoding it
    */
   void closeOgg (void);

   /**
    * Stop playing this sound effect. g_lpStreamLock must be acquired before calling this
    */
   void stopSampleInternal (void);
   
   /**
    * Pause or resume playing this music or sound effect. g_lpStreamLock must be acquired before calling this
    * method
    *
    * \param bPause true to pause, false to resume
    */
   void pauseInternal (bool bPause);
   
   /**
    * Read and decompress the next packet in a music stream and upload it into a sound buffer
    *
    * \param nBufferIdx index of buffer to upload to
    *
    * \return true for success, false for failure
    */
   bool streamPacket (long nBufferIdx);
   
   /**
    * Decompress and play a new buffer worth of data for this music stream
    */
   void updateStream (void);
   
   /**
    * Copy part of an oversized sound effect into the streaming buffer
    *
    * \param lpBuffer where to copy part of the sample at
    * \param nMaxSize number of bytes to copy
    *
    * \return number of bytes actually copied
    */
   size_t copyLongSample (char *lpBuffer, size_t nMaxSize);
   
   /**
    * Read data from buffer and provide it to Ogg Vorbis
    *
    * \param lpBuffer buffer to read data into
    * \param nMemberSize size of one data member to be read, in bytes
    * \param nMembers number of data members to be read
    * \param lpDataSource user pointer (instance of KSound reading the Vorbis file)
    *
    * \return number of members effectively read
    */
   static size_t vorbisUserRead (void *lpBuffer, size_t nMemberSize, size_t nMembers, void *lpDataSource);

   /**
    * Seek to specified position inside data buffer for Ogg Vorbis
    *
    * \param lpDataSource user pointer (instance of KSound reading the Vorbis file)
    * \param nOffset offset to seek to
    * \param nWhence how to interpret the offset (SEEK_SET, SEEK_CUR or SEEK_SET)
    *
    * \return 0 for success, EOF for failure
    */
   static int vorbisUserSeek (void *lpDataSource, ogg_int64_t nOffset, int nWhence);

   /**
    * Close buffer read for Ogg Vorbis
    *
    * \param lpDataSource user pointer (instance of KSound reading the Vorbis file)
    *
    * \return 0 for success, EOF for failure
    */
   static int vorbisUserClose (void *lpDataSource);

   /**
    * Get current position in buffer for Ogg Vorbis
    *
    * \param lpDataSource user pointer (instance of KSound reading the Vorbis file)
    *
    * \return current position
    */
   static long vorbisUserTell (void *lpDataSource);

   /** Android sound objects */
   AndroidSound *_lpSound[_KSOUND_MAXCHANNELS];

   /** For each android sound object, true if it needs to be collected as soon as it's done playing */
   bool _bCollectSound[_KSOUND_MAXCHANNELS];

   /** Currently opened OGG file, if any */
   OggVorbis_File *_lpOggFile;

   /** true if high priority sound, false if not */
   bool _bIsHighPriority;
   
   /** true if force static sound, false if not (can be streamed) */
   bool _bIsForceStatic;
   
   /** true to resume sound after suspending app, even if doesn't loop */
   bool _bAlwaysResume;
   
   /** true if non-streamed sound was set to loop by loadSample(), false if not */
   bool _bSoundLoopsDefault;
   
   /** true if non-streamed sound loops, false if it doesn't */
   bool _bSoundLoops;
   
   /** Sound volume, 0..100 */
   long _nVolume;

   /** Sound pitch, 1.0f for normal playback */
   float _fPitch;
   
   /** true if pitch has been changed from any value other than 1.0f for this sound */
   bool _bPitchChanged;
   
   /** Compressed sound file currently being read, NULL for none */
   void *_lpSoundFileData;

   /** Size of compressed sound file currently being read */
   unsigned long _nSoundFileSize;

   /** Current offset into sound file, when streaming music in the Ogg Vorbis format */
   long _nSoundFileOffs;

   /** Number of channels for this sound */
   long _nChannels;

   /** Number of bits per sample for this sound */
   long _nBitsPerSample;

   /** Frequency of currently loaded sound file, if any */
   long _nSoundFreq;

   /** Maximum number of simultaneous sources for this sound */
   long _nSourcesCnt;

   /** Index of buffer that will play next, for sounds with multiple buffers */
   long _nCurBufferIdx;

   /** Uncompressed audio data */
   void *_lpUncompressedData;

   /** Uncompressed audio size */
   size_t _nUncompressedSize;

   /** true if this sound effect is currently paused */
   bool _bIsSoundPaused;
   
   /** true if suspend() paused this sound effect while it was playing */
   bool _bPausedBySuspend;
   
   /** true to loop streamed sound, false not to */
   bool _bStreamLoops;
   
   /** For streamed sounds that don't loop, number of empty buffers played to let the sound finished */
   long _nLastStreamBuffersPlayed;
   
   /** true if Ogg stream is currently open and its contents haven't been freed yet */
   bool _bIsOggStreamOpen;
   
   /** true if currently streaming an oversized sound effect */
   bool _bIsLongSampleStream;
   
   /** true if stream is currently playing */
   bool _bIsStreamPlaying;
   
   /** Size of decoding buffers, in bytes, for a stream */
   size_t _nStreamBufferSize;
   
   /** true if this streaming music has looped */
   bool _bStreamLooped;
   
   /** Current playback time for this stream */
   double _fStreamTime;
   
   /** Timestamp for when this sound started initially playing */
   long _nPlayStartTime;
   
   /** PCM buffer used to decode data while streaming */
   char *_lpPcmBuffer;
   
   /** For external audio sources, method called for rewinding this stream */
   kanjiRewindStreamCallback _lpRewindStream;
   
   /** For external audio sources, method called for getting a new packet */
   kanjiGetStreamPacketCallback _lpGetStreamPacket;
   
   /** For external audio sources, user data passed back to the rewind and get callbacks */
   void *_lpStreamUserData;
   
   /** true if sound is initialized */
   static bool g_bSoundInitialized;
   
   /** true if sound output is suspended */
   static bool g_bSoundSuspended;

   /** List of all instantiated sounds */
   static KList<KSound> g_lSounds;
   
   /** Lock protecting access to the streamed sounds hashtable */
   static KSysLock *g_lpStreamLock;
   
   /** Next key to use for hashing a stream */
   static long g_nNextStreamKey;
   
   /** Hashtable of streamed sounds */
   static KHashTableLong *g_lpStreamsHash;

   /** Global volume for sound effects, multiplied with the individual sound volumes; 0..100 */
   static long g_nGlobalSampleVolume;
};

#endif   /* _KANJI_KSOUND_H */
