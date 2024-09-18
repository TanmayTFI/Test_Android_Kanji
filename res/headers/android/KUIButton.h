/**
 * Kanji game engine - button UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UIBUTTON_H
#define  _KANJI_UIBUTTON_H

/* Include definitions */
#include "KUIElement.h"
#include "KUIImage.h"
#include "KUIText.h"

/** Messages sent by KUIButton */
enum {
   K_UIBUTTON_MSGCLICK = K_UIMSG ('k','b','t'),    /**< The button has been clicked */
   K_UIBUTTON_MSGDOUBLECLICK,                      /**< The button has been double-clicked */
   K_UIBUTTON_MSGFIRSTDOWN,                        /**< The button has just been pushed down */
};

/**
 * Clickable button UI element
 */

class KUIButton : public KUIElement {
public:
   /**
    * Check if the supplied element is a button, and cast as KUIButton if it is
    *
    * \return KUIButton if element is a button, NULL otherwise
    */
   static KUIButton *isButton (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIButton (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUIButton ();

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
    * Set new state
    *
    * \param nState state
    */
   virtual void setState (KUIElementState nState);

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
    * Set offset of rotation and scaling for background graphic, relative to its center
    *
    * \param nState state to set center offset for
    * \param cx X offset for center (defaults to 0)
    * \param cy Y offset for center (defaults to 0)
    */
   void setBackgroundCenter (KUIElementState nState, float cx, float cy);

   /**
    * Set scaling factor for the background graphic for a button's state
    *
    * \param nState state to set scaling factor for
    * \param fScale scaling factor for the specified state
    */
   void setBackgroundScale (KUIElementState nState, float fScale);

   /**
    * Set background graphic for a button's state
    *
    * \param nState state to set graphic for
    * \param lpGraphic source graphic for the background for this state
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    * \param lpHitMask hit mask for hit testing, NULL for none
    */
   void setBackgroundGraphic (KUIElementState nState, KGraphic *lpGraphic, float x1, float y1, float x2, float y2, const unsigned char *lpHitMask = NULL);

   /**
    * Set tinting color for the background graphic for a button's state
    *
    * \param nState state to set tinting color for
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setBackgroundColor (KUIElementState nState, float r, float g, float b, float a);

   /**
    * Set lightness factor (multiplied to each of the rgb components), used for darkening the background graphic for a button's state
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
    * Set offset for the caption text or graphic for a button's state
    *
    * \param nState state to set offset for
    * \param x horizontal offset for the specified state, in pixels
    * \param y vertical offset for the specified state, in pixels
    */
   void setCaptionOffset (KUIElementState nState, float x, float y);

   /**
    * Set scaling factor for the caption text or graphic for a button's state
    *
    * \param nState state to set scaling factor for
    * \param fScale scaling factor for the specified state
    */
   void setCaptionScale (KUIElementState nState, float fScale);

   /**
    * Set caption graphic for a button's state
    *
    * \param nState state to set graphic for
    * \param lpGraphic source graphic for the caption for this state
    * \param x1 source X coordinate of top,left of portion to display in graphic
    * \param y1 source Y coordinate of top,left of portion to display in graphic
    * \param x2 source X coordinate of bottom,right of portion to display in graphic + 1
    * \param y2 source Y coordinate of bottom,right of portion to display in graphic + 1
    */
   void setCaptionGraphic (KUIElementState nState, KGraphic *lpGraphic, float x1, float y1, float x2, float y2);

   /**
    * Set tinting color for the caption graphic for a button's state
    *
    * \param nState state to set tinting color for
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setCaptionGraphicColor (KUIElementState nState, float r, float g, float b, float a);

   /**
    * Set lightness factor (multiplied to each of the rgb components), used for darkening the caption graphic for a button's state
    *
    * \param nState state to set lightness factor for
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setCaptionGraphicLightness (KUIElementState nState, float fLightness);

   /**
    * Set Z rotation angle for caption graphic, in radians
    *
    * \param fZAngle rotation angle in radians
    */
   void setCaptionGraphicZAngle (float fZAngle);

   /**
    * Set font to use for drawing this button's text caption
    *
    * \param lpFont font to use
    * \param fFontHeight font height in pixels (-1 to automatically determine)
    */
   void setTextFont (KTextFace *lpFont, float fFontHeight = -1);

   /**
    * Set this button's text caption
    *
    * \param lpszText text to draw
    */
   void setText (const char *lpszText);

   /**
    * Set this button's text caption flow mode
    *
    * \param nMode new mode (K_UITEXT_xxx)
    */
   void setTextFlow (KUITextFlowMode nMode);

   /**
    * Set horizontal spacing between characters for this button's text caption
    *
    * \param fKerning new kerning, in pixels
    */
   void setTextKerning (float fKerning);

   /**
    * Set vertical alignment mode for this button's text caption
    *
    * \param nMode new mode
    */
   void setTextVertAlignMode (KUIVertAlignMode nMode);

   /**
    * Set vertical spacing between lines for this button's text caption
    *
    * \param fVertSpacing new spacing, in pixels
    */
   void setTextVertSpacing (float fVertSpacing);

   /**
    * Set color (tint) of this button's text caption
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextColor (float r, float g, float b, float a);

   /**
    * Enable or disable outline of this button's text caption and set its thickness
    *
    * \param fThickness new thickness (0 to disable)
    */
   void setTextOutlineThickness (float fThickness);

   /**
    * Set color of the outline (tint) for this button's text caption
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextOutlineColor (float r, float g, float b, float a);

   /**
    * Check if button is currently pressed
    *
    * \return true if currently pressed, false if not
    */
   bool isDown (void);

   /**
    * Check if button has been clicked since last check
    *
    * \return true if clicked, false if not
    */
   bool isClicked (void);

   /**
    * Procedurally click the button like if the user pressed it
    */
   void click (void);

   /**
    * Get UI element that renders the background image for this button
    *
    * \return background image
    */
   KUIImage *getBackgroundImage (void);

   /**
    * Get UI element that renders the caption image for this button
    *
    * \return background image
    */
   KUIImage *getCaptionImage (void);

   /**
    * Get UI element that renders the caption text for this button
    *
    * \return background image
    */
   KUIText *getCaptionText (void);

   /**
    * Capture element used for the background image for this button
    *
    * \param bCapture true to prevent changes to the background image by this button, false to allow them (for instance allowing the button to change the graphic on a state change, default)
    */
   void captureBackgroundImage (bool bCapture);

   /**
    * Capture element used for the caption image for this button
    *
    * \param bCapture true to prevent changes to the background image by this button, false to allow them (for instance allowing the button to change the graphic on a state change, default)
    */
   void captureCaptionImage (bool bCapture);

   /**
    * Capture element used for the caption text for this button
    *
    * \param bCapture true to prevent changes to the background image by this button, false to allow them (for instance allowing the button to change the graphic on a state change, default)
    */
   void captureCaptionText (bool bCapture);

protected:
   /**
    * Handle that the button has been clicked
    */
   virtual void onClick (void);

   /**
    * Handle that the button has been double-clicked
    */
   virtual void onDoubleClick (void);

   /**
    * Handle that the button has just been pushed down
    */
   virtual void onFirstDown (void);

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

   /**
    * Handle input event
    *
    * \param lpKEvent event
    *
    * \return true if event was processed by this element, false if it wasn't
    */
   virtual bool handleEvent (KEvent *lpKEvent);

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
   /** Element for rendering this button's background */
   KUIImage *_lpBackgroundImageElem;

   /** Element for rendering text as this button's caption */
   KUIText *_lpCaptionTextElem;

   /** Element for rendering an image as this button's caption */
   KUIImage *_lpCaptionImageElem;

   /** true to prevent changes to the background image */
   bool _bCaptureBackgroundImage;

   /** true to prevent changes to the caption image */
   bool _bCaptureCaptionImage;
   
   /** true to prevent changes to the caption text */
   bool _bCaptureCaptionText;

   /** Current scaling level for the background graphic */
   float _fCurBackgroundScale;

   /** Current scaling level for the caption graphic or text */
   float _fCurCaptionScale;

   /** true if button has been clicked since last check */
   bool _bClicked;

   /** Offset for each state */
   KVector2 _vCaptionOffset[K_UIELEMENT_STATES];

   /** Scaling level for the caption for each state */
   float _fCaptionScale[K_UIELEMENT_STATES];

   /** Scaling level for the background for each state */
   float _fBackgroundScale[K_UIELEMENT_STATES];

   /** Background graphic for each state */
   KGraphic *_lpBackgroundGraphic[K_UIELEMENT_STATES];

   /** Background graphic's hit mask for each state, NULL for none */
   const unsigned char *_lpBackgroundHitMask[K_UIELEMENT_STATES];

   /** Image coordinates in background graphic for each state */
   KRectangle _rcBackgroundGraphic[K_UIELEMENT_STATES];

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

   /** Offset of center of rotation and scaling for background graphic, for each state */
   KVector2 _vBackgroundCenter[K_UIELEMENT_STATES];

   /** Caption graphic for each state */
   KGraphic *_lpCaptionGraphic[K_UIELEMENT_STATES];

   /** Image coordinates in caption graphic for each state */
   KRectangle _rcCaptionGraphic[K_UIELEMENT_STATES];

   /** true if tinting the caption graphic is enabled for any state */
   bool _bCaptionGraphicColorEnabled;

   /** Caption graphic tinting color for each state */
   float _fCaptionGraphicColor[K_UIELEMENT_STATES][4];

   /** Caption graphic lightness factor for each state, 0..1 (fully dark..normal color) */
   float _fCaptionGraphicLightness[K_UIELEMENT_STATES];
};

#endif   /* _K_UIELEMENT_BUTTON_H */
