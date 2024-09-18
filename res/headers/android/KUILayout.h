/**
 * Kanji game engine - auto-layout element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UILAYOUT_H
#define  _KANJI_UILAYOUT_H

/* Include definitions */
#include "KUIElement.h"

/* Layout type */

typedef enum {
   K_UILAYOUT_FIXED = 0,         /**< Children set their own position - default */
   K_UILAYOUT_HORIZONTAL,        /**< Align children horizontally */
   K_UILAYOUT_VERTICAL,          /**< Align children vertically */
   K_UILAYOUT_GRID,              /**< Align children in a grid */
} KUILayoutType;

/**
 * Auto-layout UI element
 */

class KUILayout : public KUIElement {
public:
   /**
    * Check if the supplied element is a layout, and cast as KUILayout if it is
    *
    * \return KUILayout if element is a layout, NULL otherwise
    */
   static KUILayout *isLayout (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUILayout (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUILayout ();

   /**
    * Copy all parameters except for the name, identifier, user data, state, layer number, visibility, input and message delivery status, and message callback, from another element
    *
    * \param lpFromElement element to copy from
    */
   virtual void copyFromTemplate (KUIElement *lpFromElement);

   /**
    * Set size
    *
    * \param fWidth width in pixels
    * \param fHeight height in pixels
    */
   virtual void setSize (float fWidth, float fHeight);

   /**
    * Get layout type
    *
    * \return type (K_UILAYOUT_xxx)
    */
   KUILayoutType getLayoutType (void);

   /**
    * Set layout type
    *
    * \param nType new type (K_UILAYOUT_xxx)
    */
   void setLayoutType (KUILayoutType nType);

   /**
    * Set vertical alignment mode, for K_UILAYOUT_VERTICAL
    *
    * \param nMode new mode
    */
   void setHorizAlignMode (KUIHorizAlignMode nMode);

   /**
    * Set vertical alignment mode, for K_UILAYOUT_HORIZONTAL
    *
    * \param nMode new mode
    */
   void setVertAlignMode (KUIVertAlignMode nMode);

   /**
    * Get horizontal spacing between children, in pixels, for K_UILAYOUT_HORIZONTAL and K_UILAYOUT_GRID
    *
    * \return horizontal spacing
    */
   float getHSpacing (void);

   /**
    * Set horizontal spacing between children, in pixels, for K_UILAYOUT_HORIZONTAL and K_UILAYOUT_GRID.
    * If not set, it is automatically calculated
    *
    * \param fHSpacing new horizontal spacing, -1 to automatically calculate
    */
   void setHSpacing (float fHSpacing);

   /**
    * Check if this element is laying children out horizontally at a fixed interval set by setHSpacing()
    *
    * \return true if using a fixed horizontal interval, false if using horizontal spacing between children
    */
   bool isFixedHInterval (void);

   /**
    * Set whether this element is laying children out horizontally at a fixed interval
    *
    * \param bEnable true to use a fixed interval set by setHSpacing(), false to use horizontal spacing between children
    */
   void setFixedHInterval (bool bEnable);

   /**
    * Check if this element is laying children out from right to left, for K_UILAYOUT_HORIZONTAL and K_UILAYOUT_GRID
    *
    * \return true if performing layout from right to left, false if performing from left to right
    */
   bool isRightToLeft (void);

   /**
    * Enable or disable laying children out from right to left, for K_UILAYOUT_HORIZONTAL and K_UILAYOUT_GRID
    *
    * \param bRightToLeft true to perform layout from right to left, false to perform from left to right
    */
   void setRightToLeft (bool bRightToLeft);

   /**
    * Get vertical spacing between children, in pixels, for K_UILAYOUT_VERTICAL and K_UILAYOUT_GRID
    *
    * \return horizontal spacing
    */
   float getVSpacing (void);

   /**
    * Set vertical spacing between children, in pixels, for K_UILAYOUT_VERTICAL and K_UILAYOUT_GRID.
    * If not set, it is automatically calculated
    *
    * \param fVSpacing new vertical spacing, -1 to automatically calculate
    */
   void setVSpacing (float fVSpacing);

   /**
    * Check if this element is laying children out vertically at a fixed interval set by setVSpacing()
    *
    * \return true if using a fixed vertical interval, false if using vertical spacing between children
    */
   bool isFixedVInterval (void);

   /**
    * Set whether this element is laying children out vertically at a fixed interval
    *
    * \param bEnable true to use a fixed interval set by setVSpacing(), false to use vertical spacing between children
    */
   void setFixedVInterval (bool bEnable);

   /**
    * Check if this element is laying children out from bottom to top, for K_UILAYOUT_VERTICAL and K_UILAYOUT_GRID
    *
    * \return true if performing layout from bottom to top, false if performing from top to bottom
    */
   bool isBottomToTop (void);

   /**
    * Enable or disable laying children out from bottom to top, for K_UILAYOUT_HORIZONTAL and K_UILAYOUT_GRID
    *
    * \param bBottomToTop true to perform layout from bottom to top, false to perform from top to bottom
    */
   void setBottomToTop (bool bBottomToTop);

   /**
    * Get number of columns, for K_UILAYOUT_GRID
    *
    * \return number of columns
    */
   long getColumns (void);

   /**
    * Set number of columns, for K_UILAYOUT_GRID
    *
    * \param nColumns new number of columns
    */
   void setColumns (long nColumns);

protected:
   /**
    * Handle a child having been added, removed or resized
    */
   virtual void handleChildUpdate (void);

   /** Re-position all children */
   virtual void updateChildrenLayout (void);

   /** Layout type */
   KUILayoutType _nLayoutType;

   /** Horizontal alignment mode, for K_UILAYOUT_VERTICAL */
   KUIHorizAlignMode _nHorizAlignMode;

   /** Vertical alignment mode, for K_UILAYOUT_HORIZONTAL */
   KUIVertAlignMode _nVertAlignMode;

   /** true to automatically calculate horizontal spacing */
   bool _bAutoHSpacing;

   /** true to use a fixed horizontal interval, false to use horizontal spacing between elements */
   bool _bFixedHInterval;

   /** Current horizontal spacing between children */
   float _fHSpacing;

   /** true if performing layout from right to left, false if performing from left to right */
   bool _bRightToLeft;

   /** true to automatically calculate vertical spacing */
   bool _bAutoVSpacing;

   /** true to use a fixed vertical interval, false to use vertical spacing between elements */
   bool _bFixedVInterval;

   /** Current vertical spacing between children */
   float _fVSpacing;

   /** true if performing layout from bottom to top, false if performing from top to bottom */
   bool _bBottomToTop;

   /** Specified number of columns for grid layouts, -1 for auto */
   long _nCols;
};

#endif   /* _KANJI_UILAYOUT_H */
