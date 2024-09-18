/**
 * Kanji rendering and I/O engine - Android OpenGL GPU graphic definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KGRAPHIC_H
#define  _KANJI_KGRAPHIC_H

/* Include definitions */
#include "KWindow.h"
#include "KImage.h"
#include "KList.h"

/** Alpha blending modes */
enum {
   K_ALPHA_ADDITIVE = 0,         /**< Add (K_ALPHAOP_SRC_ALPHA, K_ALPHAOP_ONE): rgba = rgba_source * blend + rgba_dest */
   K_ALPHA_NORMAL = 1,           /**< Normal (K_ALPHAOP_SRC_ALPHA, K_ALPHAOP_ONE_MINUS_SRC_ALPHA): rgba_dest = rgba_source * blend + rgba_dest * (1 - blend) */
   K_ALPHA_SATSUB = 2,           /**< Saturate source and darken destination (K_ALPHAOP_ONE, K_ALPHAOP_ONE_MINUS_SRC_ALPHA): rgba_dest = rgba_source + rgba_dest * (1 - blend) */
   K_ALPHA_SUBSTRACTIVE = 3,     /**< Darken destination (K_ALPHAOP_ZERO, K_ALPHAOP_ONE_MINUS_SRC_ALPHA): rgba_dest = -rgba_dest * (1 - blend) */
   K_ALPHA_MULTIPLE = 5,         /**< Multiply (K_ALPHAOP_ZERO, K_ALPHAOP_SRC_COLOR): rgba_dest = r_source * r_dest, g_source * g_dest, b_source * b_dest, a_source * a_dest */
};

/** Alpha blending operations */
enum eKanjiAlphaOp {
	K_ALPHAOP_ZERO = 0,           /**< Multiply by 0, 0, 0, 0 */
	K_ALPHAOP_ONE,                /**< Multiply by 1, 1, 1, 1 */
	K_ALPHAOP_DST_COLOR,          /**< Multiply by r_dst, g_dst, b_dst, a_dst */
	K_ALPHAOP_ONE_MINUS_DST_COLOR,/**< Multiply by 1 - r_dst, 1 - g_dst, 1 - b_dst, 1 - a_dst */
	K_ALPHAOP_SRC_ALPHA,          /**< Multiply by a_src, a_src, a_src, a_src */
	K_ALPHAOP_ONE_MINUS_SRC_ALPHA,/**< Multiply by 1 - a_src, 1 - a_src, 1 - a_src, 1 - a_src */
	K_ALPHAOP_DST_ALPHA,          /**< Multiply by a_dst, a_dst, a_dst, a_dst */
	K_ALPHAOP_ONE_MINUS_DST_ALPHA,/**< Multiply by 1 - a_dst, 1 - a_dst, 1 - a_dst, 1 - a_dst */
	K_ALPHAOP_SRC_ALPHA_SATURATE, /**< Multiply by f, f, f, f, 1, where f is min (a_src, 1 - a_dst) */
	K_ALPHAOP_SRC_COLOR,          /**< Multiply by r_src, g_src, b_src, a_src */
	K_ALPHAOP_ONE_MINUS_SRC_COLOR,/**< Multiply by 1 - r_src, 1 - g_src, 1 - b_src, 1 - a_src */
};

/** Coordinate wrapping modes */
enum {
   K_WRAP_CLAMP = 0,          /**< Clamp to edge */
   K_WRAP_REPEAT = 1,         /**< Repeat (0..n, 0..n, 0..n ...) */
   K_WRAP_MIRROR = 2,         /**< Mirror (0..n..0..n..0..n ...) */
};

/** Section of an image that is spliced into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage */
typedef struct KGraphicSpliceStruct {
   float fXSrc1;              /**< Source X coordinate of top, left pixel in image */
   float fYSrc1;              /**< Source Y coordinate of top, left pixel in image */
   float fXSrc2;              /**< Source X coordinate + 1 of bottom, right pixel in image */
   float fYSrc2;              /**< Source Y coordinate + 1 of bottom, right pixel in image */
   long nGraphic;             /**< Index of texture to copy source rectangle to */
   float fXDst;               /**< Destination X coordinate of top, left pixel in texture */
   float fYDst;               /**< Destination Y coordinate of top, left pixel in texture */
} KGraphicSplice;

