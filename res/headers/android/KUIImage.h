/**
 * Kanji game engine - image UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UIIMAGE_H
#define  _KANJI_UIIMAGE_H

/* Include definitions */
#include "KUIElement.h"
#include "KGraphic.h"
#
/** Messages sent by KUIImage */
enum {
   K_UIIMAGE_MSGANIMENDED = K_UIMSG ('k','i','m'),    /**< The frame sequence is done animating */
   K_UIIMAGE_MSGFRAMEREACHED,                         /**< The frame sequence is displaying the specified trigger frame */
};

/** One frame displayed by this image */

typedef struct {
   /** Graphic */
   KGraphic *lpGraphic;

   /** Hit mask, or NULL for none */
   const unsigned char *lpHitMask;

   /** Hit mask's width in pixels, -1 to take the graphic's width */
   long nHitMaskWidth;

   /** Hit mask's height in pixels, -1 to take the graphic's width */
   long nHitMaskHeight;

   /** Source coordinates in graphic */
   KRectangle rcSrcCoord;

   /** Offset to apply to destination (screen) coordinates for this frame */
   KVector2 vDstOffset;

   /** true to enable tiling, false not to */
   bool bEnableTiling;

   /** true to align tiling with the bottom right, false to align with the top left */
   bool bReverseTiling;

   /** true to enable X stretching, false not to */
   bool bEnableXStretching;

   /** X range of the portion of the graphic that stretches, if enabled */
   float fXStretch[2];

   /** true to enable Y stretching, false not to */
   bool bEnableYStretching;

   /** Y range of the portion of the graphic that stretches, if enabled */
   float fYStretch[2];

   /** When stretching is enabled, true to stretch the sides and keep the middle part intact; false to stretch the middle and keep the sides intact */
   bool bStretchSides;

   /** true to mirror over the X axis when drawing */
   bool bXFlip;

   /** true to mirror over the Y axis when drawing */
   bool bYFlip;

   /** true to draw an arbitrary quad, false to draw a rectangle or stretched rectangle */
   bool bArbitraryQuadEnabled;

   /** true if source coordinates have been defined for drawing an arbitrary quad, false if not */
   bool bQuadSrcCoordDefined;

   /** true if destination (screen) coordinates have been defined for drawing an arbitrary quad, false if not */
   bool bQuadDstCoordDefined;

   /** true if anchor has been defined in normalized coordinates */
   bool bNormalizedAnchorDefined;

   /** true if pivot has been defined in normalized coordinates */
   bool bNormalizedCenterDefined;

   /** Source coordinates of each point of the quad in the graphic */
   KVector2 vQuadSrcCoord[4];

   /** Source coordinates anchor point */
   KVector2 vQuadSrcAnchor;

   /** Source coordinates anchor point in normalized coordinates, if set */
   KVector2 vNormalizedQuadSrcAnchor;

   /** Source coordinates pivot point */
   KVector2 vQuadSrcCenter;

   /** Source coordinates pivot point in normalized coordinates, if set */
   KVector2 vNormalizedQuadSrcCenter;

   /** Source coordinates horizontal scaling factor */
   float fQuadSrcScaleX;

   /** Source coordinates vertical scaling factor */
   float fQuadSrcScaleY;

   /** Source coordinates Z rotation angle */
   float fQuadSrcZAngle;

   /** Destination coordinates of each point of the quad, relative to the UI element's position */
   KVector2 vQuadDstCoord[4];

   /** Bounding hierarchy for this frame, NULL if none and this frame uses the default bounding rectangle */
   KUIBounds *lpBounds;

   /** true if the bounding hierarchy for this frame shouldn't be freed when unset or when the element is deleted */
   bool bDoNotFreeBounds;

   /** true if this frame defines a subrectangle that is fully opaque and can be drawn front to back when drawing with the Z buffer is enabled */
   bool bOpaqueRectEnabled;

   /** When enabled, subrectangle for this frame, in source coordinates, that is fully opaque */
   KRectangle rcOpaque;

   /** Mask graphic */
   KGraphic *lpMaskGraphic;

   /** Source coordinates in mask graphic */
   KRectangle rcSrcMask;
} KUIImageFrame;

/** One sequence of frames (visual) displayed by this image */

