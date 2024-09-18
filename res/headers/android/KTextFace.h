/**
 * Kanji rendering and I/O engine - base text rendering class definition
 *
 * (c) 2010
 */

#ifndef  _KANJI_KTEXTFACE_H
#define  _KANJI_KTEXTFACE_H

/* Include definitions */
#include "KObjectBase.h"
#include "KGraphic.h"
#include "KBatch.h"

/** Style for drawMultiline() */

#define TEXTSTYLE_LEFT        0  /**< Align lines left */
#define TEXTSTYLE_RIGHT       1  /**< Align lines right */
#define TEXTSTYLE_CENTER      2  /**< Center each line horizontally */
#define TEXTSTYLE_JUSTIFIED   3  /**< Make each line the same width by increasing the width of spaces between words to fill the width */
#define TEXTSTYLE_WRAP        4  /**< Make each line the same width by breaking words in the middle if necessary */
#define TEXTSTYLE_EDIT        5  /**< Make each line the same width by breaking words in the middle if necessary, and don't eliminate spaces at the end of each line */

/** Text encoding type */
#define K_ANSI                0  /**< 8-bit ANSI codes */
#define K_UTF8                1  /**< UTF-8 encoding */
#define K_WCHAR               2  /**< String is made of wchar_t (character byte size is operating system-dependent) in native byte order */
#define K_UTF16               3  /**< String is made of 16-bit codes in native byte order */
#define K_UTF16_SWAPPED       4  /**< String is made of 16-bit codes in reverse byte order */

/** Stage for the method called back by drawStringClippedF */
#define K_STRCLIP_BEFORE      0  /**< Just before drawing character of interest (for instance for changing the text color) */
#define K_STRCLIP_AFTER       1  /**< Just after drawing character of interest  (for instance for changing the text color back) */
#define K_STRCLIP_POST        2  /**< After the whole string is drawn (for instance for drawing a cursor's caret) */

/* Forward declarations */
class KTextFace;
class KText;
class KTrueText;
class KPixelShader;

/**
 * Callback when the character of interest is drawn by drawStringClippedF() or drawMultilineClipped()
 *
 * \param lpFont font used for drawing
 * \param lpCallBackData user data as passed to drawStringClippedF() or drawMultilineClipped()
 * \param nStage current stage (K_STRCLIP_xxx)
 * \param x1 X coordinate of top, left of clipped character box being drawn
 * \param y1 Y coordinate of top, left of clipped character box being drawn
 * \param x2 X coordinate of bottom, right of clipped character box being drawn, +1
 * \param y2 Y coordinate of bottom, right of clipped character box being drawn, +1
 */
typedef void (*KTextFaceCursorCallBack)(KTextFace *lpFont, void *lpCallBackData, long nStage, float x1, float y1, float x2, float y2);

/** Operation to perform on text node */
typedef enum {
   /** No-op */
   KTEXTOP_NOOP = 0,

   /** Set color */
   KTEXTOP_SET_COLOR,
} KTextFaceNodeOp;

/** Parameter set for text node operation */
typedef enum {
   KTEXTOP_PARAM0 = 0,
   KTEXTOP_PARAM1 = 1,
   
   KTEXTOP_NPARAMSETS,
} KTextFaceNodeParamSet;

/**
 * Multiline text node
 */

class KTextFaceNode {
   friend class KTextFace;

public:
   /** Constructor */
   KTextFaceNode ();

   /**
    * Set font to use for this node
    *
    * \param lpFontFace font
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    * \param fFontBaseline font baseline, in pixels, -1 to set to the font height
    */
   void setFont (KTextFace *lpFontFace, float fFontHeight = -1, float fFontBaseline = -1);

   /**
    * Set text to use for this node
    *
    * \param lpszText text for this node
    * \param nTextEncoding text encoding, -1 to use the font's encoding (default)
    */
   void setText (const char *lpszText, long nTextEncoding = -1);

   /**
    * Set graphic to use for this node
    *
    * \param lpGraphic graphic
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param fGraphicBaseline baseline to use for this graphic, for relative alignment with text and other graphics, in pixels
    */
   void setGraphic (KGraphic *lpGraphic, float sx1, float sy1, float sx2, float sy2, bool bXFlip = false, bool bYFlip = false, float fGraphicBaseline = -1);

   /**
    * Clear text operations for this node
    *
    * \param nParamSet parameter set
    */
   void setNoOp (KTextFaceNodeParamSet nParamSet);

   /**
    * Set tinting color operation when drawing text using this node
    *
    * \param nParamSet parameter set
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setColor (KTextFaceNodeParamSet nParamSet, float r, float g, float b, float a);

   /**
    * Set tinting color operation when drawing text using this node
    *
    * \param nParamSet parameter set
    * \param lpszColor in color in #[aa]rrggbb format
    */
   void setColorAsText (KTextFaceNodeParamSet nParamSet, const char *lpszColor);

private:
   /** Font to use for this node, NULL if graphic node */
   KTextFace *lpFontFace;

   /** Font height */
   float fFontHeight;

   /** Font baseline */
   float fFontBaseline;
  
   /** Text to draw for this node */
   const char *lpszText;

   /** Encoding for this node's text */
   long nTextEncoding;

   /** Graphic to draw for this node, NULL if text node */
   KGraphic *lpGraphic;

   /** Source coordinates in graphic */
   KRectangle rcSrcGraphic;

