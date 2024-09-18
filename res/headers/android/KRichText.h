/**
 * Kanji game engine - rich text renderer definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_RICHTEXT_H
#define  _KANJI_RICHTEXT_H

/* Include definitions */
#include "KUIText.h"
#include "KUIImage.h"
#include "KTextFace.h"
#include "KTinyXml.h"

/**
 * Vertical alignment for rich text
 */

enum {
   K_RICHTEXT_TOP = 0,                          /**< Align with the top of the box */
   K_RICHTEXT_BOTTOM,                           /**< Align with the bottom of the box */
   K_RICHTEXT_VCENTER,                          /**< Center vertically */
};

/**
 * Rendering flags
 */

#define  K_RICHTEXT_FLAG_BASELINE_ALIGN   (1)   /**< Align the top line of text with (default font baseline - default font height). Set by default: yes */
#define  K_RICHTEXT_FLAG_USE_SHADER       (2)   /**< Use shader to render glyphs and outlines from seperate channels. The font must also be rendered as multiple channels */
#define  K_RICHTEXT_FLAG_USE_OUTLINE_FONT (4)   /**< Use seperate font to render glyph and outlines */
#define  K_RICHTEXT_FLAG_RAW_TEXT         (8)   /**< Do not parse tags in text, display literally */

/**
 * Rich text renderer
 */

class KRichText : public KObjectBase {
public:
   /**
    * Constructor
    */
   KRichText (void);

   /** Destructor */
   virtual ~KRichText ();

   /**
    * Set default font to use for drawing text in this element
    *
    * \param lpFont font to use
    * \param fFontHeight font height in pixels (-1 to automatically determine)
    * \param fFontBaseline font baseline in pixels (-1 to set to the font height)
    */
   void setFont (KTextFace *lpFont, float fFontHeight = -1, float fFontBaseline = -1);

   /**
    * Set default font to use for drawing outlines in this element, when the K_RICHTEXT_FLAG_USE_OUTLINE_FONT flag is set to denote using two different fonts
    *
    * \param lpFont font to use for outlines; uses the same height and baseline as the text font
    */
   void setOutlineFont (KTextFace *lpFont);

   /**
    * Enable or disable clipping to the draw box boundaries
    *
    * \param bEnable true to enable clipping, false to disable
    */
   void enableClipping (bool bEnable);

   /**
    * Get current rendering flags
    *
    * \return mask of flags (K_RICHTEXT_FLAG_xxx)
    */
   long getFlags (void);

   /**
    * Clear one or more rendering flags
    *
    * \param mask of flags to clear (K_RICHTEXT_FLAG_xxx)
    */
   void clearFlags (long nFlags);

   /**
    * Set one or more rendering flags
    *
    * \param mask of flags to clear (K_RICHTEXT_FLAG_xxx)
    */
   void setFlags (long nFlags);

   /**
    * Set text drawn by this element; the default font must be set first
    *
    * \param lpszText text to draw, using the UTF-8 encoding
    */
   void setText (const char *lpszText);

   /**
    * Set text scaling factors (to rescale the text without affecting the draw box)
    *
    * \param fScaleX X scaling factor (1.0f for default)
    * \param fScaleY Y scaling factor (1.0f for default)
    */
   void setTextScale (float fScaleX, float fScaleY);

   /**
    * Set text flow mode
    *
    * \param nStyle new style (TEXTSTYLE_xxx)
    */
   void setStyle (long nStyle);

   /**
    * Set horizontal spacing between characters
    *
    * \param fKerning new kerning, in pixels
    */
   void setKerning (float fKerning);

   /**
    * Set vertical alignment mode
    *
    * \param nMode new mode (K_RICHTEXT_xxx)
    */
   void setVertAlignMode (long nMode);

   /**
    * Set vertical spacing between lines
    *
    * \param fVertSpacing new spacing, in pixels
    */
   void setVertSpacing (float fVertSpacing);

   /**
    * Set horizontal adjustment offset
    *
    * \param fHorizOffset new horizontal offset
    */
   void setHorizOffset (float fHorizOffset);

   /**
    * Set vertical adjustment offset
    *
    * \param fVertOffset new vertical offset
    */
   void setVertOffset (float fVertOffset);

   /**
    * Set maximum number of characters to be drawn, starting with the first one
    *
    * \param nMaxChars maximum number of characters to be drawn (-1 for all, default)
    */
   void setMaxChars (long nMaxChars);

   /**
    * Get total number of characters in the current text
    *
    * \return total number of characters
    */
   long getTotalChars (void);

