/**
 * Kanji game engine - selector UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UISELECTOR_H
#define  _KANJI_UISELECTOR_H

/* Include definitions */
#include "KUIElement.h"
#include "KUIButton.h"
#include "KUILayout.h"

/** Messages sent by KUISelector */
enum {
   K_UISELECTOR_MSGSELECTIONCHANGED = K_UIMSG ('k','s','e'),      /**< The selected element has changed */
};

/**
 * Element that lets the user select one child button element among multiple ones, offering a multiple choice such as fullscreen/windowed
 */

class KUISelector : public KUILayout {
public:
   /**
    * Check if the supplied element is a selector, and cast as KUISelector if it is
    *
    * \return KUISelector if element is a selector, NULL otherwise
    */
   static KUISelector *isSlider (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUISelector (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUISelector ();

   /**
    * Check if the currently selected button has changed since the last time this method was called
    *
    * \return true if changed, false if not
    */
   bool hasSelectionChanged (void);
   
   /**
    * Get currently selected button element
    *
    * \return currently selected button element, NULL for none
    */
   KUIElement *getSelectedElement (void);

   /**
    * Set currently selected button element
    *
    * \param lpElement newly selected button element
    */
   void setSelectedElement (KUIElement *lpElement);

protected:
   /**
    * Handle that the selected element has changed
    */
   virtual void onSelectionChanged (void);

   /**
    * Handle a child having been added, removed or resized
    */
   virtual void handleChildUpdate (void);

   /** Update logic for this element */
   virtual void move (void);

private:
   /** true if selected element changed */
   bool _bSelectionChanged;

   /** Currently selected element */
   KUIElement *_lpSelectedElem;
};

#endif   /* _KANJI_UISELECTOR_H */
