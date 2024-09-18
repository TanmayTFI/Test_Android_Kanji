/**
 * Kanji game engine - managed pixel shader definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDSHADER_H
#define  _KANJI_KMANAGEDSHADER_H

/* Include definitions */
#include "KPTK.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedShaderList;
class KPixelShader;

/** Managed pixel shader, with reference counting */

class KManagedShader : public KObjectHashable {
   friend class KManagedShaderList;

private:
   /** Filename */
   char szFilePath[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** KPixelShader holding the pixel shader */
   KPixelShader *lpShader;
};

/** List of managed pixel shaders */

class KManagedShaderList : public KList<KManagedShader> {
public:
   /** Constructor */
   KManagedShaderList ();

   /** Destructor */
   ~KManagedShaderList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to relative filenames, so that the filename can be used as a key to retrieve a shader
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Load pixel shader from a file or add a reference to it
    *
    * \param lpszFilename name of file to load pixel shader from. the name of the game window's renderer is prefixed to the last part of the filename so as to
    *                     load the shader that corresponds to the renderer (for instance, shader.bin becomes d3d9_shader.bin for K_DIRECTX9 and gl_shader.bin for K_OPENGL)
    *
    * \return pixel shader, or NULL if loading failed
    */
   KPixelShader *loadShader (const char *lpszFilename);

   /**
    * Get pixel shader from its filename
    *
    * \param lpszFilename filename of pixel shader to get
    *
    * \return pixel shader, NULL if not currently loaded
    */
   KPixelShader *getShaderByName (const char *lpszFilename);

   /**
    * Dereference or unload pixel shader by reference
    *
    * \param lpShader pixel shader to be dereferenced or unloaded
    */
   void unloadShaderByRef (KPixelShader *lpShader);

   /**
    * Dereference or unload pixel shader by filename
    *
    * \param lpszFilename filename of pixel shader to be dereferenced or unloaded
    */
   void unloadShaderByName (const char *lpszFilename);

   /**
    * Enumerate currently loaded pixel shaders in this list
    *
    * \param enumCallback method called back for each pixel shader with user data, the filename, reference count and KPixelShader
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KPixelShader *lpShader), void *lpUserData);

   /** Free all loaded pixel shaders */
   void freeAll (void);

private:
   /**
    * Compose shader name in path buffer, from filename
    *
    * \param lpszFilename name of file containing pixel shader
    */
   void composeShaderName (const char *lpszFilename);

   /** Thread safety lock for accessing this list of pixel shaders */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Hashtable of all managed pixel shaders in this list */
   KHashTable _hash;
};

#endif   /* _KANJI_KMANAGEDSHADER_H */
