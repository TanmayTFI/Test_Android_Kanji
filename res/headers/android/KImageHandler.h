/**
 * Kanji rendering and I/O engine - image format handler definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KIMAGEHANDLER_H
#define  _KANJI_KIMAGEHANDLER_H

/** \cond private */

/* Include definitions */
#include "KObjectListable.h"

/** Create an identifier for externally supported images format, from a three letter extension */
#define  K_IMAGE_FORMAT(_a,_b,_c)(0x01000000|(_a)<<16|(_b)<<8|(_c))

/* Forward declarations */
class KImage;

/**
 * Base class for image format handlers
 */

class KImageHandler : public KObjectListable {
public:
   /** Destructor */
   virtual ~KImageHandler ();

   /**
    * Get array of filename extensions corresponding to format supported by this handler
    *
    * \return array of extensions, NULL terminated
    */
   virtual const char **getExtensions (void) = 0;

   /**
    * Get ID of format supported by this handler
    *
    * \return format ID
    */
   virtual const unsigned long getFormatId (void) = 0;

   /**
    * Decode buffer into an image of the format that this handler supports
    *
    * \param lpBuffer buffer containing data to be decoded
    * \param nBufferSize size of buffer, in bytes
    * \param lpImage instance to store decoded image into
    *
    * \return true for success, false for failure
    */
   virtual bool decode (const unsigned char *lpBuffer, size_t nBufferSize, KImage *lpImage) = 0;

   /**
    * Encode this image into a buffer, in the specified format
    *
    * \param lpBuffer returned buffer, must be freed by the caller
    * \param nBufferSize returned size of buffer, in bytes
    * \param lpImage instance to encode image from
    * \param nQuality quality setting for lossy formats, 0..100
    *
    * \return true for success, false for failure
    */
   virtual bool encode (unsigned char *&lpBuffer, size_t &nBufferSize, KImage *lpImage, long nQuality) = 0;
};

/** \endcond */

#endif   /* _KANJI_KIMAGEHANDLER_H */
