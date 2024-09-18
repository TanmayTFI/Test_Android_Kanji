/**
 * Kanji rendering and I/O engine - bitmap text rendering class definition
 *
 * (c) 2010
 */

#ifndef  _KANJI_KTEXT_H
#define  _KANJI_KTEXT_H

/* Include definitions */
#include "KTextFace.h"

/** Maximum number of graphics per font */
#define  KTEXT_MAXPAGES    8

/** Number of buckets in the kerning pairs hashtable */
#define  _KTEXT_NKERNINGBUCKETS   4096

/** One entry in the character table */
typedef struct KFont {
   unsigned long _c;          /**< Character; 0 terminates the character table */
   short _x1;                 /**< X coordinate of top, left of character bitmap in graphic */
   short _y1;                 /**< Y coordinate of top, left of character bitmap in graphic */
   short _x2;                 /**< X coordinate of bottom, right of character bitmap in graphic */
   short _y2;                 /**< Y coordinate of bottom, right of character bitmap in graphic */
   short _w;                  /**< How much to increase screen X coordinate by, after drawing this character, in pixels */
   short _h;                  /**< Height of this character, in pixels */
   long _nGraphic;            /**< Index of graphic page that this character is on (only if initializeAsUnicode is called) */
   long _xOffset;             /**< Offset to be applied to screen X coordinate when drawing this character, in pixels (only if initializeAsUnicode is called) */
   long _yOffset;             /**< Offset to be applied to screen Y coordinate when drawing this character, in pixels (only if initializeAsUnicode is called) */
} KFont;

/** One entry in the extended character table */
typedef struct KFontExt {
   unsigned long _c;          /**< Character; 0 terminates the character table */
   float _x1;                 /**< X coordinate of top, left of character bitmap in graphic */
   float _y1;                 /**< Y coordinate of top, left of character bitmap in graphic */
   float _x2;                 /**< X coordinate of bottom, right of character bitmap in graphic */
   float _y2;                 /**< Y coordinate of bottom, right of character bitmap in graphic */
   float _w;                  /**< How much to increase screen X coordinate by, after drawing this character, in pixels */
   float _h;                  /**< Height of this character, in pixels */
   long _nGraphic;            /**< Index of graphic page that this character is on */
   float _xOffset;            /**< Offset to be applied to screen X coordinate when drawing this character, in pixels */
   float _yOffset;            /**< Offset to be applied to screen Y coordinate when drawing this character, in pixels */
} KFontExt;

/** One entry in the kerning pairs table */
typedef struct {
   unsigned long _c1;         /**< First character; 0 terminates the kerning pairs table */
   unsigned long _c2;         /**< Second character */
   float _fKerning;           /**< Kerning amount */
} KFontKerningPair;

/** Kerning pair */
class KTextKerningEntry : public KObjectListable {
public:
   /** First character in pair */
   unsigned long _c1;

   /** Second character in pair */
   unsigned long _c2;

   /** Kerning amount */
   float _fKerning;

   /** Next object in the same hash bucket */
   KTextKerningEntry *_lpHashNext;

   /** Index of hashtable bucket that this pair is in */
   long _nHash;
};

/* Forward declarations */
class KWindow;
class KWindowD3D;
class KWindowD3D9;
class KWindowD3D11;
class KWindowGL;
class KWindowGLES;
class KWindowGLES2;
class KTrueText;

/**
 * Class that models bitmap text rendering
 */

class KText : public KTextFace {
   friend class KWindow;
   friend class KWindowD3D;
   friend class KWindowD3D9;
   friend class KWindowD3D11;
   friend class KWindowGL;
   friend class KWindowGLES;
   friend class KWindowGLES2;
   friend class KTextFace;
   friend class KTrueText;

public:
   /**
    * Set text encoding for bitmapped fonts and enable _nGraphic, _xOffset and _yOffset in KFont.
    *
    * \param nTextEncoding encoding of all text passed to KText for drawing (K_ANSI, K_UTF8 or K_WCHAR)
    */
   static void initializeAsUnicode (long nTextEncoding);