typedef struct {
   /** true if this visual is in use; false if it is an empty slot */
   bool bInUse;

   /** Name of this visual */
   char szVisualName[100];

   /** Index of first frame to show */
   long nStartFrameIdx;

   /** Index of last frame to show */
   long nEndFrameIdx;

   /** Index of frame currently shown */
   long nCurFrameIdx;

   /** Index of frame to cross-blend to */
   long nCrossBlendFrameIdx;

   /** Current factor for cross blending frames */
   float fCrossBlendFactor;

   /** Index of frame that triggers a K_UIIMAGE_MSGFRAMEREACHED event, -1 for none */
   long nTriggerFrameIdx;

   /** Duration of each frame, in milliseconds */
   double fFrameDuration;

   /** Number of milliseconds left before advancing to the next frame */
   double fCurFrameMillis;

   /** true if currently going forward, false if going backwards */
   bool bGoForward;

   /** true if sequence loops once it reaches the end */
   bool bLoops;

   /** If looping is enabled, true if sequence goes backwards after going forwards, or the other way around, after reaching the end, false if the sequence restarts at
    * the opposite frame (for instance nStartFrameIdx after reaching nEndFrameIdx) */
   bool bPingPongs;

   /** true if trigger frame was reached since last polling */
   bool bTriggerReached;
} KUIImageVisual;

/**
 * Image UI element
 */

