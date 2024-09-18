/**
 * Kanji game engine - managed graphic class definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMANAGEDGRAPHIC_H
#define  _KANJI_KMANAGEDGRAPHIC_H

/* Include definitions */
#include "KPTK.h"
#include "KSysLock.h"
#include "KHashTable.h"
#include "KList.h"

/* Forward definitions */
class KManagedGraphicList;
class KManagedGraphic;
class KUIImage;

/** Node of a graphic atlas being created */

class KManagedGraphicAtlasNode {
public:
   /** Constructor */
   inline KManagedGraphicAtlasNode() {
      lpChild[0] = lpChild[1] = NULL;
      rc = KRectangle (0, 0, 0, 0);
      lpEntry = NULL;
   }

   /** Children */
   KManagedGraphicAtlasNode *lpChild[2];

   /** Rectangle of graphic atlas covered by this node */
   KRectangle rc;

   /** For leaf nodes, graphic being atlased, that is assigned to this node */
   KManagedGraphic *lpEntry;
};

/** Managed graphic, with reference counting and alpha splitting support */

class KManagedGraphic : public KObjectHashable {
   friend class KManagedGraphicList;

private:
   /** Filename */
   char szFilePath[K_MAXPATH];

   /** Current reference count */
   long nRefCount;

   /** true if loaded as truecolor, false if loaded as 15 or 16bpp */
   bool bHiQuality;

   /** true if the alpha mask from the graphic was loaded if present, false if not */
   bool bUseAlpha;

   /** true if this graphic is spliced into multiple GPU textures that fit the maximum hardware texture size, false if loaded as a single texture */
   bool bDoSplice;

   /** true if this graphic was loaded as seperate color and alpha images */
   bool bAlphaSplit;

   /** true if graphic has been uploaded to VRAM yet */
   bool bUploaded;

   /** KGraphic holding the graphics */
   KGraphic *lpGraphic;

   /** Atlas for this graphic, NULL if none */
   KManagedGraphic *lpAtlas;

   /** If the graphic is part of an atlas with tree-based placement, placed coordinates of this managed graphic in its atlas */
   KRectangle rcDstInAtlas;

   /** For the hit mask, alpha value above or equal to which pixels will be considered opaque, and under which they will be considered transparent */
   long nAlphaMaskThreshold;

   /** Hit mask for this graphic, NULL if none */
   unsigned char *lpHitMask;

   /** For atlases, if tree-based placement of managed graphics is used, root node of tree */
   KManagedGraphicAtlasNode *lpRootAtlasNode;
};

/** List of managed graphics */

class KManagedGraphicList : public KList<KManagedGraphic> {
public:
   /** Constructor */
   KManagedGraphicList ();

   /** Destructor */
   ~KManagedGraphicList ();

   /**
    * Set path prepended to relative filenames. The prefix can only be set once
    *
    * \param lpszPrefix path prepended to relative filenames, so that the filename can be used as a key to retrieve a graphic
    */
   void setPrefix (const char *lpszPrefix);

   /**
    * Begin loading managed graphics into atlases. If a previous atlassing request had already begun and not ended, it will be ended first
    *
    * \param nMaxAtlasWidth maximum width, in pixels, of an individual atlas
    * \param nMaxAtlasHeight maximum height, in pixels, of an individual atlas
    * \param nMaxGraphicWidth maximum width, in pixels, of a managed graphic to fit into atlases; above this width, the graphic is put in its own texture
    * \param nMaxGraphicHeight maximum height, in pxiels, of a managed graphic to fit into atlases; above this height, the graphic is put in its own texture
    */
   void beginAtlas (long nMaxAtlasWidth, long nMaxAtlasHeight, long nMaxGraphicWidth, long nMaxGraphicHeight);

   /**
    * End loading managed graphics into atlases; the atlases will be finalized and the graphics will be usable for drawing.
    * This method may only be called from the main thread
    */
   void endAtlas (void);

