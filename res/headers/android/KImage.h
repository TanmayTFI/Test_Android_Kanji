/**
 * Kanji rendering and I/O engine - image reader/writer definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KIMAGE_H
#define  _KANJI_KIMAGE_H

/* Include definitions */
#include "KObjectBase.h"
#include "KList.h"
#include "KWindow.h"

/* Forward declarations */
typedef struct KGraphicSpliceStruct KGraphicSplice;
class KImageHandler;
class KMiscTools;

/** Resampling method for resampleImage() */

typedef enum {
   K_IMAGE_RESAMPLE_BILINEAR = 0,         /**< Bilinear (fast, default) */
   K_IMAGE_RESAMPLE_BICUBIC,              /**< Bicubic (much slower, better quality for downscaling) */
} KImageResampleMethod;

/**
 * Class for loading and saving image files
 */

class KImage : public KObjectBase {
   friend class KMiscTools;

public:
   /** Constructor */
   KImage ();

   /** Destructor */
   ~KImage ();

   /** Free resources used by this image */
   void freeImage (void);

   /**
    * Detect image format from the supplied filename's extension (.tga, .jpg, ...)
    *
    * \param lpszFileName filename to detect image format from
    *
    * \return detected format, or K_IMAGE_UNKNOWN if unsupported
    */
   static long detectFormat (const char *lpszFileName);

   /**
    * Decode specified buffer into this image
    *
    * \param nFormat format of image to be decoded (K_IMAGE_xxx)
    * \param lpBuffer buffer containing data to be decoded
    * \param nBufferSize size of buffer, in bytes
    *
    * \return true for success, false for failure
    */
   bool decodeImage (long nFormat, const unsigned char *lpBuffer, size_t nBufferSize);

   /**
    * Encode this image into a buffer, in the specified format
    *
    * \param nFormat format of image to be encoded (K_IMAGE_xxx)
    * \param lpBuffer returned buffer, must be freed by the caller
    * \param nBufferSize returned size of buffer, in bytes
    * \param nQuality quality setting for lossy formats, 0..100
    *
    * \return true for success, false for failure
    */
   bool encodeImage (long nFormat, unsigned char *&lpBuffer, size_t &nBufferSize, long nQuality = 94);

   /**
    * Rescale this image to the specified width and height
    *
    * \param nNewWidth new width for this image, in pixels
    * \param nNewHeight new height for this image, in pixels
    * \param nMethod resampling method (K_IMAGE_RESAMPLE_xxx, defaults to K_IMAGE_RESAMPLE_BILINEAR)
    */
   void resampleImage (unsigned long nNewWidth, unsigned long nNewHeight, KImageResampleMethod nMethod = K_IMAGE_RESAMPLE_BILINEAR);

   /**
    * Get this image's width
    *
    * \return width in pixels
    */
   unsigned long getImageWidth (void) const;

   /**
    * Get this image's height
    *
    * \return height in pixels
    */
   unsigned long getImageHeight (void) const;

   /**
    * Check if this image was created from greyscale data
    *
    * \return true if image was created from greyscale data, false if it was created from color data
    */
   bool isGreyscale (void) const;

   /**
    * Check if image was created from 8 bits per color channel data, false it was created from less (for instance 15/16 bits per pixel images)
    *
    * \return true if created from 8 bits per color channel data, false if not
    */
   bool isHiQuality (void) const;

   /**
    * Check if this image contains an alpha channel
    *
    * \return true if image contains an alpha channel, false if it only contains color data
    */
   bool hasAlphaChannel (void) const;

   /**
    * Get this image's contents, as an array of ABGR pixels
    *
    * \return pixel data
    */
   unsigned int *getPixels (void) const;

   /**
    * Set this image's contents, as an array of ABGR pixels
    *
    * \param nWidth image width, in pixels
    * \param nHeight image height, in pixels
    * \param bAlphaChannelPresent true if alpha channel is present, false if not
    * \param lpPixels image contents, as an array of ABGR pixels; the buffer is not copied but will be freed when this image is destroyed
    * \param bGreyscale true if image was created from greyscale data
    * \param bHiQuality true if image was created from 8 bits per color channel data, false it was created from less (for instance 15/16 bits per pixel images)
    */
   void setPixels (unsigned long nWidth, unsigned long nHeight, bool bAlphaChannelPresent, unsigned int *lpPixels, bool bGreyscale = false, bool bHiQuality = true);

   /**
    * Unlock the paid version of j2k-codec by providing the license key. This method needs to be called before loading any images
    *
    * \param lpszKey license key
    */
   static void unlockJ2kCodec (const char *lpszKey);

   /**
    * Add external image format handler
    *
    * \param lpHandler image handler to add
    */
   static void addHandler (KImageHandler *lpHandler);

   /**
    * Remove external image format handler
    *
    * \param lpHandler image handler to remove
    */
   static void removeHandler (KImageHandler *lpHandler);

private:
   /** Initialize internal handlers */
   static void initialize (void);

   /** Free all resources used by image handling */
   static void cleanup (void);

   /** Image width, in pixels */
   unsigned long _nImageWidth;

   /** Image height, in pixels */
   unsigned long _nImageHeight;

   /** true if image was created from greyscale data */
   bool _bGreyscale;

   /** true if image was created from 8 bits per color channel data, false it was created from less (for instance 15/16 bits per pixel images) */
   bool _bHiQuality;

   /** true if image contains an alpha channel */
   bool _bAlphaChannelPresent;

   /** Array of image pixels */
   unsigned int *_lpPixels;

   /** true if the list of handlers has been initialized yet */
   static bool g_bHandlersInitialized;

   /** List of image handlers */
   static KList<KImageHandler> g_lHandlers;
};

#endif   /* _KANJI_KIMAGE_H */