   /** true to flip graphic horizontally when drawing it for this node, false not to */
   bool bXFlipGraphic;

   /** true to flip graphic vertically when drawing it for this node, false not to */
   bool bYFlipGraphic;

   /** Operation to perform for this node */
   KTextFaceNodeOp nOp[KTEXTOP_NPARAMSETS];

   /** Parameter sets for operation */
   float fOpParam[KTEXTOP_NPARAMSETS][4];

   /** Original color before drawing */
   float fOrigColor[4];

   /** Index of first character represented by this node, in the current line */
   long nFirstLineCharIdx;

   /** Index of last character represented by this node, in the current line, +1 */
   long nLastLineCharIdx;
};

/** Maximum depth of a font face's class hierarchy */
#define  K_TEXTFACE_MAXCLASSES  4

/** Maximum length of a font face's class name */
#define  K_TEXTFACE_MAXCLASSNAMELEN   32

/**
 * Base class for text rendering
 */

class KTextFace : public KObjectBase {
   friend class KText;
   friend class KTrueText;

public:
   /** Constructor */
   KTextFace ();

   /** Destructor */
   virtual ~KTextFace ();

   /**
    * Add specified class to the hierarchy of classes that this element is part of; this is used
    * to handle the isXXX() methods, for instance KUIButton::isButton() 
    *
    * \param lpszClassName name of class to declare for this element
    */
   void addClass (const char *lpszClassName);

   /**
    * Check if this element is of the specified class, somewhere in its hierarchy; this is used
    * to handle the isXXX() methods, for instance KUIButton::isButton() 
    *
    * \param lpszClassName name of class to be checked
    *
    * \return true if element is of the specified class, false if not
    */
   bool isOfClass (const char *lpszClassName) const;

   /**
    * Get name of a class that this element is part of
    *
    * \param nDepth depth to inspect in the class stack (0=last entry added by addClass(), 1=second to last, ...)
    *
    * \return class name, NULL if reached beyond maximum depth
    */
   const char *getClassName (long nDepth = 0) const;

   /**
    * Get compatibility level for this font's rendering
    *
    * \return this font's compatibility level
    */
   virtual long getCompatibilityLevel (void) const = 0;

   /**
    * Set compatibility level for this font's rendering
    *
    * \param nLevel compatibility level (defaults to 0 to support older games)
    */
   virtual void setCompatibilityLevel (long nLevel) = 0;

   /**
    * Get encoding used for this font's text
    *
    * \return text encoding (K_ANSI, ...)
    */
   virtual long getTextEncoding (void) const = 0;

   /**
    * Get height of this font, in pixels
    *
    * \return height in pixels
    */
   virtual long getHeightPix (void) = 0;

   /**
    * Get baseline of this font, in pixels
    *
    * \return baseline in pixels
    */
   virtual float getBaseline (void);

   /**
    * Set baseline of this font, in pixels
    *
    * \param fBaseline baseline in pixels
    */
   virtual void setBaseline (float fBaseline);

   /**
    * Get width of the specified string using this font, in pixels
    *
    * \param lpszText to get width of
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor that will be used for drawing the string
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    * \param lpPrevChar pointer to previous char, for correctly handling kerning pairs across multiple getStringWidth() calls, NULL for normal operation - internal
    * \param bLastComponent true if this is the last in a series of multiple getStringWidth() call for the same text, and normal operation, false if not - internal
    *
    * \return width
    */
   virtual float getStringWidth (const char *lpszText, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1, unsigned long *lpPrevChar = NULL, bool bLastComponent = true) = 0;

   /**
    * Get width of a set number of characters in the specified string, using this font, in pixels
    *
    * \param lpszText text to get width of
    * \param nCharCount number of characters, starting at the first one, to get width of
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor that will be used for drawing the string
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    *
    * \return width
    */
   virtual float getNCharWidth (const char *lpszText, long nCharCount, float fKerning, float fScale = 1, long nTextEncodingOverride = -1) = 0;

   /**
    * Get width of the specified character using this font, in pixels
    *
    * \param c character to get width of
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    *
    * \return width
    *
    * \private
    */
   virtual float getCharWidth (unsigned long c, float fKerning) = 0;

   /**
    * Get horizontal offset of the specified character using this font, in pixels
    *
    * \param c character to get width of
    *
    * \return horizontal offset
    *
    * \private
    */
   virtual float getCharXOffset (unsigned long c) = 0;

   /**
    * Get height of the specified string using this font, in pixels
    *
    * \param lpszText to get height of
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    *
    * \return height
    */
   virtual float getStringHeight (const char *lpszText, long nTextEncodingOverride = -1) = 0;

   /**
    * Set tinting color when drawing text using this font
    *
    * \param lpColor 4 floats where to store red, green, blue and alpha components
    */
   virtual void getColor (float *lpColor) = 0;

   /**
    * Set tinting color when drawing text using this font
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   virtual void setColor (float r, float g, float b, float a) = 0;

   /**
    * Set tinting color when drawing text using this font - same as setColor(), present for source compatibility purposes
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   virtual void setBlitColor (float r, float g, float b, float a) = 0;

   /**
    * Set blending factor for this font. This is multiplied with the alpha component of the tinting color
    *
    * \param fBlend blending factor
    */
   virtual void setBlend (float fBlend) = 0;