   /**
    * Set overall color (tint) to multiply with the default text and outline color, as well as the colors set by the font tags
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setColor (float r, float g, float b, float a);

   /**
    * Set default color (tint) of text drawn by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setDefaultTextColor (float r, float g, float b, float a);

   /**
    * Set default color of the outline (tint) for the text drawn by this element
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setDefaultOutlineColor (float r, float g, float b, float a);

   /**
    * Enable or disable outline and set its thickness. If shader rendering is enabled, this method must be called and the supplied thickness must match the outline thickness set when
    * rendering the font, so that text is correctly aligned within the UI element without being cut off at the outline.
    *
    * \param fThickness new thickness (0 to disable)
    */
   void setOutlineThickness (float fThickness);

   /**
    * Set box for drawing this instance's text in
    *
    * \param x1 X coordinate of top, left of box to draw in
    * \param y1 Y coordinate of top, left of box to draw in
    * \param x2 X coordinate of bottom, right of box to draw in, +1
    * \param y2 Y coordinate of bottom, right of box to draw in, +1
    */
   void setDrawBox (float x1, float y1, float x2, float y2);

   /**
    * Set center for rotation and scaling, in normalized coordinates (0..1)
    *
    * \param cx X center (defaults to 0.5f)
    * \param cy Y center (defaults to 0.5f)
    */
   void setNormalizedCenter (float cx, float cy);

   /**
    * Set rotation angle in degrees
    *
    * \param fAngle rotation angle
    */
   void setRotation (float fAngle);

   /**
    * Set scaling factors
    *
    * \param fScaleX X scaling factor (1.0f for default)
    * \param fScaleY Y scaling factor (1.0f for default)
    */
   void setScale (float fScaleX, float fScaleY);

   /**
    * Set shearing factors
    *
    * \param fShearX X shearing factor (0.0f for default)
    * \param fShearY Y shearing factor (0.0f for default)
    */
   void setShear (float fShearX, float fShearY);

   /**
    * Set user transformation matrix to use for drawing. The matrix is applied before scaling, shearing and other transforms
    *
    * \param mat transformation matrix to use for drawing
    */
   void setMatrix (const KMatrix &mat);

   /**
    * Get actual width of text rendered by this instance. Call setDrawBox() first
    *
    * \return actual text width
    */
   float getActualWidth (void);

   /**
    * Get actual height of text rendered by this instance. Call setDrawBox() first
    *
    * \return actual text height
    */
   float getActualHeight (void);

   /**
    * Draw this instance's text
    */
   void blit (void);

protected:
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

   /** Rendering flags */
   long _nFlags;

   /** Text font, and optionally outline font */
   KTextFace *_lpFont[2];
   
   /** Screen rectangle for drawing this text in */
   KRectangle _rcDst;

   /** Rotation angle in degrees */
   float _fRotation;

   /** X scaling factor */
   float _fScaleX;

   /** Y scaling factor */
   float _fScaleY;

   /** X shearing factor */
   float _fShearX;

   /** Y shearing factor */
   float _fShearY;

   /** Additional X offset */
   float _fXOffset;

   /** Additional Y offset */
   float _fYOffset;

   /** Center in normalized coordinates (0..1, 0..1) */
   KVector2 _vNormalizedCenter;

   /** User transformation matrix */
   KMatrix _matUser;

   /** true if clipping is enabled */
   bool _bEnableClipping;

   /** Font height in pixels (-1 to automatically determine) */
   float _fFontHeight;

   /** Font baseline in pixels (-1 to set to the font height */
   float _fFontBaseline;

   /** Text alignment style */
   long _nTextStyle;

   /** Text kerning */
   float _fTextKerning;

   /** Text vertical aligment mode */
   long _nTextVertAlignMode;

   /** Vertical spacing between each line of text */
   float _fTextVertSpacing;

   /** Text X scaling factor */
   float _fTextScaleX;

   /** Text Y scaling factor */
   float _fTextScaleY;

   /** Maximum number of characters to drawn, -1 for all (default) */
   long _nMaxDrawnChars;

   /** Total number of characters (counting <img> images as one character each) to be rendered */
   long _nTotalChars;

   /** Overall color multiplier */
   float _fColorMultiplier[4];

   /** Text color */
   float _fTextColor[4];

   /** Outline thickness */
   float _fOutlineThickness;

   /** Outline color */
   float _fOutlineColor[4];

   /** Text to be drawn */
   char *_lpszRichText;

   /** Number of bytes currently occupied by the text to be drawn */
   size_t _nTextBytes;

   /** Parsed xml text */
   KTiXmlDocument _doc;

   /** Number of text nodes to be drawn */
   long _nTextNodes;

   /** Number of text nodes currently allocated */
   long _nMaxTextNodes;

   /** Text nodes to be drawn */
   KTextFaceNode *_lpTextNodes;
};

#endif   /* _KANJI_RICHTEXT_H */
