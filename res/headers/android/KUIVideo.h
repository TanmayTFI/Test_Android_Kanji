/**
 * Kanji game engine - video UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UIVIDEO_H
#define  _KANJI_UIVIDEO_H

/* Include definitions */
#include "KUIElement.h"
#include "KGraphic.h"
#include "KVideoBase.h"

/** Messages sent by KUIVideo */
enum {
   K_UIVIDEO_MSGVIDEOENDED = K_UIMSG ('k','v','d'),   /**< Video done playing */
   K_UIVIDEO_MSGFRAMEREACHED,                         /**< Frame index reached in video */
   K_UIVIDEO_MSGTIMEREACHED,                          /**< Time index reached in video */
};

/* Forward declarations */
class KVideo;

/**
 * Video UI element
 */

class KUIVideo : public KUIElement {
public:
   /**
    * Check if the supplied element is an image, and cast as KUIVideo if it is
    *
    * \return KUIVideo if element is an image, NULL otherwise
    */
   static KUIVideo *isVideo (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIVideo (KUIElement *lpParentElem);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    * \param lpVideo video to use for this element
    * \param x1 source X coordinate of top,left of portion to display in video
    * \param y1 source Y coordinate of top,left of portion to display in video
    * \param x2 source X coordinate of bottom,right of portion to display in video + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in video + 1
    */
   KUIVideo (KUIElement *lpParentElem, KVideo *lpVideo, float x1, float y1, float x2, float y2) :
         KUIElement (lpParentElem),
         _bColorEnabled (false),
         _fLightness (1),
         _bAlphaOpEnabled (false),
         _nSrcAlphaOp (K_ALPHAOP_SRC_ALPHA),
         _nDstAlphaOp (K_ALPHAOP_ONE_MINUS_SRC_ALPHA),
         _nAlphaSrcAlphaOp (K_ALPHAOP_SRC_ALPHA),
         _nAlphaDstAlphaOp (K_ALPHAOP_ONE_MINUS_SRC_ALPHA),
         _lpVideo (NULL),
         _bXFlip (false),
         _bYFlip (false),
         _nTriggerFrameIdx (-1),
         _bTriggerFrameReached (false),
         _fTriggerTime (-1),
         _nPrevFrameIdx (0),
         _fPrevTime (0),
         _bTriggerTimeReached (false),
         _lpszLoadedVideoFilename (NULL) {
      extern void KVideo_register (void);

      initializeVideo ();
      KVideo_register ();   /* Make sure KVideo is linked in and available */
      setVideoInternal (lpVideo, x1, y1, x2, y2);
   }

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    * \param x X coordinate of top,left of element, relative to the parent
    * \param y Y coordinate of top,left of element, relative to the parent
    * \param lpszElemName image element's name
    * \param lpszVideoFilename filename to load video from, using KManagedVideo; an instance of KGame must have been created, and getVideo()->resumeTime() must be called to start playback
    * \param x1 source X coordinate of top,left of portion to display in video
    * \param y1 source Y coordinate of top,left of portion to display in video
    * \param x2 source X coordinate of bottom,right of portion to display in video + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in video + 1
    * \param bLoopVideo true to loop the video indefinitely, false to play it once and report the end using isEndReached()
    * \param bPlayAudio true to play audio stream (if any), false not to
    * \param bSeekable true to make video seekable into with seekToTime() and seekToFrame(), false if not needed
    */
   KUIVideo (KUIElement *lpParentElem, float x, float y, const char *lpszElemName, const char *lpszVideoFilename, float x1, float y1, float x2, float y2,
             bool bLoopVideo = true, bool bPlayAudio = true, bool bSeekable = false) :
         KUIElement (lpParentElem),
         _bColorEnabled (false),
         _fLightness (1),
         _bAlphaOpEnabled (false),
         _nSrcAlphaOp (K_ALPHAOP_SRC_ALPHA),
         _nDstAlphaOp (K_ALPHAOP_ONE_MINUS_SRC_ALPHA),
         _nAlphaSrcAlphaOp (K_ALPHAOP_SRC_ALPHA),
         _nAlphaDstAlphaOp (K_ALPHAOP_ONE_MINUS_SRC_ALPHA),
         _lpVideo (NULL),
         _bXFlip (false),
         _bYFlip (false),
         _nTriggerFrameIdx (-1),
         _bTriggerFrameReached (false),
         _fTriggerTime (-1),
         _nPrevFrameIdx (0),
         _fPrevTime (0),
         _bTriggerTimeReached (false),
         _lpszLoadedVideoFilename (NULL) {
      extern void KVideo_register (void);

      initializeVideo ();
      KVideo_register ();   /* Make sure KVideo is linked in and available */
      loadVideoInternal (lpszVideoFilename, x1, y1, x2, y2, bLoopVideo, bPlayAudio, bSeekable);

      /* Set properties */
      if (lpszElemName && lpszElemName[0]) setName (lpszElemName);
      setPosition (x, y);
   }

   /** Destructor */
   virtual ~KUIVideo ();

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
    * Get lightness factor (multiplied to each of the rgb components, used for darkening this video)
    *
    * \return lightness factor, 0..1 (fully dark..normal color)
    */
   float getLightness (void);

   /**
    * Set lightness factor (multiplied to each of the rgb components, used for darkening this video)
    *
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setLightness (float fLightness);

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
   eKanjiAlphaOp getSrcAlphaOp (void);
   
   /**
    * Get destination alpha blending operation for the color channels of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getDstAlphaOp (void);
   
   /**
    * Get source alpha blending operation for the alpha channel of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getAlphaSrcAlphaOp (void);
   
   /**
    * Get destination alpha blending operation for the alpha channel of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getAlphaDstAlphaOp (void);

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
    * Get video displayed by this instance, if any
    *
    * \return video, NULL if none is set 
    */
   KVideo *getVideo (void);

   /**
    * Get source coordinates of the portion of the video drawn by this instance
    *
    * \param lpRect rectangle that receives the coordinates (if a video is set, otherwise 0,0,0,0 is returned)
    */
   void getVideoSourceRect (KRectangle *lpRect);

   /**
    * Set video to display
    *
    * \param lpVideo video to use for this element
    * \param x1 source X coordinate of top,left of portion to display in video
    * \param y1 source Y coordinate of top,left of portion to display in video
    * \param x2 source X coordinate of bottom,right of portion to display in video + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in video + 1
    */
   void setVideo (KVideo *lpVideo, float x1, float y1, float x2, float y2) {
      extern void KVideo_register (void);

      KVideo_register ();   /* Make sure KVideo is linked in and available */
      setVideoInternal (lpVideo, x1, y1, x2, y2);
   }

   /**
    * Check if this video is drawn mirrored on the X axis
    *
    * \return true if mirrored over the X axis, false if not
    */
   bool isXFlipped (void);

   /**
    * Check if this video is drawn mirrored on the Y axis
    *
    * \return true if mirrored over the Y axis, false if not
    */
   bool isYFlipped (void);

   /**
    * Enable or disable mirroring
    *
    * \param bXFlip true to mirror over the X axis when drawing, false not to (default)
    * \param bYFlip true to mirror over the Y axis when drawing, false not to (default)
    */
   void setFlipping (bool bXFlip, bool bYFlip);

   /**
    * Get index of frame that triggers an event in this video
    *
    * \return frame number that triggers an K_UIVIDEO_MSGFRAMEREACHED event and an onFrameReached() call, -1 if none is set
    */
   unsigned long getTriggerFrame (void);

   /**
    * Set index of frame that triggers an event in this video
    *
    * \param nTriggerFrameIdx frame number that triggers a K_UIVIDEO_MSGFRAMEREACHED event and an onFrameReached() call
    */
   void setTriggerFrame (unsigned long nTriggerFrameIdx);

   /**
    * Check if the specified trigger frame has been reached since the last check
    *
    * \return true if trigger frame was reached since last check, false if not
    */
   bool isTriggerFrameReached (void);

   /**
    * Get index of frame that triggers an event in this video
    *
    * \return time in seconds that triggers an K_UIVIDEO_MSGFRAMEREACHED event and an onTimeReached() call, -1 if none is set
    */
   double getTriggerTime (void);

   /**
    * Set time that triggers an event in this video
    *
    * \param fTriggerTime time in seconds that triggers a K_UIVIDEO_MSGTIMEREACHED event and an onTimeReached() call
    */
   void setTriggerTime (double fTriggerTime);

   /**
    * Check if the specified trigger time has been reached since the last check
    *
    * \return true if trigger time was reached since last check, false if not
    */
   bool isTriggerTimeReached (void);

protected:
   /** Update logic for this element */
   virtual void move (void);

   /**
    * Handle that this video is done playing
    */
   virtual void onVideoEnded (void);

   /**
    * Handle that this video has reached a specified trigger frame index
    */
   virtual void onFrameReached (void);

   /**
    * Handle that this video has reached a specified trigger time index
    */
   virtual void onTimeReached (void);

   /**
    * Draw this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blit (float fBlend, KMatrix *lpMatrix);

private:
   /** Initialize this instance */
   void initializeVideo ();

   /**
    * Set video to display
    *
    * \param lpVideo video to use for this element
    * \param x1 source X coordinate of top,left of portion to display in video
    * \param y1 source Y coordinate of top,left of portion to display in video
    * \param x2 source X coordinate of bottom,right of portion to display in video + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in video + 1
    */
   void setVideoInternal (KVideo *lpVideo, float x1, float y1, float x2, float y2);

   /**
    * Load managed video to display
    *
    * \param lpzVideoFilename filename of video to be loaded, using KManagedVideo
    * \param x1 source X coordinate of top,left of portion to display in video
    * \param y1 source Y coordinate of top,left of portion to display in video
    * \param x2 source X coordinate of bottom,right of portion to display in video + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in video + 1
    * \param bLoopVideo true to loop the video indefinitely, false to play it once and report the end using isEndReached()
    * \param bPlayAudio true to play audio stream (if any), false not to
    * \param bSeekable true to make video seekable into with seekToTime() and seekToFrame(), false if not needed
    */
   void loadVideoInternal (const char *lpzVideoFilename, float x1, float y1, float x2, float y2, bool bLoopVideo, bool bPlayAudio, bool bSeekable);

   /** true if tinting is enabled */
   bool _bColorEnabled;

   /** RGBA tint */
   float _fColor[4];

   /** Lightness factor, 0..1 (fully dark..normal color) */
   float _fLightness;

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

   /** Video */
   KVideoBase *_lpVideo;

   /** Source coordinates in video */
   KRectangle _rcSrcCoord;

   /** true to mirror over the X axis when drawing */
   bool _bXFlip;

   /** true to mirror over the Y axis when drawing */
   bool _bYFlip;

   /** Index of frame that triggers a K_UIVIDEO_MSGFRAMEREACHED event, -1 for none */
   unsigned long _nTriggerFrameIdx;

   /** true if trigger frame was reached since last polling */
   bool _bTriggerFrameReached;

   /** Time in seconds that triggers a K_UIVIDEO_MSGTIMEREACHED event, -1 for none */
   double _fTriggerTime;

   /** true if trigger time was reached since last polling */
   bool _bTriggerTimeReached;

   /** Last seen frame index while playing */
   unsigned long _nPrevFrameIdx;
   
   /** Last seen time index while playing */
   double _fPrevTime;

   /** Name of video file to be dereferenced when this video is deleted, NULL for none */
   char *_lpszLoadedVideoFilename;
};

#endif   /* _KANJI_UIVIDEO_H */