class KUIImage : public KUIElement {
public:
   /**
    * Check if the supplied element is an image, and cast as KUIImage if it is
    *
    * \return KUIImage if element is an image, NULL otherwise
    */
   static KUIImage *isImage (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIImage (KUIElement *lpParentElem);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    * \param x X coordinate of top,left of element, relative to the parent
    * \param y Y coordinate of top,left of element, relative to the parent
    * \param lpszElemName image element's name
    * \param lpszVisualName name of frame sequence (visual) to be added
    * \param lpszGraphicFilename filename to load graphic from, using KManagedGraphic; an instance of KGame must have been created
    * \param nAlphaMaskThreshold when a mask is retained for hit testing purposes, value above or equal to which pixels will be considered opaque, and under which they will be considered transparent (-1 for no hit mask, default)
    * \param bSetAsStencil enable stencil mask as well as hit mask; an alpha threshold value must have been specified as well
    */
   KUIImage (KUIElement *lpParentElem, float x, float y, const char *lpszElemName, const char *lpszVisualName, const char *lpszGraphicFilename, long nAlphaMaskThreshold = -1, bool bSetAsStencil = false);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    * \param x X coordinate of top,left of element, relative to the parent
    * \param y Y coordinate of top,left of element, relative to the parent
    * \param lpszElemName image element's name
    * \param lpszVisualName name of frame sequence (visual) to be added
    * \param nMaxFrames maximum number of frames of animation to be loaded (frames must be sequentially named: image1,2,3.. image001,002,003..)
    * \param lpszGraphicFilename filename to load graphic from, using KManagedGraphic; an instance of KGame must have been created
    * \param fFrameDuration number of milliseconds that each frame will be shown for, when animating
    * \param bGoForward true to go forward in frames, false to go backwards, when animating
    * \param bLoops true if sequence loops, when animating
    * \param bPingPongs if looping is enabled, true if sequence goes backwards after going forwards, or the other way around, after reaching the end, false if the sequence restarts at
    *                   the opposite frame (for instance nStartFrameIdx after reaching nEndFrameIdx)
    */
   KUIImage (KUIElement *lpParentElem, float x, float y, const char *lpszElemName, const char *lpszVisualName, long nMaxFrames, const char *lpszGraphicFilename,
             double fFrameDuration = 40.0, bool bGoForward = true, bool bLoops = false, bool bPingPongs = false);

   /** Destructor */
   virtual ~KUIImage ();

   /**
    * Copy all parameters except for the name, identifier, user data, state, layer number, visibility, input and message delivery status, and message callback, from another element
    *
    * \param lpFromElement element to copy from
    */
   virtual void copyFromTemplate (KUIElement *lpFromElement);

   /**
    * Get tinting color
    *
    * \param lpColor 4 floats where to store red, green, blue and alpha components
    */
   void getColor (float *lpColor);

   /**
    * Set tinting color
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setColor (float r, float g, float b, float a);

   /**
    * Get lightness factor (multiplied to each of the rgb components, used for darkening this image)
    *
    * \return lightness factor, 0..1 (fully dark..normal color)
    */
   float getLightness (void) const;

   /**
    * Set lightness factor (multiplied to each of the rgb components, used for darkening this image)
    *
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setLightness (float fLightness);

   /**
    * Get over-brightness factor
    *
    * \return over-brightness factor, 0..1 (normal.. fully overlit)
    */
   float getOverBrightness (void) const;

   /**
    * Set over-brightness factor
    *
    * \param fOverBrightness over-brightness factor, 0..1 (normal.. fully overlit)
    */
   void setOverBrightness (float fOverBrightness);

   /**
    * Set alpha blending mode
    *
    * \param nAlphaMode new alpha blending mode (K_ALPHA_xxx)
    */
   void setAlphaMode (long nAlphaMode);

   /**
    * Get source alpha blending operation for the color channels of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getSrcAlphaOp (void) const;
   
   /**
    * Get destination alpha blending operation for the color channels of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getDstAlphaOp (void) const;
   
   /**
    * Get source alpha blending operation for the alpha channel of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getAlphaSrcAlphaOp (void) const;
   
   /**
    * Get destination alpha blending operation for the alpha channel of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getAlphaDstAlphaOp (void) const;

   /**
    * Set alpha blending operations for this graphic
    *
    * \param nSrcAlphaOp new source alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nDstAlphaOp new destination alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nAlphaSrcAlphaOp new source alpha blending op for the alpha channel (K_ALPHAOP_xxx)
    * \param nAlphaDstAlphaOp new destination alpha blending op for the alpha channel (K_ALPHAOP_xxx)
    */
   void setAlphaOp (eKanjiAlphaOp nSrcAlphaOp, eKanjiAlphaOp nDstAlphaOp,
                    eKanjiAlphaOp nAlphaSrcAlphaOp = (eKanjiAlphaOp) -1, eKanjiAlphaOp nAlphaDstAlphaOp = (eKanjiAlphaOp) -1);

   /**
    * Set single graphic for this image to display. Calling this method automatically defines a single frame and a single frame sequence for this image
    *
    * \param lpGraphic graphic to use as source for this image
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    * \param lpHitMask hit mask for hit testing, NULL for none
    * \param nHitMaskWidth hit mask's width in pixels, -1 to take the graphic's width (default)
    * \param nHitMaskHeight hit mask's height in pixels, -1 to take the graphic's width (default)
    */
   void setGraphic (KGraphic *lpGraphic, float x1, float y1, float x2, float y2, const unsigned char *lpHitMask = NULL, long nHitMaskWidth = -1, long nHitMaskHeight = -1);

   /**
    * Enable or disable stretching the graphic to the size of the element. Stretching defines a range over X and
    * over Y that stretches, so that corners can remain untouched and only the middle stretches to fill the size of
    * the element. Calling this method automatically defines a single frame and a single frame sequence for this image
    *
    * \param bXEnable true to enable X stretching, false to disable
    * \param fXMin leftmost X coordinate in graphic of part that stretches
    * \param fXMax rightmost X coordinate in graphic of part that stretches, +1
    * \param bYEnable true to enable Y stretching, false to disable
    * \param fYMin topmost Y coordinate in graphic of part that stretches
    * \param fYMax bottommmost Y coordinate in graphic of part that stretches, +1
    */
   void setStretching (bool bXEnable, float fXMin, float fXMax, bool bYEnable, float fYMin, float fYMax);

   /**
    * Set destination coordinates offset for all frames
    *
    * \param vGlobalDstOffset global destination coordinates offset, in screen space
    */
   void setGlobalDstOffset (KVector2 vGlobalDstOffset);

   /**
    * Get root primitive of the bounding hierarchy of the first frame displayed by this element
    *
    * \return root bounding primitive, or NULL if no hierarchy is set and the element uses the default bounding rectangle
    */
   virtual KUIBounds *getBounds (void);

   /**
    * Set root primitive of the bounding hierarchy of the first frame of this element. By default, a bounding rectangle of (0, 0, width, height) is set.
    * The bounding hierarchy being set will also automatically be deleted when the element is deleted
    *
    * \param lpBounds root bounding primitive, NULL to remove hierarchy and use the default bounding rectangle
    * \param bDoNotFree true to avoid freeing previous bounding hierarchy if any; false to free previous hierarchy (default)
    */
   virtual void setBounds (KUIBounds *lpBounds, bool bDoNotFree = false);

   /**
    * Get total number of frames displayed by this image
    *
    * \return number of frames
    */
   long getFrameCount (void) const;

   /**
    * Set total number of frames displayed by this image
    *
    * \param nFrameCount number of frames
    */
   void setFrameCount (long nFrameCount);

   /**
    * Get frame data
    *
    * \param nFrameIdx index of frame to get data for
    * \param lpFrame frame that receives the data
    *
    * \return true if successful, false if not (frame index out of bounds)
    */
   bool getFrameData (long nFrameIdx, KUIImageFrame *lpFrame) const;

   /**
    * Set graphic for a frame
    *
    * \param nFrameIdx index of frame to set graphic for
    * \param lpGraphic graphic to use as source for this image
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    * \param lpHitMask hit mask for hit testing, NULL for none
    * \param nHitMaskWidth hit mask's width in pixels, -1 to take the graphic's width (default)
    * \param nHitMaskHeight hit mask's height in pixels, -1 to take the graphic's width (default)
    */
   void setFrameGraphic (long nFrameIdx, KGraphic *lpGraphic, float x1, float y1, float x2, float y2, const unsigned char *lpHitMask = NULL, long nHitMaskWidth = -1, long nHitMaskHeight = -1);

   /**
    * Append graphic to this image and increment frame count
    *
    * \param lpGraphic graphic to use as source for this image
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    * \param lpHitMask hit mask for hit testing, NULL for none
    * \param nHitMaskWidth hit mask's width in pixels, -1 to take the graphic's width (default)
    * \param nHitMaskHeight hit mask's height in pixels, -1 to take the graphic's width (default)
    *
    * \return index of frame that was appended
    */
   long appendFrameGraphic (KGraphic *lpGraphic, float x1, float y1, float x2, float y2, const unsigned char *lpHitMask = NULL, long nHitMaskWidth = -1, long nHitMaskHeight = -1);

   /**
    * Set destination coordinates offset for a frame
    *
    * \param nFrameIdx index of frame to set offset for
    * \param vDstOffset offset
    */
   void setFrameOffset (long nFrameIdx, KVector2 vDstOffset);

   /**
    * Enable or disable tiling the graphic to fill the size of the element, for a given frame.  Calling this method disables tiling, setting the source texture coordinates,
    * source coordinates scaling and rotation, and destination coordinates
    *
    * \param nFrameIdx index of frame to set stretching for
    * \param bEnable true to enable tiling, false to disable
    * \param bReverse true to align tiling with the bottom right, false to align with the top left
    */
   void setFrameTiling (long nFrameIdx, bool bEnable, bool bReverse = false);

   /**
    * Enable or disable stretching the graphic to the size of the element, for a given frame. Stretching defines a range
    * over X and over Y that stretches, so that corners can remain untouched and only the middle stretches (or the opposite) to fill the size of
    * the element. Calling this method disables tiling, flipping, setting the source texture coordinates, source coordinates scaling and rotation, and destination coordinates
    *
    * \param nFrameIdx index of frame to set stretching for
    * \param bXEnable true to enable X stretching, false to disable
    * \param fXMin leftmost X coordinate in graphic of part that stretches
    * \param fXMax rightmost X coordinate in graphic of part that stretches, +1
    * \param bYEnable true to enable Y stretching, false to disable
    * \param fYMin topmost Y coordinate in graphic of part that stretches
    * \param fYMax bottommmost Y coordinate in graphic of part that stretches, +1
    * \param bStretchSides true to stretch the sides, false to stretch the middle (default)
    */
   void setFrameStretching (long nFrameIdx, bool bXEnable, float fXMin, float fXMax, bool bYEnable, float fYMin, float fYMax, bool bStretchSides = false);

   /**
    * Enable or disable fully opaque sub-rectangle for a given frame. When drawing with the Z-buffer is enabled, this rectangle is drawn front to back to avoid overdraw
    * and save on fill rate
    *
    * \param nFrameIdx index of frame to set stretching for
    * \param bEnable true to enable, false to disable
    * \param x1 leftmost X coordinate in graphic of opaque rectangle
    * \param y1 topmost Y coordinate in graphic of opaque rectangle
    * \param x2 rightmost X coordinate in graphic of opaque rectangle, +1
    * \param y2 bottommmost Y coordinate in graphic of opaque rectangle, +1
    */
   void setFrameOpaqueRect (long nFrameIdx, bool bEnable, float x1, float y1, float x2, float y2);

   /**
    * Enable or disable mirroring, for a given frame
    *
    * \param nFrameIdx index of frame to set mirroring for
    * \param bXFlip true to mirror over the X axis when drawing, false not to (default)
    * \param bYFlip true to mirror over the Y axis when drawing, false not to (default)
    */
   void setFrameFlipping (long nFrameIdx, bool bXFlip, bool bYFlip);

   /**
    * Set coordinates to use for each point of the quad, in the graphic to be drawn, for a given frame. By default, coordinates are
    * (x1,y1), (x2,y1), (x2,y2) and (x1,y2) as supplied by setFrameGraphic(). Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set coordinates for
    * \param sx1 source X coordinate of first point, in clockwise order, in the graphic
    * \param sy1 source Y coordinate of first point, in clockwise order, in the graphic
    * \param sx2 source X coordinate of second point, in clockwise order, in the graphic
    * \param sy2 source Y coordinate of second point, in clockwise order, in the graphic
    * \param sx3 source X coordinate of third point, in clockwise order, in the graphic
    * \param sy3 source Y coordinate of third point, in clockwise order, in the graphic
    * \param sx4 source X coordinate of fourth point, in clockwise order, in the graphic
    * \param sy4 source Y coordinate of fourth point, in clockwise order, in the graphic
    */
   void setFrameSrcCoord (long nFrameIdx, float sx1, float sy1, float sx2, float sy2, float sx3, float sy3, float sx4, float sy4);

   /**
    * Set anchor point for defining the pivot of the source (graphic) coordinates (defaults to the center of the portion of the source
    * graphic to be drawn). Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set anchor for
    * \param sx X coordinate of anchor point in the graphic to be drawn
    * \param sy Y coordinate of anchor point in the graphic to be drawn
    */
   void setFrameSrcAnchor (long nFrameIdx, float sx, float sy);

   /**
    * Set anchor point for defining the pivot of the source (graphic) coordinates (defaults to the center of the portion of the source
    * graphic to be drawn), in normalized coordinates. Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set anchor for
    * \param fNormalizedX X coordinate of anchor point in the graphic to be drawn, in normalized space (divided by the source graphic's width; for instance 0.5f=center, 1=width)
    * \param fNormalizedY Y coordinate of anchor point in the graphic to be drawn, in normalized space (divided by the source graphic's height; for instance 0.5f=center, 1=height)
    */
   void setNormalizedFrameSrcAnchor (long nFrameIdx, float fNormalizedX, float fNormalizedY);

   /**
    * Set center (pivot) point for scaling and Z rotation of the source (graphic) coordinates (defaults to 0, 0). Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set pivot for
    * \param sx X coordinate of pivot, relative to anchor point
    * \param sy Y coordinate of pivot, relative to anchor point
    */
   void setFrameSrcCenter (long nFrameIdx, float sx, float sy);

   /**
    * Set center (pivot) point for scaling and Z rotation of the source (graphic) coordinates (defaults to 0, 0). Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set pivot for
    * \param fNormalizedX X coordinate of pivot, relative to anchor point, in normalized space (divided by the source graphic's width; for instance if anchor is unchanged, 0=center, -0.5f=-width/2)
    * \param fNormalizedY Y coordinate of pivot, relative to anchor point, in normalized space (divided by the source graphic's width; for instance if anchor is unchanged, 0=center, -0.5f=-height/2)
    */
   void setNormalizedFrameSrcCenter (long nFrameIdx, float fNormalizedX, float fNormalizedY);

   /**
    * Set horizontal and vertical scaling factors to be applied to source (graphic) coordinates, for a given frame. Scaling is performed using the pivot
    * point as the center. Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set scaling for
    * \param fScaleX new horizontal scaling factor
    * \param fScaleY new vertical scaling factor
    */
   void setFrameSrcScaleXY (long nFrameIdx, float fScaleX, float fScaleY);

   /**
    * Set Z rotation angle to be applied to source (graphic) coordinates, for a given frame. Z axis rotation is performed using the pivot
    * point as the center. Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set Z rotation for
    * \param fZAngle new Z rotation angle, in degrees
    */
   void setFrameSrcZAngle (long nFrameIdx, float fZAngle);

   /**
    * Set coordinates to use for each point of the quad, on screen, for a given frame, relative to the position of the UI element. By
    * default, coordinates are (0,0),(w,0),(w,h),(0,h), where w = x2-x1 and h=y2-y1 as supplied by setFrameGraphic(), or, if stretching
    * was enabled, w=element width and h=element height. Calling this method disables stretching
    *
    * \param nFrameIdx index of frame to set coordinates for
    * \param dx1 screen X coordinate of first point, in clockwise order, relative to the position of the element
    * \param dy1 screen Y coordinate of first point, in clockwise order, relative to the position of the element
    * \param dx2 screen X coordinate of second point, in clockwise order, relative to the position of the element
    * \param dy2 screen Y coordinate of second point, in clockwise order, relative to the position of the element
    * \param dx3 screen X coordinate of third point, in clockwise order, relative to the position of the element
    * \param dy3 screen Y coordinate of third point, in clockwise order, relative to the position of the element
    * \param dx4 screen X coordinate of fourth point, in clockwise order, relative to the position of the element
    * \param dy4 screen Y coordinate of fourth point, in clockwise order, relative to the position of the element
    */
   void setFrameDstQuad (long nFrameIdx, float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4);

   /**
    * Set mask graphic for a frame. The alpha channel will be taken from this graphic instead of the image's graphic, and combined with the image.
    * Setting the mask cancels tiling, stretching, source and destination arbitrary quads, and the opaque rectangle
    *
    * \param nFrameIdx index of frame to set graphic for
    * \param lpGraphic graphic to use as source for this image
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    * \param lpHitMask hit mask for hit testing, NULL for none
    * \param nHitMaskWidth hit mask's width in pixels, -1 to take the graphic's width (default)
    * \param nHitMaskHeight hit mask's height in pixels, -1 to take the graphic's width (default)
    */
   void setFrameMaskGraphic (long nFrameIdx, KGraphic *lpMaskGraphic, float x1, float y1, float x2, float y2);

   /**
    * Set root primitive of the bounding hierarchy for a given frame of this element. By default, a bounding rectangle of (0, 0, width, height) is set.
    * The bounding hierarchy being set will also automatically be deleted when the element is deleted
    *
    * \param nFrameIdx index of frame to set bounding hierarchy for
    * \param lpBounds root bounding primitive, NULL to remove hierarchy and use the default bounding rectangle
    * \param bDoNotFree true to avoid freeing previous bounding hierarchy if any; false to free previous hierarchy (default)
    */
   virtual void setFrameBounds (long nFrameIdx, KUIBounds *lpBounds, bool bDoNotFree = false);

   /**
    * Set level of subdivisions for all frames, when drawing arbitrarily defined quads
    *
    * \param nSubdivisions subdivision level (defaults to 0). When nSubdivisions isn't zero, 4^(nSubdivisions - 1) quads will be drawn
    */
   void setQuadSubdivisions (long nSubdivisions);

   /**
    * Add new frame sequence (visual) to this image. The visual is added with defaults
    *
    * \param lpszVisualName name of frame sequence (visual) to be added
    */
   void addVisual (const char *lpszVisualName);

   /**
    * Remove specified frame sequence (visual) from this image
    *
    * \param lpszVisualName name of frame sequence (visual) to be removed; NULL for current visual
    */
   void removeVisual (const char *lpszVisualName);

   /**
    * Get the currently displayed and active frame sequence (visual)
    *
    * \return active frame sequence(visual) (NULL for none)
    */
   const char *getCurrentVisual (void);

   /**
    * Set the currently displayed and active frame sequence (visual)
    *
    * \param lpszVisualName name of newly active frame sequence (visual); the visual will be added if it doesn't already exist
    */
   void setCurrentVisual (const char *lpszVisualName);

   /**
    * Get frame sequence (visual) data
    *
    * \param lpszVisualName name of frame sequence (visual) to get data for; NULL for current visual
    * \param lpVisual visual that receives the data
    *
    * \return true if successful, false if not (visual not found)
    */
   bool getVisualData (const char *lpszVisualName, KUIImageVisual *lpVisual);

   /**
    * Set details of a frame sequence (visual)
    *
    * \param lpszVisualName name of frame sequence to set; the visual will be added if it doesn't already exist; NULL for current visual
    * \param nStartFrameIdx first frame number in sequence
    * \param nEndFrameIdx last frame number in sequence
    * \param fFrameDuration number of milliseconds that each frame will be shown for, when animating
    * \param bGoForward true to go forward in frames, false to go backwards, when animating
    * \param bLoops true if sequence loops, when animating
    * \param bPingPongs if looping is enabled, true if sequence goes backwards after going forwards, or the other way around, after reaching the end, false if the sequence restarts at
    *                   the opposite frame (for instance nStartFrameIdx after reaching nEndFrameIdx)
    */
   void setVisualSequence (const char *lpszVisualName, long nStartFrameIdx, long nEndFrameIdx, double fFrameDuration, bool bGoForward, bool bLoops, bool bPingPongs);

   /**
    * Get frame currently displayed by a visual
    *
    * \param lpszVisualName name of visual to get current frame of; NULL for current visual
    *
    * \return frame number currently displayed by the specified frame sequence
    */
   long getVisualCurrentFrame (const char *lpszVisualName);

   /**
    * Set frame currently displayed by a visual
    *
    * \param lpszVisualName name of visual to set current frame of; the visual will be added if it doesn't already exist; NULL for current visual
    * \param nCurFrameIdx frame number that the sequence will display
    * \param nCrossBlendFrameIdx index of frame to cross-blend to
    * \param fCrossBlendFactor current factor for cross blending frames
    */
   void setVisualCurrentFrame (const char *lpszVisualName, long nCurFrameIdx, long nCrossBlendFrameIdx = -1, float fCrossBlendFactor = 0);

   /**
    * Get index of frame that triggers an event in a specific visual when it is reached while animating
    *
    * \param lpszVisualName name of visual to get trigger frame of; NULL for current visual
    *
    * \return frame number that triggers an K_UIIMAGE_MSGFRAMEREACHED event and an onFrameReached() call, -1 if none is set
    */
   long getVisualTriggerFrame (const char *lpszVisualName);

   /**
    * Set index of frame that triggers an event in a specific visual when it is reached while animating
    *
    * \param lpszVisualName name of visual to set trigger frame of; the visual will be added if it doesn't already exist; NULL for current visual
    * \param nTriggerFrameIdx frame number that triggers a K_UIIMAGE_MSGFRAMEREACHED event and an onFrameReached() call
    */
   void setVisualTriggerFrame (const char *lpszVisualName, long nTriggerFrameIdx);

   /**
    * Start animating the specified visual
    *
    * \param lpszVisualName name of visual to start animating; NULL for current visual
    */
   void startVisualAnim (const char *lpszVisualName);

   /**
    * Stop animating the specified visual
    *
    * \param lpszVisualName name of visual to stop animating; NULL for current visual
    */
   void stopVisualAnim (const char *lpszVisualName);

   /**
    * Check if the specified visual is currently animating
    *
    * \param lpszVisualName name of visual to check; NULL for current visual
    *
    * \return true if currently animating, false if not (done animating or wasn't started)
    */
   bool isVisualAnimating (const char *lpszVisualName);

   /**
    * Check if the specified trigger frame has been reached by the specified visual since the last check, while animating
    *
    * \param lpszVisualName name of visual to check trigger frame of; NULL for current visual
    *
    * \return true if trigger frame was reached since last check, false if not
    */
   bool isVisualTriggerFrameReached (const char *lpszVisualName);

   /**
    * Check if specified coordinates hit a lit pixel in the hit mask of the currently shown frame, or not
    *
    * \param x X coordinate (expressed in the element's coordinate space)
    * \param y Y coordinate (expressed in the element's coordinate space)
    *
    * \return true if specified coordinates hit a lit pixel in the mask of the currently shown frame (or if there is no hit mask), false if they hit an unlit one
    */
   bool checkMask (float x, float y);

   /**
    * Handle hit test: check whether the supplied coordinates are actually over the element or not. The default
    * implementation always returns true, which makes the entire element's rectangle active. This method can be
    * derived to implement elements such as circular buttons
    *
    * \param x X coordinate (expressed in the element's coordinate space)
    * \param y Y coordinate (expressed in the element's coordinate space)
    *
    * \return true if coordinates are over the element, false if not
    */
   virtual bool onHit (float x, float y);
   
protected:
   /** Update logic for this element */
   virtual void move (void);

   /**
    * Handle that a visual has stopped animating
    *
    * \param nVisualIdx index of visual that stopped animating
    */
   virtual void onAnimEnded (long nVisualIdx);

   /**
    * Handle that a visual has reached a specified trigger frame
    *
    * \param nVisualIdx index of visual that reached the trigger frame
    */
   virtual void onFrameReached (long nVisualIdx);

   /**
    * Check if this element has opaque parts. If it does, and drawing with the depth buffer is enabled, and this element is fully blended in, blitOpaque() will be called to
    * draw the opaque parts front-to-back, and blitTransparent() will be called to draw the transparent parts back-to-front, instead of blit(). Otherwise, blit() will be
    * called (default)
    *
    * \return true if element has opaque parts to be seperately drawn front-to-back if possible, false if not
    */
   virtual bool hasOpaqueParts (void);

   /**
    * Draw this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blit (float fBlend, KMatrix *lpMatrix);

   /**
    * Draw opaque parts of this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blitOpaque (float fBlend, KMatrix *lpMatrix);

   /**
    * Draw transparent parts of this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blitTransparent (float fBlend, KMatrix *lpMatrix);

   /** true if tinting is enabled */
   bool _bColorEnabled;

   /** RGBA tint */
   float _fColor[4];

   /** Lightness factor, 0..1 (fully dark..normal color) */
   float _fLightness;

   /** Overbrightness factor, 0..1 (normal..fully overlit) */
   float _fOverBrightness;

   /** true if setting the alpha operation (mode) is enabled */
   bool _bAlphaOpEnabled;

   /** Source alpha operation for the color channels */
   eKanjiAlphaOp _nSrcAlphaOp;

   /** Destination alpha operation for the color channels */
   eKanjiAlphaOp _nDstAlphaOp;

   /** Source alpha operation for the alpha channel */
   eKanjiAlphaOp _nAlphaSrcAlphaOp;

   /** Destination alpha operation for the alpha channel */
   eKanjiAlphaOp _nAlphaDstAlphaOp;

   /** Arbitrary quads subdivision level */
   long _nQuadSubdivisions;

   /** Screen coordinates offset to apply to all the frames */
   KVector2 _vGlobalDstOffset;

   /** Total number of frames defined */
   long _nFrameCount;

   /** Frames displayed by this image */
   KUIImageFrame *_frame;

   /** Total number of frame sequences defined */
   long _nVisualCount;

   /** Index of currently displayed frame sequence */
   long _nCurVisual;

   /** Sequence of frames displayed by this image */
   KUIImageVisual *_visual;

private:
   /**
    * Set total number of frame sequences (visuals) displayed by this image
    *
    * \param nVisualCount number of visuals
    */
   void setVisualCount (long nVisualCount);

   /**
    * Initialize specified visual
    *
    * \param nVisualIdx index of visual to initialize
    */
   void initVisual (long nVisualIdx);

   /**
    * Get index of visual from its name
    *
    * \param lpszVisualName name of visual to get index for, NULL to get the index of the current visual
    *
    * \return index, -1 if not found
    */
   long getVisualIndex (const char *lpszVisualName);

   /**
    * Get index of visual from its name, and attempt to create it if it doesn't exist
    *
    * \param lpszVisualName name of visual to get index for NULL to get the index of the current visual
    *
    * \return index, -1  if not found and unable to create
    */
   long getOrCreateVisualIndex (const char *lpszVisualName);

   /**
    * Recursively subdivide and draw quads
    *
    * \param lpGraphic graphic to draw (if not batching)
    * \param lpBatch vertex buffer (if batching)
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param nCurSubdivision current subdivision level
    * \param nTargetSubdivisions number of subdivisions at which to draw the quads
    */
   void blitSubQuads (KGraphic *lpGraphic, KBatch *lpBatch,
                      float sx1, float sy1, float sx2, float sy2, float sx3, float sy3, float sx4, float sy4,
                      float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4, float fBlend,
                      long nCurSubdivision, long nTargetSubdivisions);

   /** Name of graphics to be dereferenced when this image is deleted, NULL for none */
   char *_lpszLoadedGraphicFilename;

   /** Number of loaded frames in a sequence, to be dereferenced when this image is deleted */
   long _nLoadedFrames;
};

#endif   /* _KANJI_UIIMAGE_H */