   /**
    * Set blending mode to use when drawing this font
    *
    * \param nBlendingMode blending mode (K_ALPHA_xxx, as defined in KGraphic.h)
    */
   virtual void setAlphaMode (long nBlendingMode) = 0;

   /**
    * Set alpha blending operations for this font
    *
    * \param nSrcAlphaOp new source alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nDstAlphaOp new destination alpha blending op for the color channels (K_ALPHAOP_xxx)
    */
   virtual void setAlphaOp (eKanjiAlphaOp nSrcAlphaOp, eKanjiAlphaOp nDstAlphaOp) = 0;

   /**
    * Draw specified string from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    * \param lpPrevChar pointer to previous char, for correctly handling kerning pairs across multiple getStringWidth() calls, NULL for normal operation - internal
    * \param lpEndX returned X coordinate of the top, right of the drawn text, NULL for normal operation - internal
    * \param lpMatrix transformation matrix to use for text, NULL for none
    */
   virtual void drawStringFromLeft (const char *lpszText, long dx, long dy, float fKerning = 0, long nTextEncodingOverride = -1, unsigned long *lpPrevChar = NULL, long *lpEndX = NULL,
                                    KMatrix *lpMatrix = NULL) = 0;

   /**
    * Draw specified string so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRight (const char *lpszText, long dx, long dy, float fKerning = 0) = 0;

   /**
    * Draw specified string so that it is centered horizontally between the specified coordinates
    *
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCentered (const char *lpszText, long dx1, long dx2, long dy, float fKerning = 0) = 0;

   /**
    * Draw specified integer value from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromLeft (long nValue, long dx, long dy, float fKerning = 0) = 0;

   /**
    * Draw specified integer value so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRight (long nValue, long dx, long dy, float fKerning = 0) = 0;

   /**
    * Draw specified integer value so that it is centered horizontally between the specified coordinates
    *
    * \param nValue value to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCentered (long nValue, long dx1, long dx2, long dy, float fKerning = 0) = 0;

   /**
    * Draw specified string and clip drawing within a box
    *
    * \param lpszText text to be drawn
    * \param dx1 leftmost X coordinate of box to draw in
    * \param dy1 topmost Y coordinate of box to draw in
    * \param dx2 rightmost X coordinate of box to draw in, +1
    * \param dy2 bottomost Y coordinate of box to draw in, + 1
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none)
    * \param lpCursorCallBack method called back when the character of interest is drawn, with the font, current stage (K_STRCLIP_xxx) and the coordinates at which the character of interest is drawn
    * \param lpCallBackData user data passed back to the method called back when the character of interest is drawn
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    * \param lpPrevChar pointer to previous char, for correctly handling kerning pairs across multiple getStringWidth() calls, NULL for normal operation - internal
    * \param lpEndXOffset returned X offset of the top, right of the drawn text, NULL for normal operation - internal
    * \param lpMatrix transformation matrix to use for text, NULL for none
    */
   virtual void drawStringClipped (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nXOffset, long nYOffset, float fKerning = 0, long nCursorPos = -1,
                                   KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, long nTextEncodingOverride = -1,
                                   unsigned long *lpPrevChar = NULL, long *lpEndXOffset = NULL, KMatrix *lpMatrix = NULL) = 0;

   /**
    * Get X coordinate of the specified character of interest in a string that is drawn clipped within a box
    *
    * \param lpszText text to look character up in
    * \param dx1 leftmost X coordinate of box
    * \param dx2 rightmost X coordinate of box
    * \param nXOffset number of horizontal pixels to offset text by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCharPos index of character of interest in the string
    * \param nX1Char returned leftmost X coordinate at which the character would be drawn by drawStringClippedF() - may be outside of dx1..dx2 if clipped
    * \param nX2Char returned rightmost X coordinate at which the character would be drawn by drawStringClippedF() - may be outside of dx1..dx2 if clipped
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    * \param bSnapToEdges true to return coordinates of right edge of string if reached the end, false not to
    *
    * \return true if character was found and coordinate was returned, false if not
    */
   virtual bool getClippedCharCoord (const char *lpszText, long dx1, long dx2, long nXOffset, float fKerning, long nCharPos, long &nX1Char, long &nX2Char,
                                     long nTextEncodingOverride = -1, bool bSnapToEdges = true) = 0;

   /**
    * Get index of a character in a string that would be drawn under the specified X coordinate
    *
    * \param lpszText text to look character up in
    * \param dx1 leftmost X coordinate of box
    * \param dx2 rightmost X coordinate of box
    * \param nXOffset number of horizontal pixels to offset text by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nXChar X coordinate to pick character from
    * \param bSnapToEdges true to return the index of the first character for any pick coordinate left of the string, and the index of the last character
    *                     for any pick coordinate right of the string; false to return -1 if the pick coordinate isn't actually over a character
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    *
    * \return index of character in the string under the specified X position, -1 if none matches
    */
   virtual long getClippedCharPos (const char *lpszText, long dx1, long dx2, long nXOffset, float fKerning, long nXChar,
                                   bool bSnapToEdges = false, long nTextEncodingOverride = -1) = 0;

   /**
    * Draw specified string from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor to use for drawing the string
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    */
   virtual void drawStringFromLeftF (const char *lpszText, float dx, float dy, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1) = 0;

