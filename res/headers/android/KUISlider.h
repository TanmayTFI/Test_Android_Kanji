/**
 * Kanji game engine - sliding button UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UISLIDER_H
#define  _KANJI_UISLIDER_H

/* Include definitions */
#include "KUIElement.h"
#include "KUIButton.h"

/* Slider type */

typedef enum {
   K_UISLIDER_HORIZONTAL = 0,    /**< Horizontal */
   K_UISLIDER_VERTICAL           /**< Vertical */
} KUISliderType;

/** Messages sent by KUISlider */
enum {
   K_UISLIDER_MSGVALUECHANGED = K_UIMSG ('k','s','l'),   /**< The slider's value has changed */
   K_UISLIDER_MSGFIRSTDOWN,                              /**< The slider has just been hit with a mousebutton or a touch */
};

/**
 * Sliding button UI element
 */

class KUISlider : public KUIElement {
public:
   /**
    * Check if the supplied element is a slider, and cast as KUISlider if it is
    *
    * \return KUISlider if element is a slider, NULL otherwise
    */
   static KUISlider *isSlider (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUISlider (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUISlider ();

   /**
    * Copy all parameters except for the name, identifier, user data, state, layer number, visibility, input and message delivery status, and message callback, from another element
    *
    * \param lpFromElement element to copy from
    */
   virtual void copyFromTemplate (KUIElement *lpFromElement);

   /**
    * Set layer number for this element
    *
    * \param nLayer layer number (defaults to the parent element's)
    */
   virtual void setLayer (long nLayer);

   /**
    * Set size
    *
    * \param fWidth width in pixels
    * \param fHeight height in pixels
    */
   virtual void setSize (float fWidth, float fHeight);

   /**
    * Set offset of top, left of background graphic from top, left of element
    *
    * \param nState state to set offset for
    * \param x X offset
    * \param y Y offset
    */
   void setBackgroundOffset (KUIElementState nState, float x, float y);

   /**
    * Set background graphic for one state of this slider
    *
    * \param nState state to set graphic for
    * \param lpGraphic source graphic for the background for this state
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    */
   void setBackgroundGraphic (KUIElementState nState, KGraphic *lpGraphic, float x1, float y1, float x2, float y2);

   /**
    * Set tinting color for the background graphic for a slider's state
    *
    * \param nState state to set tinting color for
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setBackgroundColor (KUIElementState nState, float r, float g, float b, float a);

   /**
    * Set lightness factor (multiplied to each of the rgb components), used for darkening the background graphic for a slider's state
    *
    * \param nState state to set lightness factor for
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setBackgroundLightness (KUIElementState nState, float fLightness);

   /**
    * Enable or disable stretching the background graphic to the size of the element. Stretching defines a range over X and
    * over Y that stretches, so that corners can remain untouched and only the middle stretches to fill the size of
    * the element
    *
    * \param nState state to set stretching for
    * \param bXEnable true to enable X stretching, false to disable
    * \param fXMin leftmost X coordinate in graphic of part that stretches
    * \param fXMax rightmost X coordinate in graphic of part that stretches, +1
    * \param bYEnable true to enable Y stretching, false to disable
    * \param fYMin topmost Y coordinate in graphic of part that stretches
    * \param fYMax bottommmost Y coordinate in graphic of part that stretches, +1
    */
   void setBackgroundStretching (KUIElementState nState, bool bXEnable, float fXMin, float fXMax, bool bYEnable, float fYMin, float fYMax);
   
   /**
    * Set slider type (horizontal, vertical)
    *
    * \param nSliderType new slider type
    */
   void setSliderType (KUISliderType nSliderType);

   /**
    * Enable or disable scrollbar mode: clicking before or after the slider button pages up and down rather than
    * placing the slider button under the cursor. If setButtonGraphicStretching() is called to enable stretching, the slider
    * button also stretches to represent the size of a page in the larger document
    *
    * \param fPageSize size of a page in the larger document (the whole document being 1.0f); 0.0f disables scrollbar mode
    */
   void setScrollbar (float fPageSize);

   /**
    * Set button graphic for a slider's state
    *
    * \param nState state to set graphic for
    * \param lpGraphic source graphic for the button for this state
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    */
   void setButtonGraphic (KUIElementState nState, KGraphic *lpGraphic, float x1, float y1, float x2, float y2);

   /**
    * Set tinting color for the button graphic for a slider's state
    *
    * \param nState state to set tinting color for
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setButtonColor (KUIElementState nState, float r, float g, float b, float a);

   /**
    * Set lightness factor (multiplied to each of the rgb components), used for darkening the button graphic for a slider's state
    *
    * \param nState state to set lightness factor for
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setButtonLightness (KUIElementState nState, float fLightness);

   /**
    * Enable or disable stretching the button graphic to the size of the element. Stretching defines a range over X and
    * over Y that stretches, so that corners can remain untouched and only the middle stretches to fill the size of
    * the element
    *
    * \param nState state to set stretching for
    * \param bXEnable true to enable X stretching, false to disable
    * \param fXMin leftmost X coordinate in button graphic of part that stretches
    * \param fXMax rightmost X coordinate in button graphic of part that stretches, +1
    * \param bYEnable true to enable Y stretching, false to disable
    * \param fYMin topmost Y coordinate in button graphic of part that stretches
    * \param fYMax bottommmost Y coordinate in button graphic of part that stretches, +1
    */
   void setButtonStretching (KUIElementState nState, bool bXEnable, float fXMin, float fXMax, bool bYEnable, float fYMin, float fYMax);

   /**
    * Set area of the element, in element's coordinates, where the button will slide (defaults to the whole element)
    *
    * \param bEnable true to enable, false to disable
    * \param x1 X coordinate of top,left of active rectangle, relative to the top,left of the element
    * \param y1 Y coordinate of top,left of active rectangle, relative to the top,left of the element
    * \param x2 X coordinate of bottom,right + 1 of active rectangle, relative to the top,left of the element
    * \param y2 Y coordinate of bottom,right + 1 of active rectangle, relative to the top,left of the element
    */
   void setActiveRect (bool bEnable, float x1, float y1, float x2, float y2);

   /**
    * Check if the slider's value has changed since last check
    *
    * \return true if value changed since last check, false if not
    */
   bool hasValueChanged (void);

   /**
    * Check if slider has been hit with a mousebutton or a touch since last check
    *
    * \return true if hit, false if not
    */
   bool isFirstDown (void);

   /**
    * Get current value of this slider
    *
    * \return current value, 0..1
    */
   float getValue (void);

   /**
    * Set new value of this slider
    *
    * \param fValue new value, 0..1
    */
   void setValue (float fValue);

   /**
    * Get this slider's button element
    *
    * \return slider's button element
    */
   KUIButton *getSliderButton (void);

   /**
    * Get the width of the sliding button
    *
    * \return width
    */
   float getButtonWidth (void);
   
   /**
    * Get the height of the sliding button
    *
    * \return height
    */
   float getButtonHeight (void);
   
   /**
    * Get the rectangle that is being drawn
    *
    * \return rectangle
    */
   KRectangle getRcDraw (void);
   
protected:
   /** 
    * Handle that this slider's value has changed
    */
   virtual void onValueChanged (void);

   /**
    * Handle that this slider has just been hit with a mousebutton or a touch
    */
   virtual void onFirstDown (void);

   /**
    * Handle input event
    *
    * \param lpKEvent event
    *
    * \return true if event was processed by this element, false if it wasn't
    */
   virtual bool handleEvent (KEvent *lpKEvent);

   /**
    * Handle the loss of the mouse capture, when this element was clicked, then the mouse button is finally released anywhere on screen
    */
   virtual void handleMouseCaptureLoss (void);

   /**
    * Set new state
    *
    * \param nState state
    */
   virtual void setState (KUIElementState nState);

   /** Update logic for this element */
   virtual void move (void);

   /**
    * Draw this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blit (float fBlend, KMatrix *lpMatrix);

private:
   /**
    * Recompute the size of the sliding button's element
    *
    * \param nState button state to be considered
    */
   void updateButtonSize (KUIElementState nState);

   /** Element for rendering this slider's background */
   KUIImage *_lpBackgroundImageElem;

   /** Sliding button */
   KUIButton *_lpButtonElem;

   /** Slider type */
   KUISliderType _nSliderType;

   /** true if value changed since the last check */
   bool _bValueChanged;

   /** true if slider was hit with a mousebutton or a touch since the last check */
   bool _bFirstDown;

   /** Current value */
   float _fValue;

   /** true if mouse is currently over the sliding button */
   bool _bMouseOverButton;

   /** true if active rectangle has been defined */
   bool _bActiveRectDefined;

   /** Active rectangle */
   KRectangle _rcActive;

   /** Background graphic for each state */
   KGraphic *_lpBackgroundGraphic[K_UIELEMENT_STATES];

   /** Image coordinates in background graphic for each state */
   KRectangle _rcBackgroundGraphic[K_UIELEMENT_STATES];

   /** Image coordinates in button graphic for each state (used to remember the size) */
   KRectangle _rcButtonGraphic[K_UIELEMENT_STATES];

   /** true if tinting the background is enabled for any state */
   bool _bBackgroundColorEnabled;

   /** Background tinting color for each state */
   float _fBackgroundColor[K_UIELEMENT_STATES][4];

   /** Background lightness factor for each state, 0..1 (fully dark..normal color) */
   float _fBackgroundLightness[K_UIELEMENT_STATES];

   /** true if stretching the background over X is enabled for each state */
   bool _bBackgroundStretchingXEnabled[K_UIELEMENT_STATES];

   /** X range of source coordinates that stretch in the background graphic, when enabled */
   float _fBackgroundStretchingX[K_UIELEMENT_STATES][2];

   /** true if stretching the background over Y is enabled for each state */
   bool _bBackgroundStretchingYEnabled[K_UIELEMENT_STATES];

   /** Y range of source coordinates that stretch in the background graphic, when enabled */
   float _fBackgroundStretchingY[K_UIELEMENT_STATES][2];

   /** Offset of top, left of background graphic from top, left of element, for each state */
   KVector2 _vBackgroundOffset[K_UIELEMENT_STATES];

   /** Page size when in scrollbar mode (0 if disabled) */
   float _fScrollbarPageSize;

   /** true if button is stretching horizontally for each state */
   bool _bButtonXStretch[K_UIELEMENT_STATES];

   /** true if button is stretching vertically for each state */
   bool _bButtonYStretch[K_UIELEMENT_STATES];

   /** true if currently dragging this slider's button */
   bool _bDraggingButton;

   /** Current paging direction in scrollbar mode; 0 if not paging, -1 if paging left/up, 1 if paging down/right */
   long _nScrollbarPagingDir;

   /** While paging, milliseconds left before repeating the page action */
   double _fScrollbarPagingTimer;
};

#endif   /* _KANJI_UISLIDER_H */
