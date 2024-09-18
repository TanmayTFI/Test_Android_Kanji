/**
 * Kanji game engine - text edition field UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UIEDITFIELD_H
#define  _KANJI_UIEDITFIELD_H

/* Include definitions */
#include "KUIElement.h"
#include "KUIImage.h"
#include "KTextFace.h"

/** Default maximum number of characters that can be input in an edit field, can be changed with setMaxChars() */
#define K_UIEDITFIELD_MAXCHARS     260

/** Messages sent by KUIEditField */
enum {
   K_UIEDITFIELD_MSGINPUT = K_UIMSG ('k','e','f'),    /**< Filter character being input, see onInput(). nParam3 = character. Reply character if input is accepted, 0 if input is refused */
   K_UIEDITFIELD_MSGTEXTCHANGED,                      /**< Text has changed */
   K_UIEDITFIELD_MSGTEXTVALIDATED,                    /**< Text has been validated */
};

/**
 * Text edition field UI element
 */

class KUIEditField : public KUIElement {
public:
   /**
    * Check if the supplied element is an edit field, and cast as KUIEditField if it is
    *
    * \return KUIEditField if element is an edit field, NULL otherwise
    */
   static KUIEditField *isEditField (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIEditField (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUIEditField ();

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
    * Get maximum number of characters for the text that can be input in this element
    *
    * \return maximum number of characters
    */
   long getMaxChars (void);

   /**
    * Set maximum number of characters for the text that can be input in this element
    *
    * \param nMaxChars new maximum number of characters
    */
   void setMaxChars (long nMaxChars);

   /**
    * Enable or disable secure (password entry) mode; in secure mode, the text is input as usual but the displayed characters are replaced by a single one, specified to this
    * method (for instance '*')
    *
    * \param bEnable true to enable, false to disable
    * \param nSecureChar character to display instead of the input characters
    */
   void setSecureMode (bool bEnable, unsigned long nSecureChar);

   /**
    * Set offset of top, left of background graphic from top, left of element
    *
    * \param nState state to set offset for
    * \param x X offset
    * \param y Y offset
    */
   void setBackgroundOffset (KUIElementState nState, float x, float y);

   /**
    * Set background graphic for an edit field's state
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
    * Set tinting color for the background graphic for an edit field's state
    *
    * \param nState state to set tinting color for
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setBackgroundColor (KUIElementState nState, float r, float g, float b, float a);

   /**
    * Set lightness factor (multiplied to each of the rgb components), used for darkening the background graphic for an edit field's state
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
    * Set area of the element, in element's coordinates, where text will be displayed and edited (defaults to the whole element)
    *
    * \param bEnable true to enable, false to disable
    * \param x1 X coordinate of top,left of edition rectangle, relative to the top,left of the element
    * \param y1 Y coordinate of top,left of edition rectangle, relative to the top,left of the element
    * \param x2 X coordinate of bottom,right + 1 of edition rectangle, relative to the top,left of the element
    * \param y2 Y coordinate of bottom,right + 1 of edition rectangle, relative to the top,left of the element
    */
   void setActiveRect (bool bEnable, float x1, float y1, float x2, float y2);

   /**
    * Set font to use for drawing this edit field's text
    *
    * \param lpFont font to use
    */
   void setTextFont (KTextFace *lpFont);

   /**
    * Get text currently entered in this edit field. Text encoding depends on the font used
    *
    * \return text
    */
   const char *getText (void);

   /**
    * Set text to be displayed and edited in this edit field. Text encoding depends on the font used; the font must be set first
    */
   void setText (const char *lpszText);

   /**
    * Get current number of characters in the input text
    *
    * \return current number of characters
    */
   long getCurChars (void);

   /**
    * Set the alignment mode of the text displayed by this element
    *
    * \param nMode new mode (K_UIELEMENT_xxx)
    */
   void setTextAlignment (KUIHorizAlignMode nMode);

   /**
    * Set scaling factor for the text being displayed and edited in this edit field
    *
    * \param fScale scaling factor
    */
   void setTextScale (float fScale);

   /**
    * Set horizontal spacing between characters for the text displayed by this element
    *
    * \param fKerning new kerning, in pixels
    */
   void setTextKerning (float fKerning);

   /**
    * Set color (tint) of the text displayed in this button
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextColor (float r, float g, float b, float a);

   /**
    * Set color (tint) of the character displayed under the cursor in this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextHighlightColor (float r, float g, float b, float a);

   /**
    * Set thickness of the outline of the text displayed by this element
    *
    * \param fThickness new thickness (0 to disable)
    */
   void setTextOutlineThickness (float fThickness);

   /**
    * Set color of the outline (tint) for the text displayed by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextOutlineColor (float r, float g, float b, float a);

   /**
    * Set color of the outline (tint) for the character displayed under the cursor in this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextOutlineHighlightColor (float r, float g, float b, float a);

   /**
    * Set color of the cursor for this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setCursorColor (float r, float g, float b, float a);

   /**
    * Check if text has changed since last check
    *
    * \return true if text has changed, false if not
    */
   bool hasTextChanged (void);

   /**
    * Check if text has been validated (return key was pressed) since last check
    *
    * \return true if text was validated, false if not
    */
   bool isTextValidated (void);

   /**
    * Validate text, as if the return key was pressed. Validation will be reported to the game and keyboard focus will be lost
    */
   void validateText (void);

protected:
   /**
    * Filter character being input. Derive this method to restrict characters being input, for instance to create an edit field that accepts numbers only; the
    * implementation in KUIEditField accepts all characters
    *
    * \param c character being input (including control codes such as 8 for backspace, 10 for ctrl+return or 13 for return)
    *
    * \return character (can be different than the character passed in) to accept input, 0 to refuse it
    */
   virtual unsigned long onInput (unsigned long c);

   /**
    * Handle that text has changed
    */
   virtual void onTextChanged (void);

   /**
    * Handle that text has been validated (the return key was pressed)
    */
   virtual void onTextValidated (void);

   /**
    * Draw the cursor for this element. Derive this method to implement your own
    *
    * \param dx1 X coordinate of top, left of area to draw cursor at
    * \param dy1 Y coordinate of top, left of area to draw cursor at
    * \param dx2 X coordinate of bottom, right of area to draw cursor at, +1
    * \param dy2 Y coordinate of bottom, right of area to draw cursor at, +1
    * \param r red component of cursor color
    * \param g red component of cursor color
    * \param b red component of cursor color
    * \param a red component of cursor color
    */
   virtual void blitCursor (float dx1, float dy1, float dx2, float dy2, float r, float g, float b, float a);

   /**
    * Handle input event
    *
    * \param lpKEvent event
    *
    * \return true if event was processed by this element, false if it wasn't
    */
   virtual bool handleEvent (KEvent *lpKEvent);

   /**
    * Handle keyboard focus
    *
    * \param bAcquire true if acquiring it, false if losing it
    *
    * \return if acquiring the focus, true if focus was accepted, false if it wasn't (element doesn't handle the keyboard) ; ignored when losing the focus
    */
   virtual bool handleFocus (bool bAcquire);

   /** Update logic for this element */
   virtual void move (void);

   /**
    * Draw once this element and its children have been drawn
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blitPost (float fBlend, KMatrix *lpMatrix);

private:
   /**
    * Method called back when drawing the character under the cursor
    *
    * \param lpFont font used for drawing the text
    * \param lpUserData instance of KUIEditField
    * \param nStage current stage (K_STRCLIP_xxx)
    * \param x1 leftmost X coordinate of character under the cursor
    * \param y1 topmost Y coordinate of character under the cursor
    * \param x2 rightmost X coordinate of character under the cursor + 1
    * \param y2 rightmost Y coordinate of character under the cursor + 1
    */
   static void cursorCallBack (KTextFace *lpFont, void *lpUserData, long nStage, float x1, float y1, float x2, float y2);

   /** Element for rendering this button's background */
   KUIImage *_lpBackgroundImageElem;

   /** true if edition rectangle has been defined */
   bool _bActiveRectDefined;

   /** Edition rectangle */
   KRectangle _rcActive;

   /** Index of the character that the cursor is currently over */
   long _nCursorIdx;

   /** Current total number of characters in the input text */
   long _nCurChars;

   /** Maximum number of characters that can be input in this element */
   long _nMaxChars;

   /** true if password entry mode is enabled */
   bool _bSecureMode;

   /** character to be displayed instead of input characters with, in password mode */
   unsigned long _nSecureChar;

   /** true if text has changed */
   bool _bTextChanged;

   /** true if text has been validated */
   bool _bTextValidated;

   /** true if this element currently has the keyboard focus */
   bool _bHasFocus;

   /** Current panning offset for drawing the text being edited */
   float _fCurTextPanX;

   /** true if currently drawing the text's outline, false if not */
   bool _bDrawingOutline;

   /** Font */
   KTextFace *_lpTextFont;

   /** Text alignment mode */
   KUIHorizAlignMode _nTextAlignMode;

   /** Text scale */
   float _fTextScale;

   /** Text kerning */
   float _fTextKerning;

   /** Text color */
   float _fTextColor[4];

   /** Text color when under the cursor */
   float _fTextHighlightColor[4];

   /** Outline thickness */
   float _fOutlineThickness;

   /** Outline color */
   float _fOutlineColor[4];

   /** Outline color when under the cursor */
   float _fOutlineHighlightColor[4];

   /** Cursor color */
   float _fCursorColor[4];

   /** Size of the buffers for inputting text, in bytes */
   long _nTextBufferSize;

   /** Currently input text*/
   char *_lpText;

   /** Currently displayed text when in password mode (the same secure character repeated as many times as the number of chars in the actual text) */
   char *_lpSecureText;

   /** Background texture for each state */
   KGraphic *_lpBackgroundGraphic[K_UIELEMENT_STATES];

   /** Image coordinates in background texture for each state */
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
};

#endif   /* _KANJI_UIEDITFIELD_H */