   /**
    * Check if the supplied font face is a KText instance, and cast as KText if it is
    *
    * \param lpFontFace font face to check and cast as KTrueText if possible
    *
    * \return KText if font face is a KText instance, NULL otherwise
    */
   static KText *isKText (KTextFace *lpFontFace);

   /** Constructor */
   KText ();

   /**
    * Constructor
    *
    * \param lpszGraphicFileName name of file to load font graphic from
    * \param lpFontTable character table
    */
   KText (const char *lpszGraphicFileName, KFont *lpFontTable);

   /** Destructor */
   virtual ~KText ();

   /**
    * Get compatibility level for this font's rendering
    *
    * \return this font's compatibility level
    */
   virtual long getCompatibilityLevel (void) const;

   /**
    * Set compatibility level for this font's rendering
    * Level 0: default
    * Level 1: fix reported multiline text dimensions
    *
    * \param nLevel compatibility level (defaults to 0 to support older games)
    */
   virtual void setCompatibilityLevel (long nLevel);

   /**
    * Set character table for this font
    *
    * \param lpFontTable character table
    * \param bExtendedData true if _nGraphic, _xOffset and _yOffset are valid in the character table entries, false if not (default, for compatibility). Extended data is also
    *                      assumed to be present if initializeAsUnicode() was called
    */
   void setFontTable (KFont *lpFontTable, bool bExtendedData = false);

   /**
    * Set extended character table for this font
    *
    * \param lpFontTable extended character table
    * \param lpKerningTable kerning pairs table, NULL for none
    * \param nDefinedFontHeightPix font height in pixels, 0 to use the height of the first character in the font table
    */
   void setFontTableExt (KFontExt *lpFontTable, KFontKerningPair *lpKerningTable = NULL, long nDefinedFontHeightPix = 0);

   /**
    * Set graphic to use for drawing this font, using the coordinates in the character table
    *
    * \param lpFontGraphics new graphics
    * \param nPage index of graphics page being set, useful when a font with international characters is spread over multiple graphics
    */
   void setKGraphic (KGraphic *lpFontGraphics, long nPage = 0);

   /**
    * Load graphic with bitmapped characters corresponding to the font table, for this font
    *
    * \param lpszGraphicFileName name of file to load font graphic from
    * \param nPage index of graphics page being loaded, useful when a font with international characters is spread over multiple graphics
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the graphic, false not to
    */
   bool loadFontBitmap (const char *lpszGraphicFileName, long nPage = 0, bool bHiQuality = true, bool bGenerateMipMaps = false);

   /**
    * Load graphic with bitmapped characters corresponding to the font table, for this font, as seperate color and alpha images
    *
    * \param lpszColorFileName name of file to load color font graphic from
    * \param lpszAlphaFileName name of file to load alpha font graphic from
    * \param nPage index of graphics page being loaded, useful when a font with international characters is spread over multiple graphics
    * \param bHiQuality true to load as truecolor, false to load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the graphic, false not to
    */
   bool loadFontBitmapWithMask (const char *lpszColorFileName, const char *lpszAlphaFileName, long nPage = 0, bool bHiQuality = true, bool bGenerateMipMaps = false);

   /**
    * Load character table from a file
    *
    * \param lpszFontTableFileName name of file to load font table from
    * \param nGraphics returned number of graphics used by the font
    * \param nChars returned number of characters in the font table
    *
    * \return true for success, false for failure
    */
   bool loadFontTable (const char *lpszFontTableFileName, long &nGraphics, long &nChars);

   /**
    * Load a character table and font bitmaps
    *
    * \param lpszFontTableFileName name of file to load font table from
    * \param lpszGraphicFileName name of file to load first font graphic from (for instance font1.png); if this font uses more than one graphic, this
    *                            method will automatically load them all by increasing the number just before the file extension (font2.png, font3.png...)
    * \param bHiQuality true to load font graphics as truecolor, false to load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the font graphics, false not to
    *
    * \return true for success, false for failure
    */
   virtual bool loadFontTableAndBitmap (const char *lpszFontTableFileName, const char *lpszGraphicFileName, bool bHiQuality = false, bool bGenerateMipMaps = false);

   /**
    * Get graphic used for the bitmapped characters
    *
    * \param nPage index of graphics page to get
    *
    * \return bitmapped font graphic
    */
   KGraphic *getKGraphicPtr (long nPage = 0);

