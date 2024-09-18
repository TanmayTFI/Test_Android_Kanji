/**
 * Kanji game engine - managed 3d model definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDMODEL_H
#define  _KANJI_KMANAGEDMODEL_H

/* Include definitions */
#include "KPTK.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedModelList;
class KModel;

/** Managed 3d model, with reference counting */

class KManagedModel : public KObjectHashable {
   friend class KManagedModelList;

private:
   /** Path to be prefixed to model filename */
   char szActualFileName[K_MAXPATH];

   /** Model filename */
   char szFilePath[K_MAXPATH];

   /** Path to be prefixed to all texture filenames */
   char szTextureFileNamePrefix[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** true if textures have been uploaded to VRAM yet */
   bool bUploaded;

   /** KModel holding the 3d model */
   KModel *lpModel;
};

/** List of managed 3d models */

class KManagedModelList : public KList<KManagedModel> {
public:
   /** Constructor */
   KManagedModelList ();

   /** Destructor */
   ~KManagedModelList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to models' relative filenames, so that the filename can be used as a key to retrieve a model
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Load 3d model from a file or add a reference to it. This method may be called from any thread
    *
    * \param lpszFilename name of file to load 3d model from
    * \param lpszTextureFileNamePrefix path to be prefixed to all texture filenames
    *
    * \return 3d model, or NULL if loading failed
    */
   KModel *loadModel (const char *lpszFilename, const char *lpszTextureFileNamePrefix);

   /**
    * Upload this model and its textures to VRAM. This method may only be called from the main thread
    *
    * \param lpModel model to be uploaded
    */
   void uploadModel (KModel *lpModel);

   /**
    * Get 3d model from its filename
    *
    * \param lpszFilename filename of 3d model to get
    *
    * \return 3d model, NULL if not currently loaded
    */
   KModel *getModelByName (const char *lpszFilename);

   /**
    * Dereference or unload 3d model by reference
    *
    * \param lpModel 3d model to be dereferenced or unloaded
    */
   void unloadModelByRef (KModel *lpModel);

   /**
    * Dereference or unload 3d model by filename
    *
    * \param lpszFilename filename of 3d model to be dereferenced or unloaded
    */
   void unloadModelByName (const char *lpszFilename);

   /**
    * Enumerate currently loaded 3d models in this list
    *
    * \param enumCallback method called back for each 3d model with user data, the filename, reference count and KModel
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KModel *lpModel), void *lpUserData);

   /** Free all loaded 3d models */
   void freeAll (void);

private:
   /** Thread safety lock for accessing this list of 3d models */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Hashtable of all managed 3d models in this list */
   KHashTable _hash;
};

#endif   /* _KANJI_KMANAGEDMODEL_H */
