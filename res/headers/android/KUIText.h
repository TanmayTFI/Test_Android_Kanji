/**
 * Kanji game engine - text UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UITEXT_H
#define  _KANJI_UITEXT_H

/* Include definitions */
#include "KUIElement.h"
#include "KUIImage.h"
#include "KTextFace.h"

/**
 * Text flow
 */

typedef enum {
   K_UITEXT_LEFT = 0,      /**< Align lines left */
   K_UITEXT_RIGHT,         /**< Align lines right */
   K_UITEXT_CENTER,        /**< Center each line horizontally */
   K_UITEXT_JUSTIFIED,     /**< Make each line the same width by increasing the width of spaces between words to fill the width */
   K_UITEXT_WRAP,          /**< Make each line the same width by breaking words in the middle if necessary */
} KUITextFlowMode;

/**
 * Text element
 */

class KUIText : public KUIElement {
public:
   /**
    * Check if the supplied element is a text, and cast as KUIText if it is
    *
    * \return KUIText if element is a text, NULL otherwise
    */
   static KUIText *isText (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIText (KUIElement *lpParentElem);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    * \param x X coordinate of top,left of element, relative to the parent
    * \param y Y coordinate of top,left of element, relative to the parent
    * \param lpszElemName text element's name
    * \param lpszFontFilename name of font to be loaded, using KManagedFont; an instance of KGame must have been created
    * \param lpszText text to display
    * \param nFontHeightPix height of font to be loaded, in pixels
    * \param nFlow text flow mode (K_UITEXT_LEFT, RIGHT, CENTER, JUSTIFIED...)
    * \param nVertAlignMode vertical alignment mode (K_UIELEMENT_TOP, BOTTOM, CENTER..)
    */
   KUIText (KUIElement *lpParentElem, float x, float y, const char *lpszElemName, const char *lpszText, const char *lpszFontFilename, long nFontHeightPix,
            KUITextFlowMode nFlow = K_UITEXT_LEFT, KUIVertAlignMode nVertAlignMode = K_UIELEMENT_TOP);

   /** Destructor */
   virtual ~KUIText ();

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
    * Get font used for drawing text in this element
    *
    * \return font, NULL if none set
    */
   KTextFace *getTextFont (void);

   /**
    * Get height in pxiels of the for used for drawing text in this element
    *
    * \return font height, 0 if not set
    */
   float getTextFontHeight (void);

   /**
    * Set font to use for drawing text in this element
    *
    * \param lpFont font to use
    * \param fFontHeight font height in pixels (-1 to automatically determine)
    */
   void setTextFont (KTextFace *lpFont, float fFontHeight = -1);

   /**
    * Enable or disable using shaders to draw this text, if possible. The font that is set for drawing this text must also be rendered as multiple channels for shader usage
    *
    * \param bEnable true to enable, false to disable
    *
    * \return true if shader was enabled or disabled correctly, false if not supported
    */
   bool enableTextShader (bool bEnable);

   /**
    * Get text currently drawn by this element
    *
    * \return text
    */
   const char *getText (void);

   /**
    * Set text drawn by this element; the font must be set first
    *
    * \param lpszText text to draw
    */
   virtual void setText (const char *lpszText);

   /**
    * Get text flow mode
    *
    * \return flow mode
    */
   KUITextFlowMode getTextFlow (void);

   /**
    * Set text flow mode
    *
    * \param nFlow new mode (K_UITEXT_xxx)
    */
   void setTextFlow (KUITextFlowMode nFlow);

   /**
    * Get horizontal spacing between characters
    *
    * \return kerning, in pixels
    */
   float getTextKerning (void);

   /**
    * Set horizontal spacing between characters
    *
    * \param fKerning new kerning, in pixels
    */
   void setTextKerning (float fKerning);

   /**
    * Get vertical alignment mode
    *
    * \return vertical alignment mode
    */
   KUIVertAlignMode getTextVertAlignMode (void);

   /**
    * Set vertical alignment mode
    *
    * \param nMode new mode
    */
   void setTextVertAlignMode (KUIVertAlignMode nMode);

   /**
    * Get vertical spacing between lines
    *
    * \return spacing, in pixels
    */
   float getTextVertSpacing (void);

   /**
    * Set vertical spacing between lines
    *
    * \param fVertSpacing new spacing, in pixels
    */
   void setTextVertSpacing (float fVertSpacing);

   /**
    * Set color (tint) of text drawn by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextColor (float r, float g, float b, float a);

   /**
    * Set text lightness factor (multiplied to each of the rgb components, used for darkening the text)
    *
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setTextLightness (float fLightness);

   /**
    * Enable or disable outline and set its thickness. If shader rendering is enabled, this method must be called and the supplied thickness must match the outline thickness set when
    * rendering the font, so that text is correctly aligned within the UI element without being cut off at the outline.
    *
    * \param fThickness new thickness (0 to disable)
    */
   void setTextOutlineThickness (float fThickness);

   /**
    * Set color of the outline (tint) for the text drawn by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextOutlineColor (float r, float g, float b, float a);

   /**
    * Set outline lightness factor (multiplied to each of the rgb components, used for darkening the outline)
    *
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setTextOutlineLightness (float fLightness);

   /**
    * Get width of specified text with this element's current font and kerning
    *
    * \param lpszText text to measure width of
    *
    * \return text width
    */
   float getTextWidth (const char *lpszText);

   /**
    * Get width of each line of specified text with this element's current font and kerning
    *
    * \param lpszText text to measure width of
    * \param nArraySize maximum number of entries to be filled in the width array
    * \param fpfWidthArray array of widths for each line
    *
    * \return number of total lines that will be drawn for text
    */
   long getTextWidthArray (const char *lpszText, long nArraySize, float *fpfWidthArray);

   /**
    * Get height of specified text with this element's current font and kerning
    *
    * \param lpszText text to measure height of
    * \param bUseVertSpacing true to use vertical spacing defined by setTextVertSpacing() when calculating the height, false not to (default)
    *
    * \return text height
    */
   float getTextHeight (const char *lpszText, bool bUseVertSpacing = false);

   /**
    * Get coordinates of a specific character in the specified text,  with this element's current font and kerning
    *
    * \param lpszText text to get position of a character for
    * \param nCharIndex character index (0..text length-1)
    * \param x1 returned top, left X coordinate of character, if successful
    * \param y1 returned top, left Y coordinate of character, if successful
    * \param x2 returned bottom, right X coordinate of character + 1, if successful
    * \param y2 returned bottom, right Y coordinate of character + 1, if successful
    *
    * \return true if character was found and coordinates were returned, false if not
    */
   bool getCharacterPosInText (const char *lpszText, long nCharIndex, float &x1, float &y1, float &x2, float &y2);

   /**
    * Get width of current text with this element's current font and kerning
    *
    * \return text width
    */
   float getCurrentTextWidth (void);

   /**
    * Get width of each line of current text with this element's current font and kerning
    *
    * \param nArraySize maximum number of entries to be filled in the width array
    * \param fpfWidthArray array of widths for each line
    *
    * \return number of total lines that will be drawn for text
    */
   long getCurrentTextWidthArray (long nArraySize, float *fpfWidthArray);

   /**
    * Get height of current text with this element's current font and kerning
    *
    * \param bUseVertSpacing true to use vertical spacing defined by setTextVertSpacing() when calculating the height, false not to (default)
    *
    * \return text height
    */
   float getCurrentTextHeight (bool bUseVertSpacing = false);

   /**
    * Get coordinates of a specific character in current text,  with this element's current font and kerning
    *
    * \param nCharIndex character index (0..text length-1)
    * \param x1 returned top, left X coordinate of character, if successful
    * \param y1 returned top, left Y coordinate of character, if successful
    * \param x2 returned bottom, right X coordinate of character + 1, if successful
    * \param y2 returned bottom, right Y coordinate of character + 1, if successful
    *
    * \return true if character was found and coordinates were returned, false if not
    */
   bool getCharacterPosInCurrentText (long nCharIndex, float &x1, float &y1, float &x2, float &y2);

protected:
   /**
    * Draw this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blit (float fBlend, KMatrix *lpMatrix);

private:
   /** Recalculate size of text to be drawn */
   void updateTextDimensions (void);

   /** Font */
   KTextFace *_lpFont;
   
   /** Use shader for drawing this text */
   bool _bUseShader;

   /** Font height in pixels (-1 to automatically determine) */
   float _fFontHeight;

   /** Text flow mode */
   KUITextFlowMode _nTextFlow;

   /** Text kerning */
   float _fTextKerning;

   /** Text vertical aligment mode */
   KUIVertAlignMode _nTextVertAlignMode;

   /** Vertical spacing between each line of text */
   float _fTextVertSpacing;

   /** Text color */
   float _fTextColor[4];

   /** Text lightness factor, 0..1 (fully dark..normal color) */
   float _fTextLightness;

   /** Outline thickness */
   float _fOutlineThickness;

   /** Outline color */
   float _fOutlineColor[4];

   /** Outline lightness factor, 0..1 (fully dark..normal color) */
   float _fOutlineLightness;

   /** Text to be drawn */
   char *_lpszText;

   /** Number of bytes currently occupied by the text to be drawn */
   size_t _nTextBytes;

   /** Text alignment style */
   long _nTextStyle;

   /** Current height of the text to be drawn, in pixels */
   float _fTextHeight;

   /** Name of font to be dereferenced when this text is deleted, NULL for none */
   char *_lpszLoadedFontFilename;

   /** Height of font to be dereferenced when this text is deleted, in pixels */
   long _nLoadedFontHeight;
};

#endif   /* _KANJI_UITEXT_H */
