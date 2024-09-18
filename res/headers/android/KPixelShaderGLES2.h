/**
 * Kanji rendering and I/O engine - Android OpenGL ES 2.0 GPU pixel shader, definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KPIXELSHADERGLES2_H
#define  _KANJI_KPIXELSHADERGLES2_H

/* Include definitions */
#include "KPixelShader.h"
#include "KWindowGLES2.h"
#include "KList.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/**
 * Base class for OpenGL ES 2.0 GPU pixel shader stubs
 */

class KPixelShaderGLES2 : public KPixelShader {
   friend class KWindowGLES2;
   
public:
   /** Constructor */
   KPixelShaderGLES2 ();

   /** Destructor */
   ~KPixelShaderGLES2 ();

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
   
private:
   /**
    * Create shader on the GPU
    *
    * \return true if successful, false if not
    */
   bool createGLES2Program (void);

   /**
    * Free shader on the GPU
    */
   void releaseGLES2Program (void);

   /**
    * Get GPU program that contains this pixel shader
    *
    * \return program
    */
   GLES2Program *getProgram (void) { return &_program; }
   
   /**
    * Initialize handling of GPU pixel shaders for OpenGL. Called by KWindowGLES2
    *
    * \param lpKWindow game window
    */
   static void setup (KWindowGLES2 *lpKWindow);
   
   /** Free all resources used by KPixelShaderGLES2 instances */
   static void cleanup (void);
   
   /* Reload all pixel shaders */
   static void reloadAll (void);

   /** GPU program */
   GLES2Program _program;
   
   /** Shader code */
   unsigned char *_lpShaderData;

   /** Shader code size, in bytes */
   size_t _nShaderDataSize;

   /** Game window */
   static KWindowGLES2 *g_lpKWindow;

   /** List of all KPixelShaderGLES2 instances */
   static KList<KPixelShaderGLES2> g_lGLES2PixelShaders;
};

#endif   /* _KANJI_KPIXELSHADERGLES2_H */