   /**
    * Draw specified string from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param lpszText text to be drawn
    * \param nCharCount number of characters, starting at the first one, to get width of
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor to use for drawing the string
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    */
   virtual void drawNCharStringFromLeftF (const char *lpszText, long nCharCount, float dx, float dy, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1) = 0;

   /**
    * Draw specified string so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRightF (const char *lpszText, float dx, float dy, float fKerning = 0) = 0;

   /**
    * Draw specified string so that it is centered horizontally between the specified coordinates
    *
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCenteredF (const char *lpszText, float dx1, float dx2, float dy, float fKerning = 0) = 0;

   /**
    * Draw specified integer value from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor to use for drawing the string
    */
   virtual void drawStringFromLeftF (long nValue, float dx, float dy, float fKerning = 0, float fScale = 1) = 0;

   /**
    * Draw specified integer value so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRightF (long nValue, float dx, float dy, float fKerning = 0) = 0;

   /**
    * Draw specified string so that it is centered horizontally between the specified coordinates
    *
    * \param nValue integer value to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCenteredF (long nValue, float dx1, float dx2, float dy, float fKerning = 0) = 0;

   /**
    * Draw specified string and clip drawing within a box
    *
    * \param lpszText text to be drawn
    * \param dx1 leftmost X coordinate of box to draw in
    * \param dy1 topmost Y coordinate of box to draw in
    * \param dx2 rightmost X coordinate of box to draw in, +1
    * \param dy2 bottomost Y coordinate of box to draw in, + 1
    * \param fXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none)
    * \param lpCursorCallBack method called back when the character of interest is drawn, with the font, current stage (K_STRCLIP_xxx) and the coordinates at which the character of interest is drawn
    * \param lpCallBackData user data passed back to the method called back when the character of interest is drawn
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    */
   virtual void drawStringClippedF (const char *lpszText, float dx1, float dy1, float dx2, float dy2, float fXOffset, float fYOffset, float fKerning = 0, long nCursorPos = -1,
                                    KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, long nTextEncodingOverride = -1) = 0;

   /**
    * Get X coordinate of the specified character of interest in a string that is drawn clipped within a box
    *
    * \param lpszText text to look character up in
    * \param dx1 leftmost X coordinate of box
    * \param dx2 rightmost X coordinate of box
    * \param fXOffset number of horizontal pixels to offset text by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCharPos index of character of interest in the string
    * \param fX1Char returned leftmost X coordinate at which the character would be drawn by drawStringClippedF() - may be outside of dx1..dx2 if clipped
    * \param fX2Char returned rightmost X coordinate at which the character would be drawn by drawStringClippedF() - may be outside of dx1..dx2 if clipped
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    * \param bSnapToEdges true to return coordinates of right edge of string if reached the end, false not to
    *
    * \return true if character was found and coordinate was returned, false if not
    */
   virtual bool getClippedCharCoordF (const char *lpszText, float dx1, float dx2, float fXOffset, float fKerning, long nCharPos, float &fX1Char, float &fX2Char,
                                      long nTextEncodingOverride = -1, bool bSnapToEdges = true) = 0;

   /**
    * Get index of a character in a string that would be drawn under the specified X coordinate
    *
    * \param lpszText text to look character up in
    * \param dx1 leftmost X coordinate of box
    * \param dx2 rightmost X coordinate of box
    * \param fXOffset number of horizontal pixels to offset text by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fXChar X coordinate to pick character from
    * \param bSnapToEdges true to return the index of the first character for any pick coordinate left of the string, and the index of the last character
    *                     for any pick coordinate right of the string; false to return -1 if the pick coordinate isn't actually over a character
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    *
    * \return index of character in the string under the specified X position, -1 if none matches
    */
   virtual long getClippedCharPosF (const char *lpszText, float dx1, float dx2, float fXOffset, float fKerning, float fXChar,
                                    bool bSnapToEdges = false, long nTextEncodingOverride = -1) = 0;

   /**
    * Check if specified character exists in the character table of this font
    *
    * \param nChar character to be checked
    *
    * \return true if character exists, false not to
    */
   virtual bool doesCharExist (unsigned long nChar) = 0;

   /**
    * Get extra line spacing to be used when drawing multiline text
    *
    * \return Y spacing
    */
   virtual float getYSpacing (void) = 0;

   /**
    * Set extra line spacing to be used when drawing multiline text
    *
    * \param fYSpacing Y spacing
    */
   virtual void setYSpacing (float fYSpacing) = 0;

   /**
    * Set extra kerning to be applied to the space character of this font
    *
    * \param fKerning extra kerning, in pixels
    */
   virtual void setSpaceAdjustment (float fKerning) = 0;

   /**
    * Get extra padding to use when calculating multiline text line widths. This is useful when drawing text with an outline or drop shadow
    *
    * \return extra padding
    */
   virtual float getMultilineHorizPadding (void) = 0;

   /**
    * Set extra padding to use when calculating multiline text line widths. This is useful when drawing text with an outline or drop shadow
    *
    * \param fHorizPadding extra padding
    */
   virtual void setMultilineHorizPadding (float fHorizPadding) = 0;

   /**
    * Draw specified string and interpret line breaks
    *
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    */
   virtual void drawMultiline (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning = 0, float fFontHeight = -1) = 0;

   /**
    * Get width of largest line that specified string would use when drawn with drawMultiline()
    *
    * \param lpszText text to get the width of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    *
    * \return width
    */
   virtual long getMultilineWidth (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning = 0, float fFontHeight = -1) = 0;

