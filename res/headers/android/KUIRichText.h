/**
 * Kanji game engine - rich text UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UIRICHTEXT_H
#define  _KANJI_UIRICHTEXT_H

/* Include definitions */
#include "KUIText.h"
#include "KUIImage.h"
#include "KTextFace.h"
#include "KTinyXml.h"

/**
 * Rendering flags
 */

#define  K_UIRICHTEXT_FLAG_BASELINE_ALIGN   (1)   /**< Align the top line of text with (default font baseline - default font height). Set by default: yes */
#define  K_UIRICHTEXT_FLAG_USE_SHADER       (2)   /**< Use shader to render glyphs and outlines from seperate channels. The font must also be rendered as multiple channels */
#define  K_UIRICHTEXT_FLAG_USE_OUTLINE_FONT (4)   /**< Use seperate font to render glyph and outlines */

/**
 * Rich text element
 */

class KUIRichText : public KUIElement {
public:
   /**
    * Check if the supplied element is a rich text, and cast as KUIRichText if it is
    *
    * \return KUIRichText if element is a rich text, NULL otherwise
    */
   static KUIRichText *isRichText (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIRichText (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUIRichText ();

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
    * Get compatibility level for this text element
    *
    * \return this font's compatibility level
    */
   long getCompatibilityLevel (long nLevel) const;

   /**
    * Set compatibility level for this text element
    *
    * \param nLevel compatibility level (defaults to 0 to support older games)
    */
   void setCompatibilityLevel (long nLevel);

   /**
    * Get font used for drawing text in this element
    *
    * \return font, NULL if none set
    */
   KTextFace *getTextFont (void) const;

   /**
    * Get font used for drawing outlines in this element, when the K_UIRICHTEXT_FLAG_USE_OUTLINE_FONT flag is set to denote using two different fonts
    *
    * \return font, NULL if none set
    */
   KTextFace *getOutlineFont (void) const;

   /**
    * Get height in pixels of the for used for drawing text in this element
    *
    * \return font height, 0 if not set
    */
   float getTextFontHeight (void);

   /**
    * Set default font to use for drawing text in this element
    *
    * \param lpFont font to use
    * \param fFontHeight font height in pixels (-1 to automatically determine)
    * \param fFontBaseline font baseline in pixels (-1 to set to the font height)
    */
   void setTextFont (KTextFace *lpFont, float fFontHeight = -1, float fFontBaseline = -1);

   /**
    * Set default font to use for drawing outlines in this element, when the K_UIRICHTEXT_FLAG_USE_OUTLINE_FONT flag is set to denote using two different fonts
    *
    * \param lpFont font to use for outlines; uses the same height and baseline as the text font
    */
   void setOutlineFont (KTextFace *lpFont);

   /**
    * Get current rendering flags
    *
    * \return mask of flags (K_UIRICHTEXT_FLAG_xxx)
    */
   long getFlags (void) const;

   /**
    * Clear one or more rendering flags
    *
    * \param nFlags mask of flags to clear (K_UIRICHTEXT_FLAG_xxx)
    */
   void clearFlags (long nFlags);

   /**
    * Set one or more rendering flags
    *
    * \param nFlags mask of flags to clear (K_UIRICHTEXT_FLAG_xxx)
    */
   void setFlags (long nFlags);

   /**
    * Get text currently drawn by this element
    *
    * \return text
    */
   const char *getText (void) const;

   /**
    * Set text drawn by this element; the default font must be set first
    *
    * \param lpszText text to draw, using the UTF-8 encoding
    */
   virtual void setText (const char *lpszText);

   /**
    * Get text flow mode
    *
    * \return flow mode
    */
   KUITextFlowMode getTextFlow (void) const;

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
   float getTextKerning (void) const;

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
   KUIVertAlignMode getTextVertAlignMode (void) const;

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
   float getTextVertSpacing (void) const;

   /**
    * Set vertical spacing between lines
    *
    * \param fVertSpacing new spacing, in pixels
    */
   void setTextVertSpacing (float fVertSpacing);

   /**
    * Set text adjustment offsets
    *
    * \param fHorizOffset new horizontal offset
    * \param fVertOffset new vertical offset
    */
   void setTextOffset (float fHorizOffset, float fVertOffset);

   /**
    * Get maximum number of characters to be drawn, starting with the first one
    *
    * \return maximum number of characters to be drawn (-1 for all, default)
    */
   long getMaxTextChars (void) const;

   /**
    * Set maximum number of characters to be drawn, starting with the first one
    *
    * \param nMaxChars maximum number of characters to be drawn (-1 for all, default)
    */
   void setMaxTextChars (long nMaxChars);

   /**
    * Get total number of characters in the current text
    *
    * \return total number of characters
    */
   long getTotalTextChars (void) const;

   /**
    * Set overall color (tint) to multiply with the default text and outline color, as well as the colors set by the font tags
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextMultiplierColor (float r, float g, float b, float a);

   /**
    * Set default color (tint) of text drawn by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextColor (float r, float g, float b, float a);

   /**
    * Set default text lightness factor (multiplied to each of the rgb components, used for darkening the text)
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
    * Set default color of the outline (tint) for the text drawn by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setTextOutlineColor (float r, float g, float b, float a);

   /**
    * Set default outline lightness factor (multiplied to each of the rgb components, used for darkening the outline)
    *
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setTextOutlineLightness (float fLightness);

   /**
    * Get width of current text as rendered by this element
    *
    * \return text width
    */
   float getCurrentTextWidth (void);

   /**
    * Get width of each line of current text as rendered by this element
    *
    * \param nArraySize maximum number of entries to be filled in the width array
    * \param fpfWidthArray array of widths for each line
    *
    * \return number of total lines that will be drawn for text
    */
   long getCurrentTextWidthArray (long nArraySize, float *fpfWidthArray);

   /**
    * Get height of current text as rendered by this element
    *
    * \return text height
    */
   float getCurrentTextHeight (void);

   /**
    * Get coordinates of a specific character in current text,  with this element's current font and kerning
    *
    * \param nCharIndex character index (0..total number of chars-1)
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

   /**
    * Get font. Derive this method to implement the font face="name" support
    *
    * \param lpszFontFaceName font face, as supplied by the font face="name" tag
    * \param lpFontFace returned font
    * \param fFontHeight returned font height in pixels, -1 to automatically determine
    * \param fFontBaseline returned font baseline in pixels, -1 to set to the font height
    *
    * \return true if font was found and returned, false if not
    */
   virtual bool getFontFace (const char *lpszFontFaceName, KTextFace *&lpFontFace, float &fFontHeight, float &fFontBaseline);

   /**
    * Get image. Derive this method to implement the img name="name" support
    *
    * \param lpszImageName name of image to get, as supplied by the img name="name" tag
    * \param nFrameIdx frame number to get, 0..n
    * \param lpGraphic returned graphic
    * \param rcSrc returned source coordinates to draw in graphic
    * \param fBaseline returned basedline in pixels, -1 to set to the height of the returned rectangle
    * \param bXFlip returned X flip flag
    * \param bYFlip returned Y flip flag
    *
    * \return true if image was found and returned, false if not
    */
   virtual bool getImage (const char *lpszImageName, long nFrameIdx, KGraphic *&lpGraphic, KRectangle &rcSrc, float &fBaseline, bool &bXFlip, bool &bYFlip);

private:
   /**
    * Recursively count XML nodes that hold content to be rendered
    *
    * \param lpParentXmlNode parent XML node
    * \param lpXmlNode current XML node to check and count children of
    *
    * \return number of nodes with contenxt
    */
   long countXmlContentNodes (const KTiXmlNode *lpParentXmlNode, const KTiXmlNode *lpXmlNode);

   /**
    * Recursively parse XML nodes and output text nodes
    *
    * \param lpParentXmlNode parent XML node
    * \param lpXmlNode current XML node
    * \param lpCurFont current font face
    * \param fCurFontHeight height of current font
    * \param fCurFontBaseline baseline of current font
    * \param lpfCurTextColor current element color
    * \param lpfCurOutlineColor current outline color
    * \param lpOutputNode array of text nodes to output to
    * \param nCurOutputNodeIdx current index into text nodes array
    * \param nTotalChars total number of characters output into the text nodes (an <img> image counts as one character)
    */
   void parseXmlNode (const KTiXmlNode *lpParentXmlNode, const KTiXmlNode *lpXmlNode, KTextFace *lpCurFont, float fCurFontHeight, float fCurFontBaseline,
                      const float *lpfCurTextColor, const float *lpfCurOutlineColor, KTextFaceNode *lpOutputNode, long &nCurOutputNodeIdx, long &nTotalChars);

   /** Recalculate size of text to be drawn */
   void updateTextDimensions (void);

   /** Compatibility level */
   long _nCompatibilityLevel;

   /** Rendering flags */
   long _nFlags;

   /** Text font, and optionally outline font */
   KTextFace *_lpFont[2];
   
   /** Font height in pixels (-1 to automatically determine) */
   float _fFontHeight;

   /** Font baseline in pixels (-1 to set to the font height */
   float _fFontBaseline;

   /** Text flow mode */
   KUITextFlowMode _nTextFlow;

   /** Text kerning */
   float _fTextKerning;

   /** Text vertical aligment mode */
   KUIVertAlignMode _nTextVertAlignMode;

   /** Vertical spacing between each line of text */
   float _fTextVertSpacing;

   /** Additional X offset */
   float _fXOffset;

   /** Additional Y offset */
   float _fYOffset;

   /** Maximum number of characters to drawn, -1 for all (default) */
   long _nMaxDrawnChars;

   /** Total number of characters (counting <img> images as one character each) to be rendered */
   long _nTotalChars;

   /** Overall color multiplier */
   float _fColorMultiplier[4];

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
   char *_lpszRichText;

   /** Number of bytes currently occupied by the text to be drawn */
   size_t _nCurRichTextLen;

   /** Maximum number of bytes available for the text to be drawn */
   size_t _nTextBytes;

   /** Parsed xml text */
   KTiXmlDocument _doc;

   /** Number of text nodes to be drawn */
   long _nTextNodes;

   /** Number of text nodes currently allocated */
   long _nMaxTextNodes;

   /** Text nodes to be drawn */
   KTextFaceNode *_lpTextNodes;

   /** Text alignment style */
   long _nTextStyle;

   /** Current height of the text to be drawn, in pixels */
   float _fTextHeight;

   /** Name of font to be dereferenced when this text is deleted, NULL for none */
   char *_lpszLoadedFontFilename;

   /** Height of font to be dereferenced when this text is deleted, in pixels */
   long _nLoadedFontHeight;
};

#endif   /* _KANJI_UIRICHTEXT_H */