   /**
    * Set vertical offset to be applied to the screen coordinates when drawing all characters on a specified texture page
    *
    * \param nPage page to set vertical offset for
    * \param nYOffset new offset
    */
   void setYOffset (long nPage, long nYOffset);

   /**
    * Set additional kerning to be added when drawing all characters on a specified texture page
    *
    * \param nPage page to set global kerning for
    * \param fGlobalKerning new global kerning for this page
    */
   void setGlobalKerning (long nPage, float fGlobalKerning);
   
   /**
    * Get index of specified Unicode character, into the character table
    *
    * \param nCharacter character to get chartable index of
    *
    * \return character table index, -1 if not found
    */
   long getCharTableIndex (long nCharacter) const;

   /**
    * Get character table for this font
    *
    * \return character table
    */
   KFontExt *getFontTableExt (void);

   /**
    * Get encoding used for this font's text
    *
    * \return text encoding (K_ANSI, ...)
    */
   virtual long getTextEncoding (void) const;

   /**
    * Get height of this font, in pixels
    *
    * \return height in pixels
    */
   virtual long getHeightPix (void);

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
   virtual float getStringWidth (const char *lpszText, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1, unsigned long *lpPrevChar = NULL, bool bLastComponent = true);

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
   virtual float getNCharWidth (const char *lpszText, long nCharCount, float fKerning, float fScale = 1, long nTextEncodingOverride = -1);

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
   virtual float getCharWidth (unsigned long c, float fKerning);

   /**
    * Get horizontal offset of the specified character using this font, in pixels
    *
    * \param c character to get width of
    *
    * \return horizontal offset
    *
    * \private
    */
   virtual float getCharXOffset (unsigned long c);

   /**
    * Get height of the specified string using this font, in pixels
    *
    * \param lpszText to get height of
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    *
    * \return height
    */
   virtual float getStringHeight (const char *lpszText, long nTextEncodingOverride = -1);

   /**
    * Set tinting color when drawing text using this font
    *
    * \param lpColor 4 floats where to store red, green, blue and alpha components
    */
   virtual void getColor (float *lpColor);

   /**
    * Set tinting color when drawing text using this font
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   virtual void setColor (float r, float g, float b, float a);

   /**
    * Set tinting color when drawing text using this font - same as setColor(), present for source compatibility purposes
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   virtual void setBlitColor (float r, float g, float b, float a);

   /**
    * Set blending factor for this font. This is multiplied with the alpha component of the tinting color
    *
    * \param fBlend blending factor
    */
   virtual void setBlend (float fBlend);

   /**
    * Set blending mode to use when drawing this font
    *
    * \param nBlendingMode blending mode (K_ALPHA_xxx, as defined in KGraphic.h)
    */
   virtual void setAlphaMode (long nBlendingMode);