   /**
    * Load graphic from file or add a reference to it. This method may be called from any thread
    *
    * \param lpszFilename filename to load graphic from
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bUseAlpha true to load the alpha mask from the graphic if present, false not to
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage, false to load as single texture
    * \param nAlphaMaskThreshold when a mask is retained for hit testing purposes, value above or equal to which pixels will be considered opaque, and under which they will be considered transparent (-1 for no hit mask, default)
    * \param bForceReload true to force the graphic to be reloaded from disk as if it was the first reference, even if it was already loaded
    * \param lpszAlphaExt extension to use when loading the image failed and the method tries to load as a seperate .jpg for color, and an image with the specified extension for alpha (defaults to '_a.png')
    * \param bEnableNPOT true to enable loading graphics using non-power of two sized textures, if supported by the hardware, false to use power-of-two dimensions
    * \param lpszColorExt extension to use when loading the image failed and the method tries to load as a seperate .jpg for color, and an image with the specified extension for alpha (defaults to '.jpg')
    * \param bGenerateMipMaps true to generate mipmaps for the loaded graphics, false not to
    *
    * \return graphic, NULL if loading failed. When successful, uploadGraphic() must be called before graphic can be blitted
    */
   KGraphic *loadGraphic (const char *lpszFilename, bool bHiQuality = true, bool bUseAlpha = true, bool bDoSplice = false, long nAlphaMaskThreshold = -1, bool bForceReload = false, const char *lpszAlphaExt = NULL,
                          bool bEnableNPOT = false, const char *lpszColorExt = NULL, bool bGenerateMipMaps = false);

   /**
    * Upload this graphic to VRAM. This method may only be called from the main thread
    *
    * \param lpGraphic graphic to be uploaded
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures, if it wasn't already enabled by loadGraphic(), now
    *                  that the graphic size is known; defaults to what was set by loadGraphic()
    */
   void uploadGraphic (KGraphic *lpGraphic, bool bDoSplice = false);

   /**
    * Get graphic from its filename
    *
    * \param lpszFilename filename of graphic to get
    *
    * \return graphic, NULL if not currently loaded
    */
   KGraphic *getGraphicByName (const char *lpszFilename);

   /**
    * Get hit mask by a graphic's filename
    *
    * \param lpszFilename filename of graphic to get hit mask for
    *
    * \return hit mask, NULL if none
    */
   const unsigned char *getHitMaskByName (const char *lpszFilename);

   /**
    * Get hit mask by a graphic's reference
    *
    * \param lpGraphic graphic to get hit mask for
    *
    * \return hit mask, NULL if none
    */
   const unsigned char *getHitMaskByRef (KGraphic *lpGraphic);

   /**
    * Get hit mask's alpha threshold value by a graphic's filename
    *
    * \param lpszFilename filename of graphic to get alpha threshold for
    *
    * \return hit mask, NULL if none
    */
   const long getAlphaThresholdByName (const char *lpszFilename);

   /**
    * Get hit mask's alpha threshold valu by a graphic's reference
    *
    * \param lpGraphic graphic to get alpha threshold for
    *
    * \return hit mask, NULL if none
    */
   const long getAlphaThresholdByRef (KGraphic *lpGraphic);

   /**
    * Set stencil and hit mask in a KUIImage by a graphic's filename
    *
    * \param lpszFilename filename of graphic to place stencil/hit mask in KUIImage for
    * \param lpUIImage target KUIImage
    * \param x1 source X coordinate of top,left of portion of graphic to use as mask
    * \param y1 source Y coordinate of top,left of portion of graphic to use as mask
    * \param x2 source X coordinate of bottom,right of portion of graphic to use as mask
    * \param y2 source Y coordinate of bottom,right of portion of graphic to use as mask
    */
   void setStencilMaskByName (const char *lpszFilename, KUIImage *lpUIImage, float x1, float y1, float x2, float y2);