/** Maximum number of splices per graphic */
#define KGRAPHIC_MAXSPLICES   (32)

/** Maximum number of textures per graphic, when splicing an image */
#define KGRAPHIC_MAXTEXTURES  (32)

/**
 * Method optionally called for every pixel by grabBackBuffer() before the pixels are copied to the graphic
 */
typedef void (*pixelCallBackPtr)(long nX, long nY, long *r, long *g, long *b);

/**
 * Class for graphics to be loaded into OpenGL textures and drawn into the backbuffer or a render target
 */

class KGraphic : public KObjectListable {
public:
   /** Destructor */
   virtual ~KGraphic () { ; }

   /**
    * Set transparent color for this graphic. If enabled, the specified color will be assigned a fully transparent alpha value when found,
    * otherwise the graphic will use the alpha channel information in the image file. Must be called before loadPicture()
    *
    * \param bEnable true to enable, false to disable
    * \param r red value of color key (0..255)
    * \param g red value of color key (0..255)
    * \param b red value of color key (0..255)
    */
   virtual void setColorKey (bool bEnable, unsigned char r, unsigned char g, unsigned char b) = 0;
   
   /**
    * Prioritize color or alpha information if this graphic contains an alpha mask and is loaded into a 15 or 16 bits/pixel texture. Must
    * be called before loadPicture()
    *
    * \param bPrioritizeColor true to prioritize color bits, false to prioritize the alpha mask bits
    */
   virtual void setTextureColorPriority (bool bPrioritizeColor) = 0;
   
   /**
    * Set whether this graphic will use allowTextureWrap(). Graphics that use texture coordinate wrapping need to be loaded differently on some renderers. Must be called before loadPicture()
    */
   virtual void setTextureWrapUsage (bool bTextureWrappingUsed) = 0;
   
   /**
    * Generate mipmaps for this graphic. This is only needed for graphics that will be used as textures on 3d objects, in order to generate filtered images at various levels of
    * detail. Must be called before loadPicture()
    *
    * \param bEnable true to enable generating mipmaps, false to disable
    */
   virtual void generateMipMaps (bool bEnable) = 0;
   
   /**
    * Enable loading a texture into this graphic as non-Power of Two dimensions, if supported by the GPU
    *
    * \param bEnable true to enable non-Power Of Two dimensions for this graphic, false to disable
    */
   virtual void enableNonPow2Dimensions (bool bEnable) = 0;

   /**
    * Get the atlas that this graphic is part of, if any
    *
    * \return atlas graphic, NULL for none
    */
   virtual KGraphic *getAtlas (void) = 0;
   
   /**
    * Set this graphic as part of another, atlas graphic. This must be called before loadPicture() or
    * makePictureFromArray().
    *
    * \param lpAtlasGraphic atlas to use for this graphic
    * \param nXOffset X offset into the atlas for top,left pixel of this graphic
    * \param nYOffset Y offset into the atlas for top,left pixel of this graphic
    * \param nWidth width of this graphic; if this is set, the graphic is considered as already loaded in its atlas
    * \param nHeight height of this graphic; if this is set, the graphic is considered as already loaded in its atlas
    */
   virtual void setInAtlas (KGraphic *lpAtlasGraphic, long nXOffset, long nYOffset, long nWidth = 0, long nHeight = 0) = 0;
   
   /**
    * Load color or color+alpha image from a file into this graphic
    *
    * \param lpszFileName name of file to load image from
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bUseAlpha true to load the alpha mask from the image if present, false not to
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    * \param lpSplices list of splices to use for this image
    *
    * \return true for success, false for failure
    */
   virtual bool loadPicture (const char *lpszFileName, bool bHiQuality, bool bUseAlpha, bool bDoSplice = false, long nStep = 0, KGraphicSplice *lpSplices = NULL) = 0;

