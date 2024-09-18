/**
 * Kanji rendering and I/O engine - Android base class for GPU pixel shaders, definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KPIXELSHADER_H
#define  _KANJI_KPIXELSHADER_H

/* Include definitions */
#include "KObjectListable.h"
#include "KGraphic.h"

/**
 * Base class for GPU pixel shaders
 */

class KPixelShader : public KObjectListable {
public:
   /** Destructor */
   virtual ~KPixelShader () { ; };

   /**
    * Load compiled pixel shader. The pixel shader's data must correspond to the active game window renderer (D3D9 or OpenGL)
    *
    * \param lpszFileName filename to load shader from
    *
    * \return true for success, false for failure
    */
   virtual bool loadPixelShader (const char *lpszFileName) = 0;

   /**
    * Load compiled pixel shader from memory buffer. The pixel shader's data must correspond to the active game window renderer (D3D9 or OpenGL)
    *
    * \param lpBuffer shader data
    * \param nBufferSize shader data size
    *
    * \return true for success, false for failure
    */
   virtual bool loadPixelShaderFromPtr (const unsigned char *lpBuffer, size_t nBufferSize) = 0;

   /** Free all resources used by this shader */
   virtual void freePixelShader (void) = 0;
};

#endif   /* _KANJI_KPIXELSHADER_H */