   /**
    * Get width of each line that specified string would use when drawn with drawMultiline()
    *
    * \param lpszText text to get the width of each line of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nArraySize maximum number of entries to be filled in the width array
    * \param fpfWidthArray array of widths for each line
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    *
    * \return number of lines that the specified text will actually use
    */
   virtual long getMultilineWidthArray (const char *lpszText, long dx1, long dx2, long dy, long nStyle, long nArraySize, float *fpfWidthArray, float fKerning = 0, float fFontHeight = -1) = 0;

   /**
    * Get height that specified string would use when drawn with drawMultiline()
    *
    * \param lpszText text to get height of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    *
    * \return height
    */
   virtual long getMultilineHeight (const char *lpszText, long dx1, long dx2, long dy, long nStyle, float fKerning = 0, float fFontHeight = -1) = 0;

   /**
    * Draw specified string and interpret line breaks
    *
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy1 Y coordinate of the top of the box to draw text in
    * \param dy2 Y coordinate of the bottom of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none)
    * \param lpCursorCallBack method called back when the character of interest is drawn, with the font, current stage (K_STRCLIP_xxx) and the coordinates at which the character of interest is drawn
    * \param lpCallBackData user data passed back to the method called back when the character of interest is drawn
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    */
   virtual void drawMultilineClipped (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning = 0,
                                      long nCursorPos = -1, KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, float fFontHeight = -1) = 0;

   /**
    * Get coordinates of the specified character of interest in a multi-line string that is drawn clipped within a box
    *
    * \param lpszText text to look character up in
    * \param dx1 X coordinate of the left of the box that text would be drawn in
    * \param dx2 X coordinate of the right of the box that text would be drawn in
    * \param dy1 Y coordinate of the top of the box that text would be drawn in
    * \param dy2 Y coordinate of the bottom of the box that text would be drawn in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none)
    * \param nX1Char returned X coordinate of top, left of clipped box in which character of interest would be drawn, when found
    * \param nY1Char returned Y coordinate of top, left of clipped box in which character of interest would be drawn, when found
    * \param nX2Char returned X coordinate of bottom, right of clipped box in which character of interest would be drawn, +1, when found
    * \param nY2Char returned Y coordinate of bottom, right of clipped box in which character of interest would be drawn, +1, when found
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    *
    * \return true if character was found, false if not
    */
   virtual bool getMultilineClippedCharCoord (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning,
                                              long nCursorPos, long &nX1Char, long &nY1Char, long &nX2Char, long &nY2Char, float fFontHeight = -1) = 0;

   /**
    * Get index of a character in a multi-line string that would be drawn under the specified coordinates
    *
    * \param lpszText text to look character up in
    * \param dx1 X coordinate of the left of the box that text would be drawn in
    * \param dx2 X coordinate of the right of the box that text would be drawn in
    * \param dy1 Y coordinate of the top of the box that text would be drawn in
    * \param dy2 Y coordinate of the bottom of the box that text would be drawn in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nXChar X coordinate to pick character from
    * \param nYChar Y coordinate to pick character from
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    *
    * \return index of character in the string under the specified X position, -1 if none matches
    */
   virtual long getMultilineClippedCharPos (const char *lpszText, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning,
                                            long nXChar, long nYChar, float fFontHeight = -1) = 0;


   /**
    * Draw specified string and interpret line breaks
    *
    * \param nTextNodes number of nodes to draw
    * \param lpTextNode nodes to draw
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    * \param nMaxDrawnChars maximum number of characters to draw, starting with the first one; -1 to draw all characters in all nodes if possible (default)
    * \param r overall red tint color
    * \param g overall green tint color
    * \param b overall blue tint color
    * \param a overall alpha tint color
    * \param lpTextPixelShader shader to use for drawing text, NULL for none
    * \param lpMatrix transformation matrix to use for text, NULL for none
    */
   static void drawMultilineNodes (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dx2, long dy, long nStyle = TEXTSTYLE_LEFT, float fKerning = 0,
                                   long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0, long nMaxDrawnChars = -1,
                                   float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, KPixelShader *lpTextPixelShader = NULL, KMatrix *lpMatrix = NULL);

   /**
    * Get width of largest line that specified string would use when drawn with drawMultiline()
    *
    * \param nTextNodes number of nodes
    * \param lpTextNode nodes containing text to get the width of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    *
    * \return width
    */
   static long getMultilineNodesWidth (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dx2, long dy, long nStyle = TEXTSTYLE_LEFT, float fKerning = 0,
                                       long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0);

   /**
    * Get width of each line that specified string would use when drawn with drawMultiline()
    *
    * \param nTextNodes number of nodes
    * \param lpTextNode nodes containing text to get the width of each line of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nArraySize maximum number of entries to be filled in the width array
    * \param fpfWidthArray array of widths for each line
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    *
    * \return number of lines that the specified text will actually use
    */
   static long getMultilineNodesWidthArray (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dx2, long dy, long nStyle, long nArraySize, float *fpfWidthArray, float fKerning = 0,
                                            long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0);

   /**
    * Get height that specified string would use when drawn with drawMultiline()
    *
    * \param nTextNodes number of nodes
    * \param lpTextNode nodes containing text to get the height of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    *
    * \return height
    */
   static long getMultilineNodesHeight (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dx2, long dy, long nStyle = TEXTSTYLE_LEFT, float fKerning = 0,
                                        long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0);