   /**
    * Set alpha blending operations for this font
    *
    * \param nSrcAlphaOp new source alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nDstAlphaOp new destination alpha blending op for the color channels (K_ALPHAOP_xxx)
    */
   virtual void setAlphaOp (eKanjiAlphaOp nSrcAlphaOp, eKanjiAlphaOp nDstAlphaOp);

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
                                    KMatrix *lpMatrix = NULL);

   /**
    * Draw specified string so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRight (const char *lpszText, long dx, long dy, float fKerning = 0);

   /**
    * Draw specified string so that it is centered horizontally between the specified coordinates
    *
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCentered (const char *lpszText, long dx1, long dx2, long dy, float fKerning = 0);

   /**
    * Draw specified integer value from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromLeft (long nValue, long dx, long dy, float fKerning = 0);

   /**
    * Draw specified integer value so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRight (long nValue, long dx, long dy, float fKerning = 0);

   /**
    * Draw specified integer value so that it is centered horizontally between the specified coordinates
    *
    * \param nValue value to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCentered (long nValue, long dx1, long dx2, long dy, float fKerning = 0);

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
                                   unsigned long *lpPrevChar = NULL, long *lpEndXOffset = NULL, KMatrix *lpMatrix = NULL);

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
                                     long nTextEncodingOverride = -1, bool bSnapToEdges = true);

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
                                   bool bSnapToEdges = false, long nTextEncodingOverride = -1);

   /**
    * Draw specified string from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor that will be used for drawing the string
    * \param nTextEncodingOverride text encoding for this string, -1 to use the default encoding set for all bitmap fonts - internal
    */
   virtual void drawStringFromLeftF (const char *lpszText, float dx, float dy, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1);

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
   virtual void drawNCharStringFromLeftF (const char *lpszText, long nCharCount, float dx, float dy, float fKerning = 0, float fScale = 1, long nTextEncodingOverride = -1);

   /**
    * Draw specified string so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param lpszText text to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRightF (const char *lpszText, float dx, float dy, float fKerning = 0);

   /**
    * Draw specified string so that it is centered horizontally between the specified coordinates
    *
    * \param lpszText text to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCenteredF (const char *lpszText, float dx1, float dx2, float dy, float fKerning = 0);

   /**
    * Draw specified integer value from the specified screen coordinates, drawing the first character at the coordinates and then
    * moving left to right
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor that will be used for drawing the string
    */
   virtual void drawStringFromLeftF (long nValue, float dx, float dy, float fKerning = 0, float fScale = 1);

   /**
    * Draw specified integer value so that the last character is drawn on the left of the specified screen coordinates
    *
    * \param nValue value to be drawn
    * \param dx X coordinate of top, right of last character
    * \param dy Y coordinate of top, left of last character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringFromRightF (long nValue, float dx, float dy, float fKerning = 0);

   /**
    * Draw specified string so that it is centered horizontally between the specified coordinates
    *
    * \param nValue integer value to be drawn
    * \param dx1 X coordinate of the left of the box to center text in
    * \param dx2 X coordinate of the right of the box to center text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    */
   virtual void drawStringCenteredF (long nValue, float dx1, float dx2, float dy, float fKerning = 0);

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
                                    KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, long nTextEncodingOverride = -1);

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
                                      long nTextEncodingOverride = -1, bool bSnapToEdges = true);

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
                                    bool bSnapToEdges = false, long nTextEncodingOverride = -1);

   /**
    * Check if specified character exists in the character table of this font
    *
    * \param nChar character to be checked
    *
    * \return true if character exists, false not to
    */
   virtual bool doesCharExist (unsigned long nChar);

   /**
    * Get extra line spacing to be used when drawing multiline text
    *
    * \return Y spacing
    */
   virtual float getYSpacing (void);

   /**
    * Set extra line spacing to be used when drawing multiline text
    *
    * \param fYSpacing Y spacing
    */
   virtual void setYSpacing (float fYSpacing);

   /**
    * Set extra kerning to be applied to the space character of this font
    *
    * \param fKerning extra kerning, in pixels
    */
   virtual void setSpaceAdjustment (float fKerning);

   /**
    * Get extra padding to use when calculating multiline text line widths. This is useful when drawing text with an outline or drop shadow
    *
    * \return extra padding
    */
   virtual float getMultilineHorizPadding (void);

   /**
    * Set extra padding to use when calculating multiline text line widths. This is useful when drawing text with an outline or drop shadow
    *
    * \param fHorizPadding extra padding
    */
   virtual void setMultilineHorizPadding (float fHorizPadding);

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
   virtual void drawMultiline (const char *lpszText, long dx1, long dx2, long dy, long nStyle = TEXTSTYLE_LEFT, float fKerning = 0, float fFontHeight = -1);

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
   virtual long getMultilineWidth (const char *lpszText, long dx1, long dx2, long dy, long nStyle = TEXTSTYLE_LEFT, float fKerning = 0, float fFontHeight = -1);

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
   virtual long getMultilineWidthArray (const char *lpszText, long dx1, long dx2, long dy, long nStyle, long nArraySize, float *fpfWidthArray, float fKerning = 0, float fFontHeight = -1);

   /**
    * Get height that specified string would use when drawn with drawMultiline()
    *
    * \param lpszText text to get the height of
    * \param dx1 X coordinate of the left of the box to draw text in
    * \param dx2 X coordinate of the right of the box to draw text in
    * \param dy Y coordinate of top of the box to draw text in
    * \param nStyle how to handle line breaks (TEXTSTYLE_xxx)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fFontHeight font height to use for determining line height, in pixels, -1 to automatically determine
    *
    * \return height
    */
   virtual long getMultilineHeight (const char *lpszText, long dx1, long dx2, long dy, long nStyle = TEXTSTYLE_LEFT, float fKerning = 0, float fFontHeight = -1);

   /**
    * Draw specified string, interpret line breaks, and clip it within a box
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
                                      long nCursorPos = -1, KTextFaceCursorCallBack lpCursorCallBack = NULL, void *lpCallBackData = NULL, float fFontHeight = -1);

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
                                              long nCursorPos, long &nX1Char, long &nY1Char, long &nX2Char, long &nY2Char, float fFontHeight = -1);

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
                                            long nXChar, long nYChar, float fFontHeight = -1);
   
private:
   /**
    * Get kerning amount for specified character pair
    *
    * \param c1 first character (-1 for none)
    * \param c2 second character
    *
    * \return kerning amount
    */
   float getKerningForCharPair (unsigned long c1, unsigned long c2) const;

   /**
    * Create extended table from regular font table
    *
    * \param lpFontTable regular table
    * \param bUseExtendedData true to use _nGraphic, _xOffset and _yOffset, false not to (for compatibility with old games)
    *
    * \return extended table
    */
   KFontExt *createExtendedFontTable (KFont *lpFontTable, bool bUseExtendedData);

   /** Free all resources used by bitmap text rendering - internal */
   static void cleanup (void);

   /**
    * Increment index in specified filename, for instance going from font1.png to font2.png, font3.png, etc.
    *
    * \param lpszFileName filename to increase index in (with at least room for K_MAXPATH characters)
    *
    * \return true if successful, false if there wasn't enough room in the filename buffer
    */
   static bool incrementIndexInName (char *lpszFileName);

   /** true if character table was allocated internally, false if it was supplied by the game */
   bool _bInternalCharTable;

   /** Character table */
   KFontExt *_lpCharTable;

   /** Compatibility level for this font; enables certain features and bugfixes if the game supports them */
   long _nCompatibilityLevel;

   /** Defined for height in pixels, 0 if none */
   long _nDefinedFontHeightPix;

   /** Number of character pages */
   long _nCharPages;

   /** For each character page, bits 11-31 of the character number, for fast lookups */
   long *_nPageStart;

   /** Character pages, for fast lookups */
   long **_nCharPage;

   /** Global blending factor when drawing this font's characters */
   float _fBlend;

   /** Source blending mode when drawing this font's characters */
   eKanjiAlphaOp _nSrcAlphaMode;

   /** Destination blending mode when drawing this font's characters */
   eKanjiAlphaOp _nDstAlphaMode;

   /** Tinting color */
   float _fFontColor[4];

   /** Extra space, to be added to the space character */
   float _fSpaceKerning;

   /** Additional X line padding when drawing multiline text */
   float _fMultilineXPadding;

   /** Additional Y spacing when drawing multiline text */
   float _fMultilineYSpacing;

   /** Graphics holding the bitmap characters for this font */
   KGraphic *_lpGraphic[KTEXT_MAXPAGES];

   /** true if each graphic was allocated internally and will need to be freed, false if provided by the game */
   bool _bInternalGraphic[KTEXT_MAXPAGES];

   /** Additional, global Y offset for all characters in each graphics page */
   float _fPageYOffset[KTEXT_MAXPAGES];

   /** Additional, global kerning for all characters in each graphics page */
   float _fGlobalKerning[KTEXT_MAXPAGES];

   /** Kerning pairs hashtable */
   KTextKerningEntry **_lpKerningHashFirst;

   /** Array of all hashed kerning pairs */
   KTextKerningEntry *_lpKerningEntries;

   /** Encoding of all strings provided to KText for drawing */
   static long g_nKTextEncoding;

   /** true if _nGraphic, _xOffset and _yOffset are globally valid in the character table entries */
   static bool g_bExtendedCharData;

   /** Buffer for composing filenames */
   static char g_szGraphicFileNameBuf[K_MAXPATH];
};

#endif   /* _KANJI_KTEXT_H */