   /**
    * Set stencil and hit mask in a KUIImage by a graphic's reference
    *
    * \param lpGraphic graphic to place stencil/hit mask in KUIImage for
    * \param lpUIImage target KUIImage
    * \param x1 source X coordinate of top,left of portion of graphic to use as mask
    * \param y1 source Y coordinate of top,left of portion of graphic to use as mask
    * \param x2 source X coordinate of bottom,right of portion of graphic to use as mask
    * \param y2 source Y coordinate of bottom,right of portion of graphic to use as mask
    */
   void setStencilMaskByRef (KGraphic *lpGraphic, KUIImage *lpUIImage, float x1, float y1, float x2, float y2);

   /**
    * Dereference or unload graphic by reference
    *
    * \param lpGraphic graphic to be dereferenced or unloaded
    */
   void unloadGraphicByRef (KGraphic *lpGraphic);

   /**
    * Dereference or unload graphic by filename
    *
    * \param lpszFilename filename of graphic to be dereferenced or unloaded
    */
   void unloadGraphicByName (const char *lpszFilename);

   /**
    * Load numbered sequence of graphics from a set of files or add references to them, and automatically organize them as frames in a specified KUIImage. This method may be called from any thread
    *
    * \param lpszBaseFilename name of file to load first graphic from, for instance image001.png
    * \param nCount number of graphics to load; the filename will automatically be modified, for instance to image002.png, image003.png .. This method looks for a number before the file extension. 0 to load as many frames
    *               as possible
    * \param bHiQuality true to load the graphics as truecolor, false to load as 15 or 16 bits/pixel
    * \param bUseAlpha true to load the alpha masks from the graphics if present, false not to
    * \param nAlphaMaskThreshold when a mask is retained for hit testing purposes, value above or equal to which pixels will be considered opaque, and under which they will be considered transparent (-1 for no hit mask, default)
    * \param bForceReload true to force the graphic to be reloaded from disk as if it was the first reference, even if it was already loaded
    * \param bEnableNPOT true to enable loading graphics using non-power of two sized textures, if supported by the hardware, false to use power-of-two dimensions
    * \param bGenerateMipMaps true to generate mipmaps for the loaded graphics, false not to
    *
    * \return number of frames, if the number of graphics specified was loaded successfully, or 0 for failure (if any of the graphics failed to load, or if the filename was incorrectly structured)
    */
   long loadGraphicSequence (const char *lpszBaseFilename, long nCount, bool bHiQuality = true, bool bUseAlpha = true, long nAlphaMaskThreshold = -1, bool bForceReload = false,
                             bool bEnableNPOT = false, bool bGenerateMipMaps = false);

   /**
    * Upload this graphic to VRAM. This method may only be called from the main thread
    *
    * \param lpszBaseFilename name of first file whose graphics are to be uploaded, for instance image001.png
    * \param nCount number of graphics to upload; the filename will automatically be modified, for instance to image002.png, image003.png .. This method looks for a number before the file extension
    * \param lpUIImage KUIImage where the frames will be added
    * \param lpszVisualName name of visual to create in the KUIImage, where the added frames will be referenced
    * \param fFrameDuration number of milliseconds that each frame will be shown for, when animating
    * \param bGoForward true to go forward in frames, false to go backwards, when animating
    * \param bLoops true if sequence loops, when animating
    * \param bPingPongs if looping is enabled, true if sequence goes backwards after going forwards, or the other way around, after reaching the end, false if the sequence restarts at
    *                   the opposite frame (for instance nStartFrameIdx after reaching nEndFrameIdx)
    */
   void uploadGraphicSequence (const char *lpszBaseFilename, long nCount,
                               KUIImage *lpUIImage = NULL, const char *lpszVisualName = NULL, double fFrameDuration = 40.0, bool bGoForward = true, bool bLoops = false, bool bPingPongs = false);

   /**
    * Dereference or unload a numbered sequence of graphics
    *
    * \param lpszBaseFilename name of first file to dereference or unload, for instance image001.png
    * \param nCount number of graphics to dereference or unload; the filename will automatically be modified, for instance to image002.png, image003.png .. This method looks for a number before the file extension
    */
   void unloadGraphicSequence (const char *lpszBaseFilename, long nCount);

