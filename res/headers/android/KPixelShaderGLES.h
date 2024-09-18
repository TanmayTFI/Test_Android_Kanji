/**
 * Kanji rendering and I/O engine - Android ES 1.x GPU pixel shader stubs, definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KPIXELSHADERGLES_H
#define  _KANJI_KPIXELSHADERGLES_H

/* Include definitions */
#include "KPixelShader.h"
#include "KWindowGLES.h"
#include "KList.h"

/**
 * Base class for OpenGL ES 1.x GPU pixel shader stubs
 */

class KPixelShaderGLES : public KPixelShader {
public:
   /** Constructor */
   KPixelShaderGLES ();

   /** Destructor */
   ~KPixelShaderGLES ();

   /**
    * Load compiled pixel shader. The pixel shader's data must correspond to the active game window renderer (D3D9 or OpenGL)
    *
    * \param lpszFileName filename to load shader from
    *
    * \return true for success, false for failure
    */
   virtual bool loadPixelShader (const char *lpszFileName);

   /**
    * Load compiled pixel shader from memory buffer. The pixel shader's data must correspond to the active game window renderer (D3D9 or OpenGL)
    *
    * \param lpBuffer shader data
    * \param nBufferSize shader data size
    *
    * \return true for success, false for failure
    */
   virtual bool loadPixelShaderFromPtr (const unsigned char *lpBuffer, size_t nBufferSize);

   /** Free all resources used by this shader */
   virtual void freePixelShader (void);
};

#endif   /* _KANJI_KPIXELSHADERGLES_H */