   /**
    * Draw specified string, interpret line breaks, and clip it within a box
    *
    * \param nTextNodes number of nodes to draw
    * \param lpTextNode nodes to draw
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy1 Y coordinate of the top of the box to draw text in
    * \param dy2 Y coordinate of the bottom of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none)
    * \param lpCursorCallBack method called back when the character of interest is drawn, with the font, current stage (K_STRCLIP_xxx) and the coordinates at which the character of interest is drawn
    * \param lpCallBackData user data passed back to the method called back when the character of interest is drawn
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    * \param nMaxDrawnChars maximum number of characters to draw, starting with the first one; -1 to draw all characters in all nodes if possible (default)
    * \param r overall red tint color
    * \param g overall green tint color
    * \param b overall blue tint color
    * \param a overall alpha tint color
    * \param lpTextPixelShader shader to use for drawing text, NULL for none
    * \param lpMatrix transformation matrix to use for text, NULL for none
    */
   static void drawMultilineNodesClipped (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset, float fKerning = 0,
                                          long nCursorPos = -1, KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL,
                                          long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0, long nMaxDrawnChars = -1,
                                          float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, KPixelShader *lpTextPixelShader = NULL, KMatrix *lpMatrix = NULL);

   /**
    * Get coordinates of the specified character of interest in a multi-line string that is drawn clipped within a box
    *
    * \param nTextNodes number of nodes to draw
    * \param lpTextNode nodes to draw
    * \param dx1 X coordinate of the left of the box that text would be drawn in
    * \param dx2 X coordinate of the right of the box that text would be drawn in
    * \param dy1 Y coordinate of the top of the box that text would be drawn in
    * \param dy2 Y coordinate of the bottom of the box that text would be drawn in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixel to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none)
    * \param nX1Char returned X coordinate of top, left of clipped box in which character of interest would be drawn, when found
    * \param nY1Char returned Y coordinate of top, left of clipped box in which character of interest would be drawn, when found
    * \param nX2Char returned X coordinate of bottom, right of clipped box in which character of interest would be drawn, +1, when found
    * \param nY2Char returned Y coordinate of bottom, right of clipped box in which character of interest would be drawn, +1, when found
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    *
    * \return true if character was found, false if not
    */
   static bool getMultilineNodesClippedCharCoord (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset,
                                                  float fKerning, long nCursorPos, long &nX1Char, long &nY1Char, long &nX2Char, long &nY2Char,
                                                  long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0);

   /**
    * Get index of a character in a multi-line string that would be drawn under the specified coordinates
    *
    * \param nTextNodes number of nodes to draw
    * \param lpTextNode nodes to draw
    * \param dx1 X coordinate of the left of the box that text would be drawn in
    * \param dx2 X coordinate of the right of the box that text would be drawn in
    * \param dy1 Y coordinate of the top of the box that text would be drawn in
    * \param dy2 Y coordinate of the bottom of the box that text would be drawn in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nXChar X coordinate to pick character from
    * \param nYChar Y coordinate to pick character from
    * \param nParamSet parameter set to use for operations on nodes
    * \param fHorizPadding extra padding
    * \param fYSpacing Y spacing
    *
    * \return index of character in the string under the specified X position, -1 if none matches
    */
   static long getMultilineNodesClippedCharPos (long nTextNodes, KTextFaceNode *lpTextNode, long dx1, long dy1, long dx2, long dy2, long nStyle, long nXOffset, long nYOffset,
                                                float fKerning, long nXChar, long nYChar,
                                                long nParamSet = KTEXTOP_PARAM0, float fHorizPadding = 0, float fYSpacing = 0);

   /**
    * Parse color in #[aa]rrggbb format
    *
    * \param color color in text format
    * \param fColor returned color as 4 floats ranging 0..1
    */
   static void parseColor (const char *color, float *fColor);

   /**
    * Attempt to detect the text encoding of the stream by reading the Byte Order Mark (BOM)
    *
    * \param lpString string to check BOM in
    * \param nStrBytes length of the string, in bytes
    * \param nTextEncoding returned encoding, if a valid byte order mark was found
    *
    * \return bool true if text encoding was detected and returned, false if not
    */
   static bool detectEncoding (const char *lpString, long nStrBytes, long &nTextEncoding);

   /**
    * Decode character at the specified index
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to decode character in
    * \param nStrIndex current index into the string (0=first character); will be updated
    * \param bMoveForward true to decode character and then move byte index forward to the next character; false to move byte index backward to the previous character and then decode it
    *
    * \return unicode character
    */
   static unsigned long decodeChar (long nTextEncoding, const char *lpString, long &nStrIndex, bool bMoveForward);

   /**
    * Encode character at the specified index, moving forward
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to encode character in
    * \param nStrIndex current index into the string (0=first character); will be updated
    * \param nChar character to be encoded
    *
    * \return unicode character
    */
   static void encodeChar (long nTextEncoding, char *lpString, long &nStrIndex, unsigned long nChar);

   /**
    * Count characters in the specified string
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to count characters in
    *
    * \return number of characters
    */
   static long countChars (long nTextEncoding, const char *lpString);

   /**
    * Count codes in the specified string
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to count codes in
    *
    * \return number of codes
    */
   static size_t getStringLen (long nTextEncoding, const char *lpString);