   /**
    * Decode color or color+alpha image from a memory buffer into this graphic
    *
    * \param lpszFileName image's filename (only used to look at the extension, such as .tga or .jpg, and determine the type of image to be decoded)
    * \param lpBuffer memory buffer to decode image from
    * \param nBufferSize size of memory buffer containing encoded image, in bytes
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bUseAlpha true to load the alpha mask from the image if present, false not to
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    * \param lpSplices list of splices to use for this image
    *
    * \return true for success, false for failure
    */
   virtual bool loadPictureFromPtr (const char *lpszFileName, const unsigned char *lpBuffer, long nBufferSize, bool bHiQuality, bool bUseAlpha,
                                    bool bDoSplice = false, long nStep = 0, KGraphicSplice *lpSplices = NULL) = 0;

   /**
    * Load seperate color and alpha images from two different files and combine them into this graphic
    *
    * \param lpszFileName name of file to load color from
    * \param lpszMaskFileName name of file to load alpha mask from
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    * \param lpSplices list of splices to use for this image
    *
    * \return true for success, false for failure
    */
   virtual bool loadPictureWithMask (const char *lpszFileName, const char *lpszMaskFileName, bool bHiQuality,
                                     bool bDoSplice = false, long nStep = 0, KGraphicSplice *lpSplices = NULL) = 0;

   /**
    * Decode seperate color and alpha images from two different memory buffers and combine them into this graphic
    *
    * \param lpszFileName color image's filename (only used to look at the extension, such as .tga or .jpg, and determine the type of image to be decoded)
    * \param lpBuffer memory buffer to decode color from
    * \param nBufferSize size of memory buffer containing encoded color image, in bytes
    * \param lpszMaskFileName alpha mask image's filename (only used to look at the extension)
    * \param lpMaskBuffer memory buffer to decode alpha mask from
    * \param nMaskBufferSize size of memory buffer containing encoded alpha mask image, in bytes
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    * \param lpSplices list of splices to use for this image
    *
    * \return true for success, false for failure
    */
   virtual bool loadPictureWithMaskFromPtr (const char *lpszFileName, const unsigned char *lpBuffer, long nBufferSize,
                                            const char *lpszMaskFileName, const unsigned char *lpMaskBuffer, long nMaskBufferSize, bool bHiQuality,
                                            bool bDoSplice = false, long nStep = 0, KGraphicSplice *lpSplices = NULL) = 0;

   /**
    * Load array of pixels into this graphic
    *
    * \param lpPixels array of nWidth*nHeight ARGB pixels to be loaded
    * \param nWidth width in pixels
    * \param nHeight height in pixels
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bUseAlpha true to use the alpha mask from the array, false not to
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage
    * \param lpSplices list of splices to use for this image
    *
    * \return true for success, false for failure
    */
   virtual bool makePictureFromArray (const unsigned int *lpPixels, long nWidth, long nHeight, bool bHiQuality, bool bUseAlpha = true,
                                      bool bDoSplice = false, KGraphicSplice *lpSplices = NULL) = 0;

   /**
    * Update graphic from array of pixels previously supplied by makePictureFromArray() or makePictureFromArrayWithMask()
    */
   virtual void pollArray (void) = 0;

   /**
    * Make this graphic into an atlas that other graphics can be loaded into. Once all the graphics have been loaded into the atlas with loadPicture(), loadPictureFromMask(),
    * makePictureFromArray(), etc., finalizeAtlas() needs to be called in order to close and upload the atlas to the card
    *
    * \param nWidth atlas width, in pixels
    * \param nHeight atlas height, in pixels
    * \param bAddBorders true to add borders to graphics added to this atlas, to avoid GPU artifacts (graphics must be placed accordingly, with a margin)
    */
   virtual void makeAtlas (long nWidth, long nHeight, bool bAddBorders = false) = 0;
   
   /**
    * Finalize the contents of this atlas graphic, and upload it. No more graphics can be loaded into this atlas after this method is called
    *
    * \return true for success, false for failure
    */
   virtual bool finalizeAtlas (void) = 0;
   
   /**
    * Free all resources used by this graphic
    */
   virtual void freePicture (void) = 0;

