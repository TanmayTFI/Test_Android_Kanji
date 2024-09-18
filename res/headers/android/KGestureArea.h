/**
 * Kanji rendering and I/O engine - gesture definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KGESTURE_H
#define  _KANJI_KGESTURE_H

/* Include definitions */
#include "KObjectListable.h"
#include "KWindow.h"
#include "KList.h"

/* Forward definitions */
class KWindowGL;
class KWindowD3D;
class KWindowD3D9;
class KWindowD3D11;
class KWindowGLES;
class KWindowGLES2;

/** Gesture handling */

class KGestureArea : public KObjectListable {
   friend class KWindow;
   friend class KWindowGL;
   friend class KWindowD3D;
   friend class KWindowD3D9;
   friend class KWindowD3D11;
   friend class KWindowGLES;
   friend class KWindowGLES2;

public:
   /**
    * Constructor
    *
    * \param fDocumentWidth width of entire document to be scrolled and scaled, in pixels
    * \param fDocumentHeight height of entire document to be scrolled and scaled, in pixels
    * \param rcViewport coordinates of top, left and bottom, right (+1) of view window, in pixels, in screen coordinates
    * \param fMinScale minimum scale (the user can go beyond it while zooming, it will snap back to the minimum value)
    * \param fMinScaleLimit hard minimum scale (the user cannot zoom below this scale)
    * \param fMaxScale maximum scale (the user can go beyond it while zooming, it will snap back to the minimum value)
    * \param fMaxScaleLimit hard maximum scale (the user cannot zoom above this scale)
    */
   KGestureArea (float fDocumentWidth, float fDocumentHeight, KRectangle rcViewport, float fMinScale, float fMinScaleLimit, float fMaxScale, float fMaxScaleLimit);

   /** Destructor */
   ~KGestureArea ();

   /**
    * Change document size
    *
    * \param fDocumentWidth width of entire document to be scrolled and scaled, in pixels
    * \param fDocumentHeight height of entire document to be scrolled and scaled, in pixels
    */
   void setDocumentSize (float fDocumentWidth, float fDocumentHeight);

   /**
    * Change viewport dimensions
    *
    * \param rcViewport coordinates of top, left and bottom, right (+1) of view window, in pixels, in screen coordinates
    */
   void setViewport (KRectangle rcViewport);

   /**
    * Change document limits
    *
    * \param fMinScale minimum scale (the user can go beyond it while zooming, it will snap back to the minimum value)
    * \param fMinScaleLimit hard minimum scale (the user cannot zoom below this scale)
    * \param fMaxScale maximum scale (the user can go beyond it while zooming, it will snap back to the minimum value)
    * \param fMaxScaleLimit hard maximum scale (the user cannot zoom above this scale)
    */
   void setLimits (float fMinScale, float fMinScaleLimit, float fMaxScale, float fMaxScaleLimit);

   /**
    * Enable or disable processing gesture events over this area
    *
    * \param bEnable true to enable (default), false to disable
    */
   void enableArea (bool bEnable);

   /**
    * Enable or disable temporarily swiping or pinch zooming outside the boundaries of the document, then doing an elastic snap back into place when released
    *
    * \param bAllow true to allow (default), false to disallow (the transformation matrix will not provide a view that shows outside the document)
    */
   void allowPanningOutOfBounds (bool bAllow);

   /**
    * Get current camera offset
    *
    * \return current camera offset (0,0=top,left of scrollable area)
    */
   KVector2 getCamera (void);

   /**
    * Set current camera offset
    *
    * \param vCamera new camera offset (0,0=top,left of scrollable area)
    */
   void setCamera (KVector2 vCamera);

   /**
    * Get current scale
    *
    * \return current scale
    */
   float getScale (void);

   /**
    * Set current scale
    *
    * \return current scale
    */
   void setScale (float fScale);

   /** Immediately snap to a camera position and scale within bounds */
   void snapToPosition (void);

   /**
    * Get transformation matrix for current camera position and scaling
    *
    * \return transformation matrix
    */
   KMatrix getMatrix (void);