   /**
    * Get index into the specified string for the specified character position
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to examine
    * \param nPos character position to get string index for (0 .. countChars())
    *
    * \return string index
    */
   static long getIndexFromCharPos (long nTextEncoding, const char *lpString, long nPos);

   /**
    * Insert character in the specified string at the specified character position
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to insert character into
    * \param nPos character position to insert at (0 .. countChars())
    * \param nMaxStringBytes maximum number of bytes that the string can hold (including terminal 0 character)
    * \param nChar character to insert
    */
   static void insertCharAtPos (long nTextEncoding, char *lpString, long nPos, long nMaxStringBytes, unsigned long nChar);

   /**
    * Delete character from the specified string at the specified character position
    *
    * \param nTextEncoding encoding of the string's characters
    * \param lpString string to delete character from
    * \param nPos character position to delete at (0 .. countChars() - 1)
    */
   static void deleteCharAtPos (long nTextEncoding, char *lpString, long nPos);

   /**
    * Get the pixel shader to set for drawing outlines, if supported
    *
    * \return pixel shader to set, NULL if not supported
    */
   static KPixelShader *getOutlinePixelShader (void);

   /**
    * Get the pixel shader to set for drawing glyphs, if supported
    *
    * \return pixel shader to set, NULL if not supported
    */
   static KPixelShader *getGlyphPixelShader (void);

private:
   /**
    * Draw multiline text and/or calculate its height
    *
    * \param lpFontFace font to use for drawing
    * \param nTextEncoding encoding of the text's characters
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to center text in (also used to clip the text against, if clipping is enabled)
    * \param dy1 Y coordinate of top of the box to draw text in (also used to clip the text against, if clipping is enabled)
    * \param dx2 X coordinate of the right of the box to center text in (also used to clip the text against, if clipping is enabled)
    * \param dy2 Y coordinate of bottom of the box to draw text in (used to clip the text against, if clipping is enabled, otherwise ignored)
    * \param nTextStyle style to use for line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fTextKerning kerning to use
    * \param fFontHeight font height, in pixels
    * \param fYSpacing extra spacing to be added between lines, in pixels
    * \param nOperation operation to execute: 0=only calculate the height, 1=draw the characters, 2=draw with clipping, 3=get character coordinates, 4=get character position, 5=calculate width
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none); returned when getting the character position
    * \param lpCursorCallBack method called back when the character of interest is drawn, with the font, current stage (K_STRCLIP_xxx) and the coordinates at which the character of interest is drawn
    * \param lpCallBackData user data passed back to the method called back when the character of interest is drawn
    * \param fX1Char when getting character coordinates, returned X coordinate of top, left of clipped box in which character of interest would be drawn; X coordinate to check when getting character pos
    * \param fY1Char when getting character coordinates, returned Y coordinate of top, left of clipped box in which character of interest would be drawn; Y coordinate to check when getting character pos
    * \param fX2Char when getting character coordinates, returned X coordinate of bottom, right of clipped box in which character of interest would be drawn, +1
    * \param fY2Char when getting character coordinates, returned Y coordinate of bottom, right of clipped box in which character of interest would be drawn, +1
    * \param bCharFound when getting character coordinates, true if character was found
    * \param fHorizPadding horizontal padding to add when calculating the width of a line (useful to make room for font outlines and drop shadows)
    * \param nNumLines returned number of drawn text lines
    * \param nWidthArraySize maximum number of entries in the width array, if used
    * \param lpfWidthArray array of line widths populated by this method, considered only for operation 5 (calculate width), NULL for none
    *
    * \return Y coordinate of text line following the rendered text
    */
   static float blitMultilineText (KTextFace *lpFontFace, long nTextEncoding, const char *lpszText, float dx1, float dy1, float dx2, float dy2, long nTextStyle, long nXOffset, long nYOffset,
                                   float fTextKerning, float fFontHeight, float fYSpacing, long nOperation, long &nCursorPos,
                                   KTextFaceCursorCallBack lpCursorCallBack, void *lpCallBackData, float &fX1Char, float &fY1Char, float &fX2Char, float &fY2Char, bool &bCharFound,
                                   float fHorizPadding, long &nNumLines, long nWidthArraySize, float *lpfWidthArray);