   /**
    * Flag that this render target will be cleared (using the transparent color for render targets with an alpha channel, and using black for ones without) on
    * the next beginRenderToTarget()
    */
   virtual void clearRenderTarget (void) = 0;

   /**
    * Make this graphic a render target, that can be used to draw other graphics into
    *
    * \param nWidth width of render target, in pixels
    * \param nHeight height of render target, in pixels
    * \param bHiQuality true to create truecolor render target, false to create as 15 or 16 bits/pixel
    * \param bCreateAlpha true to create an alpha channel for the target, false not to
    * \param bCreateDepthBuffer true to create a depth buffer (Z-buffer) for the render target, false not to
    *
    * \return true for success, false for failure
    */
   virtual bool makeRenderTarget (long nWidth, long nHeight, bool bHiQuality = false, bool bCreateAlpha = false, bool bCreateDepthBuffer = false) = 0;

   /**
    * Start drawing to this render target
    */
   virtual void beginRenderToTarget (void) = 0;

   /**
    * Stop drawing to this render target, and resume drawing to the previous one (render target or backbuffer)
    */
   virtual void endRenderToTarget (void) = 0;

   /**
    * Check if this render target's contents were lost. This can happen after a fullscreen/windowed mode change or other conditions
    *
    * \return true if contents were lost, false if they weren't
    */
   virtual bool isRenderTargetLost (void) = 0;

   /**
    * Copy contents of the backbuffer into this graphic
    *
    * \param lpPixelCallBack method called for every pixel in order to alter its value before it is copied to the graphic, NULL for none
    * \param bDoSplice true to enable splicing the loaded image into multiple GPU textures that fit the maximum hardware texture size and optimize VRAM usage
    * \param lpSplices list of splices to use for this image
    */
   virtual bool grabBackBuffer (pixelCallBackPtr lpPixelCallBack, bool bDoSplice = false, KGraphicSplice *lpSplices = NULL) = 0;

   /**
    * Get width of this graphic, in pixels
    *
    * \return width in pixels
    */
   virtual float getWidth (void) = 0;

   /**
    * Get height of this graphic, in pixels
    *
    * \return height in pixels
    */
   virtual float getHeight (void) = 0;

   /**
    * Get actual width of this graphic, taking into account power-of-2 alignment and other hardware contraints
    *
    * \return actual width in pixels
    */
   virtual float getSurfaceWidth (void) = 0;

   /**
    * Get actual height of this graphic, taking into account power-of-2 alignment and other hardware contraints
    *
    * \return actual width in pixels
    */
   virtual float getSurfaceHeight (void) = 0;

   /**
    * Check if filtering is enabled for this graphic, for instance when scaling and rotating
    *
    * \return true if filtering is enabled, false if not
    */
   virtual bool getTextureQuality (void) = 0;
   
   /**
    * Enable or disable filtering for this graphic, for instance when scaling and rotating
    *
    * \param bEnableFiltering true to enable, false to disable
    */
   virtual void setTextureQuality (bool bEnableFiltering) = 0;

   /**
    * Set alpha blending mode for this graphic
    *
    * \param nAlphaMode new alpha blending mode (K_ALPHA_xxx)
    */
   virtual void setAlphaMode (long nAlphaMode) = 0;

   /**
    * Get source alpha blending operation for the color channels of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   virtual eKanjiAlphaOp getSrcAlphaOp (void) = 0;
   
   /**
    * Get destination alpha blending operation for the color channels of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   virtual eKanjiAlphaOp getDstAlphaOp (void) = 0;
   
   /**
    * Get source alpha blending operation for the alpha channel of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   virtual eKanjiAlphaOp getAlphaSrcAlphaOp (void) = 0;
   
   /**
    * Get destination alpha blending operation for the alpha channel of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   virtual eKanjiAlphaOp getAlphaDstAlphaOp (void) = 0;
   
   /**
    * Set alpha blending operations for this graphic
    *
    * \param nSrcAlphaOp new source alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nDstAlphaOp new destination alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nAlphaSrcAlphaOp new source alpha blending op for the alpha channel (K_ALPHAOP_xxx)
    * \param nAlphaDstAlphaOp new destination alpha blending op for the alpha channel (K_ALPHAOP_xxx)
    */
   virtual void setAlphaOp (eKanjiAlphaOp nSrcAlphaOp, eKanjiAlphaOp nDstAlphaOp,
                            eKanjiAlphaOp nAlphaSrcAlphaOp = (eKanjiAlphaOp) -1, eKanjiAlphaOp nAlphaDstAlphaOp = (eKanjiAlphaOp) -1) = 0;
   
