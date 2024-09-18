/**
 * Kanji rendering and I/O engine - base class for sound services - internal
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSOUNDBASE_H
#define  _KANJI_KSOUNDBASE_H

/** \cond private */

/* Include definitions */
#include "KObjectListable.h"
#include "KHashTableLong.h"

/**
 * Rewind music when playing a stream created with initStream()
 *
 * \param lpUserData data passed to initStream()
 */
typedef void (*kanjiRewindStreamCallback)(void *lpUserData);

/**
 * Get a block of music data when playing a stream created with initStream()
 *
 * \param lpUserData data passed to initStream()
 * \param lpBuffer where to store the data
 * \param nMaxSize maximum number of bytes of data to get
 *
 * \return number of bytes actually obtained, 0 if stream ends
 */
typedef size_t (*kanjiGetStreamPacketCallback)(void *lpUserData, char *lpBuffer, size_t nMaxSize);

/**
 * Growable buffer - used for concatenating unpacked samples into a single buffer, in all sound format loaders
 * Replaces the very slow std::vector
 */

class KSoundGrowableArray {
public:
   /** Constructor */
   KSoundGrowableArray () :
      _lpData (NULL),
      _nDataSize (0),
      _nReservedSize (0) {
   }

   /** Destructor */
   ~KSoundGrowableArray () {
      if (_lpData) {
         free (_lpData);
         _lpData = NULL;
      }
   }

   /**
    * Get current size of the buffer
    *
    * \return size in bytes
    */
   const size_t size() {
      return _nDataSize;
   }

   /**
    * Get current contents of the buffer
    *
    * \return buffer contents
    */
   const void *data () {
      return (const void *) _lpData;
   }

   /**
    * Reserve space in the buffer
    */   
   void reserve (size_t nSize) {
      if (nSize != 0) {
         char *lpCombinedData = (char *) realloc (_lpData, _nDataSize + nSize);

         if (lpCombinedData) {
            _lpData = lpCombinedData;
            _nReservedSize += nSize;
         }
      }
   }

   /**
    * Append data into the buffer
    *
    * \param lpNewData start of data to append to this buffer
    * \param lpNewDataEnd end of data to append to this buffer, + 1
    */
   void appendData (const void *lpNewData, const void *lpNewDataEnd) {
      if (lpNewDataEnd > lpNewData) {
         size_t nNewDataSize = ((const char *) lpNewDataEnd) - ((const char *) lpNewData);

         if (_nReservedSize >= nNewDataSize) {
            memcpy (_lpData + _nDataSize, lpNewData, nNewDataSize);
            _nReservedSize -= nNewDataSize;
            _nDataSize += nNewDataSize;
         }
         else {
            size_t nNewAllocSize = (_nDataSize + nNewDataSize) * 2; /* Allocate memory for data and reserve some additional memory */
            char *lpCombinedData = (char *) realloc (_lpData, nNewAllocSize);

            if (lpCombinedData) {
               memcpy (lpCombinedData + _nDataSize, lpNewData, nNewDataSize);
               _lpData = lpCombinedData;
               _nDataSize += nNewDataSize;
               _nReservedSize = nNewAllocSize - _nDataSize; /* Calculate size of reserved memory */
            }
         }
      }
   }

private:
   /** Buffer contents */
   char *_lpData;

   /** Buffer size */
   size_t _nDataSize;

   /** Preallocated size */
   size_t _nReservedSize;
};

/**
 * Base class for playing music and sound effects
 */

class KSoundBase : public KObjectHashableLong {
public:
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
   virtual bool loadSample (const char *lpszFileName, int nVolume = 100, bool bLoop = false, short nMaxChannels = 1) = 0;

   /**
    * Start playing this music
    *
    * \param bLoopStream true to loop music indefinitely, false to play once and then stop
    */
   virtual void playStream (bool bLoopStream = true) = 0;

   /**
    * Stop playing this music
    */
   virtual void stopStream (void) = 0;

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
                            kanjiRewindStreamCallback lpRewindStream, kanjiGetStreamPacketCallback lpGetStreamPacket, void *lpUserData, long nBufferSize = 32768) = 0;

   /**
    * Pause or resume playing this music or sound effect
    *
    * \param bPause true to pause, false to resume
    */
   virtual void pause (bool bPause) = 0;
};

/** \endcond */

#endif   /* _KANJI_KSOUNDBASE_H */