   /**
    * Draw multiline text nodes and/or calculate their height
    *
    * \param nTextNodes number of nodes to draw
    * \param lpFontFace lpTextNode nodes to draw
    * \param dx1 X coordinate of the left of the box to center text in (also used to clip the text against, if clipping is enabled)
    * \param dy1 Y coordinate of top of the box to draw text in (also used to clip the text against, if clipping is enabled)
    * \param dx2 X coordinate of the right of the box to center text in (also used to clip the text against, if clipping is enabled)
    * \param dy2 Y coordinate of bottom of the box to draw text in (used to clip the text against, if clipping is enabled, otherwise ignored)
    * \param nTextStyle style to use for line breaks (TEXTSTYLE_xxx)
    * \param nXOffset number of horizontal pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param nYOffset number of vertical pixels to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
    * \param fTextKerning kerning to use
    * \param fYSpacing extra spacing to be added between lines, in pixels
    * \param nOperation operation to execute: 0=only calculate the height, 1=draw the characters, 2=draw with clipping, 3=get character coordinates, 4=get character position, 5=calculate width
    * \param nCursorPos index of character in the string that is of interest to the caller, for instance for drawing a cursor (-1 for none); returned when getting the character position
    * \param lpCursorCallBack method called back when the character of interest is drawn, with the font, current stage (K_STRCLIP_xxx) and the coordinates at which the character of interest is drawn
    * \param lpCallBackData user data passed back to the method called back when the character of interest is drawn
    * \param fX1Char when getting character coordinates, returned X coordinate of top, left of clipped box in which character of interest would be drawn; X coordinate to check when getting character pos
    * \param fY1Char when getting character coordinates, returned Y coordinate of top, left of clipped box in which character of interest would be drawn; Y coordinate to check when getting character pos
    * \param fX2Char when getting character coordinates, returned X coordinate of bottom, right of clipped box in which character of interest would be drawn, +1
    * \param fY2Char when getting character coordinates, returned Y coordinate of bottom, right of clipped box in which character of interest would be drawn, +1
    * \param bCharFound when getting character coordinates, true if character was found
    * \param fHorizPadding horizontal padding to add when calculating the width of a line (useful to make room for font outlines and drop shadows)
    * \param nNumLines returned number of drawn text lines
    * \param nWidthArraySize maximum number of entries in the width array, if used
    * \param lpfWidthArray array of line widths populated by this method, considered only for operation 5 (calculate width), NULL for none*
    * \param nParamSet parameter set to use for node ops (0..n)
    * \param nMaxDrawnChars maximum number of characters to draw, starting with the first one; -1 to draw all characters in all nodes if possible (default)
    * \param r overall red tint color
    * \param g overall green tint color
    * \param b overall blue tint color
    * \param a overall alpha tint color
    * \param lpTextPixelShader shader to use for drawing text, NULL for none
    * \param lpMatrix transformation matrix to use for text, NULL for none
    *
    * \return Y coordinate of text line following the rendered text
    */
   static float blitMultilineTextNodes (long nTextNodes, KTextFaceNode *lpTextNode, float dx1, float dy1, float dx2, float dy2, long nTextStyle,
                                        long nXOffset, long nYOffset, float fTextKerning, float fYSpacing, long nOperation,
                                        long &nCursorPos, KTextFaceCursorCallBack lpCursorCallBack, void *lpCallBackData,
                                        float &fX1Char, float &fY1Char, float &fX2Char, float &fY2Char, bool &bCharFound,
                                        float fHorizPadding, long &nNumLines, long nWidthArraySize, float *lpfWidthArray,
                                        long nParamSet, long nMaxDrawnChars, float r, float g, float b, float a, KPixelShader *lpTextPixelShader, KMatrix *lpMatrix);

   /**
    * Get node that holds the specified character in the line currently being processed
    *
    * \param nTextNodes number of nodes to draw
    * \param lpFontFace lpTextNode nodes to draw
    * \param nFirstLineNodeIdx index of first node to search
    * \param nLastLineNodeIdx index of last node to search, +1
    * \param nLineCharIdx index of character to find node of, in overall concatenated text line array, 0..length - 1
    *
    * \return node, NULL if not found
    */
   static KTextFaceNode *getNodeForLineIndex (long nTextNodes, KTextFaceNode *lpTextNode, long nFirstLineNodeIdx, long nLastLineNodeIdx, long nLineCharIdx);

   /**
    * Get string width for text potentially across multiple nodes
    *
    * \param nTextNodes number of nodes to check
    * \param lpFontFace lpTextNode nodes to check
    * \param nFirstLineNodeIdx index of first node to search
    * \param nLastLineNodeIdx index of last node to search, +1
    * \param nLineCharIdx index of first character in string, in overall concatenated text line array
    * \param nEndLineCharIdx index of last character in string, in overall concatenated text line array + 1
    * \param fTextKerning kerning to use
    * \param nPrevChar previous char when continuing to measure text, -1 for none
    * \param bMeasureRightPadding true to include the spacing necessary for the outline or drop shadow to the right of the last character, false not to
    *
    * \return width
    */
   static float getStringWidthForNodes (long nTextNodes, KTextFaceNode *lpTextNode, long nFirstLineNodeIdx, long nLastLineNodeIdx, long nLineCharIdx, long nEndLineCharIdx, float fTextKerning,
                                        unsigned long nPrevChar = -1, bool bMeasureRightPadding = true);

   /** Load shaders used for text rendering, if supported */
   static void loadShaders (void);

   /** Free all resources used by text rendering - internal */
   static void cleanup (void);

   /** Baseline for this font (-1 if undefined) */
   float _fBaseline;

   /** Number of classes in the element's hierarchy */
   long _nClasses;

   /** Classes in the font's hierarchy */
   char _szClass[K_TEXTFACE_MAXCLASSES][K_TEXTFACE_MAXCLASSNAMELEN];

   /** Batching buffer used for all font rendering */  
   static KBatch *g_lpTextBatch;

   /** true if shader support has been tested, and shaders loaded if supported */
   static bool g_bShadersChecked;

   /** Pixel shader for drawing outlines */
   static KPixelShader *g_lpOutlineShader;

   /** Pixel shader for drawing glyphs */
   static KPixelShader *g_lpGlyphShader;

   /** Buffer for multiline drawing */
   static unsigned long g_szTextLine[4096];
};

#endif   /* _KANJI_KTEXTFACE_H */