   /**
    * Get source coordinates wrapping mode for this graphic
    *
    * \return wrapping mode (K_WRAP_xxx)
    */
   virtual long getTextureWrappingMode (void) = 0;
   
   /**
    * Set source coordinates wrapping mode for this graphic
    *
    * \param nWrappingMode new wrapping mode (K_WRAP_xxx)
    */
   virtual void allowTextureWrap (long nWrappingMode) = 0;

   /**
    * Get vertex (tinting) color for this graphic
    *
    * \param lpColor 4 floats where to store red, green, blue and alpha components
    */
   virtual void getBlitColor (float *lpColor) = 0;
   
   /**
    * Set vertex (tinting) color for this graphic
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   virtual void setBlitColor (float r, float g, float b, float a) = 0;
   
   /**
    * Get scaling applied to horizontal texture coordinates for this graphic
    *
    * \return X texture coordinates scale
    */
   virtual float getUScale (void) = 0;
   
   /**
    * Get scaling applied to vertical texture coordinates for this graphic
    *
    * \return Y texture coordinates scale
    */
   virtual float getVScale (void) = 0;
   
   /**
    * Set scaling applied to texture coordinates for this graphic
    *
    * \param fUScale new X texture coordinates scale
    * \param fVScale new Y texture coordinates scale
    * \param bScaleDimensions true to scale the reported graphic and surface width and height as well, false not to (default)
    */
   virtual void setUVScale (float fUScale, float fVScale, bool bScaleDimensions = false) = 0;

   /**
    * Set pixel
    *
    * \param dx screen X coordinate
    * \param dy screen Y coordinate
    * \param r red component of pixel color
    * \param g green component of pixel color
    * \param b blue component of pixel color
    * \param fBlend blending factor
    */
   virtual void plotPixel (float dx, float dy, float r, float g, float b, float fBlend) = 0;

   /**
    * Draw line between two points
    *
    * \param dx1 screen X coordinate of first point
    * \param dy1 screen Y coordinate of first point
    * \param dx2 screen X coordinate of second point
    * \param dy2 screen Y coordinate of second point
    * \param r red component of line color
    * \param g green component of line color
    * \param b blue component of line color
    * \param fBlend blending factor
    */
   virtual void drawLine (float dx1, float dy1, float dx2, float dy2, float r, float g, float b, float fBlend) = 0;

   /**
    * Draw rectangle
    *
    * \param dx1 screen X coordinate of top, left pixel
    * \param dy1 screen Y coordinate of top, left pixel
    * \param dx2 screen X coordinate + 1 of bottom, right pixel
    * \param dy2 screen Y coordinate + 1 of bottom, right pixel
    * \param r red component of rectangle color
    * \param g green component of rectangle color
    * \param b blue component of rectangle color
    * \param fBlend blending factor
    */
   virtual void drawRect (float dx1, float dy1, float dx2, float dy2, float r, float g, float b, float fBlend) = 0;

   /**
    * Draw quad
    *
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param r red component of quad color
    * \param g green component of quad color
    * \param b blue component of quad color
    * \param fBlend blending factor
    */
   virtual void drawArbitraryQuad (float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4,
                                   float r, float g, float b, float fBlend) = 0;

