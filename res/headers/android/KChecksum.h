/**
 * Kanji game engine - MD5 checksum definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KCHECKSUM_H
#define  _KANJI_KCHECKSUM_H

/* Include definitions */
#include "KObjectBase.h"

/**
 * MD5 checksum, that can be used for instance to sign saved game states 
 */

class KChecksum {
public:
   /** Constructor */
   KChecksum ();

   /** Destructor */
   ~KChecksum ();

   /** Reset checksum */
   void reset ();

   /**
    * Add the specified memory block to the checksum
    *
    * \param lpBuffer memory block
    * \param nBytes size of the memory block, in bytes
    */
   void update (const unsigned char *lpBuffer, size_t nBytes);

   /**
    * Finalize and output the checksum after all the memory blocks have been passed
    *
    * \param lpChecksum 16-byte array where the final MD5 digest will be returned
    */
   void finalize (unsigned char *lpChecksum);

private:
   /**
    * The core of the MD5 algorithm, this alters an existing MD5 hash to
    * reflect the addition of 16 longwords of new data.  MD5Update blocks
    * the data and converts bytes into longwords for this routine.
    *
    * \param lpBuffer current digest
    * \param lpInData 16 ints of data to be crunched into digest
    */
   static void transform (unsigned int *lpBuffer, unsigned int *lpInData);

   /**
    * Byte swap data, if needed
    *
    * \param lpBuffer buffer to be byte-swapped
    * \param nLongs number of ints in buffer
    */
   static void byteReverse (unsigned char *lpBuffer, unsigned int nLongs);

   /** Data (byte-swapped if necessary) currently being processed */
   unsigned char _in[64];

   /** Current digest */
   unsigned int _buf[4];

   /** Current bitcount */
   unsigned int _bits[2];
};

#endif   /* _KANJI_KCHECKSUM_H */