   /**
    * Check if a gesture is currently in progress over this area
    *
    * \return true if a gesture is in progress, false if not
    */
   bool isGestureInProgress (void);

   /**
    * Get amount of movement in pixels that triggers a swipe gesture in all areas
    *
    * \return amount in pixels
    */
   static long getSwipeThreshold (void);
   
   /**
    * Set amount of movement in pixels that triggers a swipe gesture in all areas
    *
    * \param nThreshold new amount in pixels
    */
   static void setSwipeThreshold (long nThreshold);
   
   /**
    * Get amount of movement in pixels that triggers a fling gesture
    *
    * \return amount in pixels
    */
   static long getFlingThreshold (void);
   
   /**
    * Set amount of movement in pixels that triggers a fling gesture
    *
    * \param nThreshold new amount in pixels
    */
   static void setFlingThreshold (long nThreshold);

private:
   /**
    * Have this area handle a gesture event
    *
    * \param lpKEvent event to handle
    */
   void processAreaEvent (KEvent *lpKEvent);

   /**
    * Update logic of this area
    *
    * \param nElapsedMillis milliseconds elapsed since last update
    */
   void updateArea (long nElapsedMillis);

   /**
    * Initialize gesture handling
    *
    * \param lpKWindow game window
    */
   static void setup (KWindow *lpKWindow);

   /**
    * Process mouse or touch event and dispatch gesture events to the game and to all areas accordingly
    *
    * \param lpKEvent mouse or touch event
    * \param lpPortableHandler game event handler to be called
    * \param bCancelGesture true if gesture is being cancelled, false if not
    */
   static void generateGestureEvents (KEvent *lpKEvent, ptkNativeCallBak lpPortableHandler, bool bCancelGesture);

   /**
    * Update gesture logic and all areas, if needed
    *
    * \param lpPortableHandler game event handler to be called
    */
   static void updateAllAreas (ptkNativeCallBak lpPortableHandler);

   /**
    * Dispatch a gesture event to all areas
    *
    * \param lpKEvent gesture event to be dispatched
    */
   static void dispatchEventToAllAreas (KEvent *lpKEvent);

   /** true if processing gesture events is currently enabled for this area, false if not */
   bool _bEnabled;

   /** true to enable temporarily swiping or pinch zooming outside the boundaries of the document, then doing an elastic snap back into place when released */
   bool _bAllowPanningOutOfBounds;

   /* Width of scrollable area */
   float _fDocumentWidth;

   /* Height of scrollable area */
   float _fDocumentHeight;
   
   /* Coordinates of top, left and bottom, right (+1) of view window, in pixels, in screen coordinates */
   KRectangle _rcViewport;

   /* Minimum scale (the user can go beyond it while zooming, it will snap back to the minimum value) */
   float _fMinScale;

   /* Hard minimum scale (the user cannot zoom below this scale) */
   float _fMinScaleLimit;
   
   /* Maximum scale (the user can go beyond it while zooming, it will snap back to the minimum value) */
   float _fMaxScale;

   /* Hard maximum scale (the user cannot zoom above this scale) */
   float _fMaxScaleLimit;

   /** Current camera offset (0,0=top,left of scrollable area) */
   KVector2 _vCamera;
   
   /** Current scale */
   float _fCameraScale;
   
   /** true if a swiping gesture is in progress */
   bool _bSwiping;
   
   /** true if a zooming gesture is in progress */
   bool _bZooming;

   /** Width of game window, in pixels */
   static long g_nWindowWidth;

   /** Height of game window, in pixels */
   static long g_nWindowHeight;

   /** Amount of movement in pixels that triggers a swipe gesture in all areas */
   static long g_nSwipeThreshold;
   
   /** Amount of movement in pixels that triggers a fling gesture in all areas */
   static long g_nFlingThreshold;
   
   /** List of all areas to handle */
   static KList<KGestureArea> g_lAreas;
};

#endif   /* _KANJI_KGESTURE_H */