   /**
    * Draw color gradient
    *
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param r1 red component of color at first point
    * \param g1 green component of color at first point
    * \param b1 blue component of color at first point
    * \param fBlend1 blending factor at first point
    * \param r2 red component of color at second point
    * \param g2 green component of color at second point
    * \param b2 blue component of color at second point
    * \param fBlend2 blending factor at second point
    * \param r3 red component of color at third point
    * \param g3 green component of color at third point
    * \param b3 blue component of color at third point
    * \param fBlend3 blending factor at third point
    * \param r4 red component of color at fourth point
    * \param g4 green component of color at fourth point
    * \param b4 blue component of color at fourth point
    * \param fBlend4 blending factor at fourth point
    */
   virtual void drawGradient (float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4,
                              float r1, float g1, float b1, float fBlend1, float r2, float g2, float b2, float fBlend2,
                              float r3, float g3, float b3, float fBlend3, float r4, float g4, float b4, float fBlend4) = 0;
   
   /**
    * Blit portion of this graphic, at integral screen coordinates, without alpha blending, without transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    */
   virtual void blitRect (float sx1, float sy1, float sx2, float sy2, short dx, short dy, bool bXFlip = false, bool bYFlip = false) = 0;

   /**
    * Blit portion of this graphic, at integral screen coordinates, without alpha blending, with transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitRectFx (float sx1, float sy1, float sx2, float sy2, short dx, short dy, float fZAngle, float fScale,
                            bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic, at integral screen coordinates, with alpha blending, without transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    */
   virtual void blitAlphaRect (float sx1, float sy1, float sx2, float sy2, short dx, short dy, bool bXFlip = false, bool bYFlip = false) = 0;

   /**
    * Blit portion of this graphic, at integral screen coordinates, with alpha blending, with transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitAlphaRectFx (float sx1, float sy1, float sx2, float sy2, short dx, short dy, float fZAngle, float fScale, float fBlend,
                                 bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic as a quad
    *
    * \param sx1 source X coordinate of first point in graphic, in clockwise order
    * \param sy1 source Y coordinate of first point in graphic, in clockwise order
    * \param sx2 source X coordinate of second point in graphic, in clockwise order
    * \param sy2 source Y coordinate of second point in graphic, in clockwise order
    * \param sx3 source X coordinate of third point in graphic, in clockwise order
    * \param sy3 source Y coordinate of third point in graphic, in clockwise order
    * \param sx4 source X coordinate of fourth point in graphic, in clockwise order
    * \param sy4 source Y coordinate of fourth point in graphic, in clockwise order
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    */
   virtual void blitArbitraryQuad (float sx1, float sy1, float sx2, float sy2, float sx3, float sy3, float sx4, float sy4,
                                   float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4,
                                   float fBlend = 1.0f) = 0;