   /**
    * Enumerate currently loaded graphics in this list
    *
    * \param enumCallback method called back for each graphic with user data, the filename, reference count and KGraphic
    * \param lpUserData data to be passed to callback
    */
   void forEach (void (*enumCallback)(void *lpUserData, const char *lpszFilename, long nRefCount, KGraphic *lpGraphic), void *lpUserData);

   /** Free all loaded graphics */
   void freeAll (void);

   /**
    * Increase sequence number in a filename, for instance image002.png->image003.png, image9.png->image10.png, etc.
    *
    * \param szFilename filename to modify
    * \param nStartPos starting position (byte index of character after the last number in the sequence) in filename
    *
    * \private
    *
    * \return true for success, false for failure
    */
   static bool increaseSequence (char *szFilename, long &nStartPos);

private:
   /**
    * Create hit mask from an image
    *
    * \param lpImage image to create hit mask from
    * \param nAlphaMaskThreshold value above or equal to which pixels will be considered opaque
    * \param nShift number of bit shifts to the right to apply to pixels to get to alpha mask
    *
    * \return mask
    */
   unsigned char *createHitMask (KImage *lpImage, long nAlphaMaskThreshold, long nShift);

   /**
    * Recursively fit managed graphic into atlas
    *
    * \param lpCurNode current atlas node being explored
    * \param lpSrcRect coordinates of rectangle of managed graphic to be fitted
    * \param fAtlasWidth total width of the atlas, in pixels
    * \param fAtlasHeight total height of the atlas, in pixels
    *
    * \return node to assign managed graphic to, or NULL if there is no room
    */
   KManagedGraphicAtlasNode *insertIntoAtlas (KManagedGraphicAtlasNode *lpCurNode, KRectangle *lpSrcRect, float fAtlasWidth, float fAtlasHeight);

   /**
    * Recursively get the actually used size in atlas, once it has been fitted with all possible nodes
    *
    * \param lpCurNode current atlas node being explored
    * \param fWidth returned width in pixels
    * \param fHeight returned height in pixels
    */
   void getAtlasSize (KManagedGraphicAtlasNode *lpCurNode, float &fWidth, float &fHeight);

   /**
    * Recursively upload managed graphics to atlas
    *
    * \param lpAtlas atlas to upload to
    * \param lpCurNode current atlas node being explored
    */
   void uploadAtlasGraphics (KManagedGraphic *lpAtlas, KManagedGraphicAtlasNode *lpCurNode);

   /**
    * Recursively free all atlas nodes
    *
    * \param lpCurNode current atlas node being explored
    */
   void freeAtlas (KManagedGraphicAtlasNode *lpCurNode);

   /** Thread safety lock for accessing this list of graphics */
   KSysLock _lock;

   /** true if prefix has been set yet */
   bool _bPrefixSet;

   /** Prefix to be appended to relative pathnames */
   char _szPrefix[K_MAXPATH];

   /** Buffer for composing pathnames */
   char _szPathBuffer[K_MAXPATH];

   /** Buffer for composing alpha texture pathnames */
   char _szAlphaPathBuffer[K_MAXPATH];

   /** Hashtable of all managed graphics in this list */
   KHashTable _hash;

   /** true to store managed graphics in atlases, false not to (default) */
   bool _bBuildAtlases;

   /** Maximum width, in pixels, of an individual atlas */
   long _nMaxAtlasWidth;

   /** Maximum height, in pixels, of an individual atlas */
   long _nMaxAtlasHeight;

   /** Maximum width, in pixels, of a managed graphic to fit into atlases; above this width, the graphic is put in its own texture */
   long _nMaxGraphicWidthForAtlas;

   /** Maximum height, in pxiels, of a managed graphic to fit into atlases; above this height, the graphic is put in its own texture */
   long _nMaxGraphicHeightForAtlas;

   /** List of graphic atlases being constructed */
   KList<KManagedGraphic> _lOpenAtlases;
};

#endif   /* _KANJI_KMANAGEDGRAPHIC_H */