   /**
    * Blit portion of this graphic and stretch it to an arbitrary rectangle on screen
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx1 screen X coordinate of top, left pixel
    * \param dy1 screen Y coordinate of top, left pixel
    * \param dx2 screen X coordinate + 1 of bottom, right pixel
    * \param dy2 screen Y coordinate + 1 of bottom, right pixel
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void stretchAlphaRect (float sx1, float sy1, float sx2, float sy2, float dx1, float dy1, float dx2, float dy2,
                                  float fBlend = 1.0f, float fZAngle = 0.0f, bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic, at subpixel screen coordinates, without alpha blending, without transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    */
   virtual void blitRectF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, bool bXFlip = false, bool bYFlip = false) = 0;

   /**
    * Blit portion of this graphic, at subpixel screen coordinates, without alpha blending, with transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitRectFxF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, float fZAngle, float fScale, bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic, at subpixel screen coordinates, with alpha blending, without transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    */
   virtual void blitAlphaRectF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, bool bXFlip = false, bool bYFlip = false) = 0;

   /**
    * Blit portion of this graphic, at subpixel screen coordinates, with alpha blending, with transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitAlphaRectFxF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, float fZAngle, float fScale, float fBlend,
                                  bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;
   
   /**
    * Blit portion of this graphic, using another graphic's alpha mask instead of this one, at subpixel screen coordinates, with stretching, with alpha blending, with transforms
    *
    * \param sx1 source X coordinate of top, left pixel in this graphic
    * \param sy1 source Y coordinate of top, left pixel in this graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in this graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in this graphic
    * \param lpMaskGraphic graphic to use alpha mask of
    * \param mx1 source X coordinate of top, left pixel in alpha mask graphic
    * \param my1 source Y coordinate of top, left pixel in alpha mask graphic
    * \param mx2 source X coordinate + 1 of bottom, right pixel in alpha mask graphic
    * \param my2 source Y coordinate + 1 of bottom, right pixel in alpha mask graphic
    * \param dx1 screen X coordinate of top, left pixel
    * \param dy1 screen Y coordinate of top, left pixel
    * \param dx2 screen X coordinate + 1 of bottom, right pixel
    * \param dy2 screen Y coordinate + 1 of bottom, right pixel
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    */
   virtual void blitAlphaRectMaskedF (float sx1, float sy1, float sx2, float sy2,
                                      KGraphic *lpMaskGraphic, float mx1, float my1, float mx2, float my2,
                                      float dx1, float dy1, float dx2, float dy2,
                                      bool bXFlip = false, bool bYFlip = false) = 0;

   /**
    * Blit portion of selected graphic to this vertex buffer, using matrix transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx1 screen X coordinate of top, left pixel
    * \param dy1 screen Y coordinate of top, left pixel
    * \param dx2 screen X coordinate + 1 of bottom, right pixel
    * \param dy2 screen Y coordinate + 1 of bottom, right pixel
    * \param lpParentMatrix matrix to apply for this blit before adding the specified rotation, scaling and shear transforms
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScaleX X scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fScaleY Y scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fShearX X shearing factor
    * \param fShearY Y shearing factor
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitAlphaRectMatrixF (float sx1, float sy1, float sx2, float sy2, float dx1, float dy1, float dx2, float dy2,
                                      KMatrix *lpParentMatrix, float fZAngle, float fScaleX, float fScaleY, float fShearX, float fShearY, float fBlend,
                                      bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;
   
   /**
    * Get value to divide X coordinates by in order to get U coordinates for 3d vertices, when using this graphic as a texture
    *
    * \return value to divide X by in order to get U for this graphic
    */
   virtual float get3dUDivider (void) = 0;
   
   /**
    * Get value to divide Y coordinates by in order to get V coordinates for 3d vertices, when using this graphic as a texture
    *
    * \return value to divide Y by in order to get V for this graphic
    */
   virtual float get3dVDivider (void) = 0;
   
   /**
    * Check if this graphic's image is spliced into multiple textures
    *
    * \return true if spliced, false if not
    */
   virtual bool isSpliced (void) = 0;

   /**
    * Get color or color+alpha image. The image is only available if loadPicture() was called with a "load only" step, and before the graphic is uploaded with the "upload only" step.
    *
    * \private
    *
    * \return image
    */
   virtual KImage *getColorImage (void) = 0;
   
   /**
    * Get alpha image. The image is only available if loadPicture() was called with a "load only" step, and before the graphic is uploaded with the "upload only" step.
    *
    * \private
    *
    * \return image
    */
   virtual KImage *getAlphaImage (void) = 0;
   
   /**
    * Blit portion of this graphic, at integral screen coordinates, without alpha blending, with transforms - internal, does not need to be called by
    * the game except for source compatibility purposes
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitSplicedRectFx (float sx1, float sy1, float sx2, float sy2, short dx, short dy, float fZAngle, float fScale,
                                   bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic, at integral screen coordinates, with alpha blending, with transforms - internal, does not need to be called by
    * the game except for source compatibility purposes
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitSplicedAlphaRectFx (float sx1, float sy1, float sx2, float sy2, short dx, short dy, float fZAngle, float fScale, float fBlend,
                                        bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic, at subpixel screen coordinates, without alpha blending, with transforms - internal, does not need to be called by
    * the game except for source compatibility purposes
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitSplicedRectFxF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, float fZAngle, float fScale,
                                    bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Blit portion of this graphic, at integral screen coordinates, with alpha blending, with transforms - internal, does not need to be called by
    * the game except for source compatibility purposes
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitSplicedAlphaRectFxF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, float fZAngle, float fScale, float fBlend,
                                         bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

};

#endif   /* _KANJI_KGRAPHIC_H */
