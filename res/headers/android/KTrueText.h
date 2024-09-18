/**
 * Kanji rendering and I/O engine - vector text rendering class definition
 *
 * (c) 2010
 */

#ifndef  _KANJI_KTRUETEXT_H
#define  _KANJI_KTRUETEXT_H

/* Include definitions */
#include "KTextFace.h"
#include "KResource.h"
#include "KWindow.h"
#include <vector>

/** Number of buckets in the kerning pairs hashtable */
#define  _KTRUETEXT_NKERNINGBUCKETS   4096

/* Definitions for source compatibility purposes */
#define  KTRUETEXT_ANSI K_ANSI
#define  KTRUETEXT_UTF8 K_UTF8

/* Forward declarations */
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_StreamRec_  *FT_Stream;
typedef struct FT_FaceRec_    *FT_Face;
typedef struct KTrueTextChar KTrueTextChar;
class KWindowGL;
class KMiscTools;

/** One color in gradient array */
typedef struct {
   /** Position of this color, 0..1 */
   float fPosition;

   /** Red, green, blue and alpha value of this color (0..1) */
   float fColor[4];
} KTrueTextGradientColor;

/** Kerning pair */
class KTrueTextKerningEntry : public KObjectListable {
public:
   /** First character in pair */
   unsigned long _c1;

   /** Second character in pair */
   unsigned long _c2;

   /** Kerning amount */
   float _fKerning;

   /** Next object in the same hash bucket */
   KTrueTextKerningEntry *_lpHashNext;

   /** Index of hashtable bucket that this pair is in */
   long _nHash;
};

/**
 * Class that models vector text rendering
 */

class KTrueText : public KTextFace {
   friend class KWindowGL;
   friend class KMiscTools;

public:
   /**
    * Let vector font renderer know what characters are actually present in the game's string table. This method must be called before any
    * vector font is instanciated, and enables rendering of internationalized text.
    *
    * \param nTableEntries number of entries in the string table
    * \param lpszStrTable string table: array of all the strings that the game will display
    * \param nTextEncoding encoding of the string table's entries as well as all text passed to KTrueText for drawing (K_ANSI, K_UTF8 or K_WCHAR)
    *
    * \return true for success, false if the number of characters was higher than KTRUETEXT_MAXCHARS and some characters were dropped
    */
   static bool initializeAsUnicode (unsigned long nTableEntries, const char *lpszStrTable[], long nTextEncoding);

   /**
    * Check if the supplied font face is a KTrueText instance, and cast as KTrueText if it is
    *
    * \param lpFontFace font face to check and cast as KTrueText if possible
    *
    * \return KTrueText if font face is a KTrueText instance, NULL otherwise
    */
   static KTrueText *isKTrueText (KTextFace *lpFontFace);

   /**
    * Constructor
    *
    * \param lpszFileName name of file to load font face from
    */
   KTrueText (const char *lpszFileName = NULL);

   /** Destructor */
   virtual ~KTrueText ();

   /**
    * Get compatibility level for this font's rendering
    *
    * \return this font's compatibility level
    */
   virtual long getCompatibilityLevel (void) const;

   /**
    * Set compatibility level for this font's rendering
    * Level 0: default
    * Level 1: fix reported multiline text dimensions; enable new Freetype bytecode interpreter and hinter (much nicer rendering using a hinter based on expired patents)
    * Level 2: fix bug in reported font height
    *
    * \param nLevel compatibility level (defaults to 0 to support older games)
    */
   virtual void setCompatibilityLevel (long nLevel);

   /**
    * Check whether this font ignores reserving space for characters that are taller than the top of the font box, both for alignment and for reporting the font height
    *
    * \return true if ignored, false if not (default)
    */
   bool getIgnoreTopSpacing (void);

   /**
    * Enable or disable not reserving space for characters that are taller than the top of the font box, both for alignment and for reporting the font height.
    *
    * \param bEnable true to enable, false to disable (default)
    */
   void setIgnoreTopSpacing (bool bEnable);

   /**
    * Load a new font face
    *
    * \param lpszFileName name of file to load font face from
    * \param bUseKerningPairs true to use kerning pairs information for rendering, if available, false not to (default)
    * \param bHiQuality true to generate font graphics as truecolor, false generate load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the font graphics, false not to
    */
   bool loadFontFile (const char *lpszFileName, bool bUseKerningPairs = false, bool bHiQuality = true, bool bGenerateMipMaps = false);

   /**
    * Load a new font face from a memory buffer
    *
    * \param lpBuffer memory buffer to decode font from
    * \param nBufferSize size of memory buffer containing font, in bytes
    * \param bUseKerningPairs true to use kerning pairs information for rendering, if available, false not to (default)
    * \param bHiQuality true to generate font graphics as truecolor, false generate load as 15 or 16 bits/pixel
    * \param bGenerateMipMaps true to generate mipmaps for the font graphics, false not to
    */
   bool loadFontFileFromPtr (const unsigned char *lpBuffer, long nBufferSize, bool bUseKerningPairs = false, bool bHiQuality = true, bool bGenerateMipMaps = false);

   /**
    * Enable or disable deferred rendering. When enabled, the font is re-rendered if its size, outline, drop shadow or gradient have been changed, when a
    * character is drawn; if disabled (default), the font is re-rendered immediately when a parameter is changed, so as to avoid a delay when initially drawing.
    * It is recommended to enable deferred rendering while changing multiple parameters (outline, drop shadow..) in a row so as to avoid multiple renders
    *
    * \param bEnable true to enable, false to disable
    */
   void setDeferredRendering (bool bEnable);

   /**
    * Get currently selected height of this font, in pixels
    *
    * \return height in pixels
    */
   virtual long getHeightPix (void);

   /**
    * Set current height of this font, in pixels
    *
    * \param nHeightPix new height in pixels
    */
   void setHeightPix (long nHeightPix);

   /**
    * Set current height of this font, in points for 96 dots per pixel
    *
    * \param nHeightPt new height in points
    */
   void setHeightPt (long nHeightPt);

   /**
    * Get this font's ascender, in pixels
    *
    * \return ascender in pixels
    */
   long getAscenderPix (void);
   
   /**
    * Get this font's descender, in pixels
    *
    * \return descender in pixels
    */
   long getDescenderPix (void);

   /**
    * Enable rendering this font as seperate glyph and outline channels to be drawn with a shader
    *
    * \param bEnable true to enable rendering seperate channels and draw this font with a shader, false for default mode
    * \param fOutlineThickness outline thickness
    */
   void setShaderRendering (bool bEnable, float fOutlineThickness);

   /**
    * Set color gradient to be applied to this font. When setShaderRendering() is used to enable rendering to seperate channels, this method is ignored.
    *
    * \param nColors number of colors
    * \param lpColor array of colors
    */
   void setGradient (long nColors, const KTrueTextGradientColor *lpColor);

   /**
    * Set outline to be applied to this font. When setShaderRendering() is used to enable rendering to seperate channels, this method is ignored.
    *
    * \param nSize size of outline in pixels (0 to disable)
    * \param r red component of outline color, 0..1
    * \param g green component of outline color, 0..1
    * \param b blue component of outline color, 0..1
    * \param fBlend blending factor of outline, 0..1
    */
   void setOutline (long nSize, float r, float g, float b, float fBlend);

   /**
    * Set drop shadow to be applied to this font. When setShaderRendering() is used to enable rendering to seperate channels, this method is ignored.
    *
    * \param nSize size of drop shadow in pixels (0 to disable)
    * \param nDistance distance of drop shadow from glyphs, in pixels
    * \param fAngle drop shadow angle, in degrees
    * \param r red component of drop shadow color, 0..1
    * \param g green component of drop shadow color, 0..1
    * \param b blue component of drop shadow color, 0..1
    * \param fBlend blending factor of drop shadow, 0..1
    */
   void setDropShadow (long nSize, long nDistance, float fAngle, float r, float g, float b, float fBlend);

   /**
    * Save this font, at the currently set height, as a font table and set of bitmap graphics that can be re-read by KText::loadFontTableAndBitmap
    *
    * \param lpszFontTableFileName filename to use for saving font table (for instance font.bin)
    * \param lpszGraphicFileName filename to use for first graphic (for instance font1.png); if this font uses more than one graphic, this
    *                            method will automatically save them all by increainge the number just before the file extension (font2.png, font3.png...)
    * \param nImageFormat image format (K_IMAGE_BMP, K_IMAGE_TGA or K_IMAGE_PNG, as the format needs to support an alpha channel)
    * \param bPackChars true to pack characters in graphics as much as possible, false to align them, so as to be able to apply art package effects on them (default)
    *
    * \return true for success, false for failure
    */
   bool saveFontTableAndBitmap (const char *lpszFontTableFileName, const char *lpszGraphicFileName, long nImageFormat = K_IMAGE_BMP, bool bPackChars = false);

   /**
    * Free the set of textures currently used for drawing this font. The textures will be re-created the next time a string is drawn with
    * this font
    */
   void freeGraphics (void);
   
   /**
    * Check if the font will be re-rendered the next time it is drawn
    *
    * \return true if a render if pending, false if not
    */
   bool isRenderingNeeded (void);

   /**
    * Get encoding used for this font's text
    *
    * \return text encoding (K_ANSI, ...)
    */
   virtual long getTextEncoding (void) const;

   /**
    * Get width of the specified string using this font, in pixels
    *
    * \param lpszText to get width of
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param fScale scaling factor to use for drawing the string
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
    * \param fScale scaling factor to use for drawing the string
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
    * \param fScale scaling factor to use for drawing the string
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
    * \param fScale scaling factor to use for drawing the string
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
    * \param nYOffset number of vertical pixel to offset drawing by (negative to skip pixels), starting at the first pixel of the first character
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

   /**
    * Measure the minimum dimensions of a pixel array to draw the specified string in
    *
    * \param lpUtf32Text string to measure (as 32-bit unicodes)
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nArrayWidth returned width of the array, in pixels
    * \param nArrayHeight returned height of the array, in pixels
    * \param nXAdvance returned number of pixels to advance horizontally by, after drawing this string, to continue drawing text
    * \param nYAdvance returned number of pixels to advance vertically by, after drawing this string, to continue drawing text
    * \param nRenderMode rendering mode
    */
   void getArraySizeForString (unsigned long *lpUtf32Text, float fKerning, long &nArrayWidth, long &nArrayHeight, long &nXAdvance, long &nYAdvance, long nRenderMode = 0);

   /**
    * Draw specified string to a pixel array
    *
    * \param lpUtf32Text string to draw (as 32-bit unicodes)
    * \param lpPixelArray pixel array to draw to
    * \param dx X coordinate of top, left of first character
    * \param dy Y coordinate of top, left of first character
    * \param fKerning additional spacing, or contraction (if the value is negative) between characters
    * \param nArrayWidth width of the array, in pixels
    * \param nArrayHeight height of the array, in pixels
    * \param nRenderMode rendering mode
    */
   void drawStringToArray (unsigned long *lpUtf32Text, unsigned int *lpPixelArray, long dx, long dy, float fKerning, long nArrayWidth, long nArrayHeight, long nRenderMode = 0); 

private:
   /** Check if this font's face needs to be reloaded, and reload it if it does */
   void checkFontFace (void);

   /**
    * Render this font's characters to a set of textures, and build char table
    *
    * \param lpszGraphicFileName filename of first graphic to save rendered characters to, NULL to only render to KGraphic for displaying on screen
    * \param nImageFormat format to use for saving bitmaps (K_IMAGE_xxx)
    * \param bPackChars true to pack characters in graphics as much as possible, false to align them, so as to be able to apply art package effects on them
    *
    * \return true for success, false for failure
    */
   bool render (const char *lpszGraphicFileName, long nImageFormat, bool bPackChars);
   
   /**
    * Copy font outline into the specified pixel buffer
    *
    * \param lpSrcPixel source pixels
    * \param x1 X coordinate of top, left pixel in source
    * \param y1 Y coordinate of top, left pixel in source
    * \param x2 X coordinate of bottom, right pixel in source, + 1
    * \param y2 Y coordinate of bottom, right pixel in source, + 1
    * \param lpDstPixel destination pixel buffer
    * \param dx X coordinate of top, left pixel in destination
    * \param dy Y coordinate of top, left pixel in destination
    * \param fBlend blending factor, 0..1
    * \param nSize width and height in pixels of source and destination pixel buffers
    */
   void copyRect (const unsigned int *lpSrcPixel, long x1, long y1, long x2, long y2, unsigned int *lpDstPixel, long dx, long dy, float fBlend, long nSize);

   /**
    * Copy font outline into the specified pixel buffer
    *
    * \param lpSrcPixel source pixels
    * \param x1 X coordinate of top, left pixel in source
    * \param y1 Y coordinate of top, left pixel in source
    * \param x2 X coordinate of bottom, right pixel in source, + 1
    * \param y2 Y coordinate of bottom, right pixel in source, + 1
    * \param lpDstPixel destination pixel buffer
    * \param dx X coordinate of top, left pixel in destination
    * \param dy Y coordinate of top, left pixel in destination
    * \param r red component of solid color to use for outline, 0..1
    * \param g green component of solid color to use for outline, 0..1
    * \param b blue component of solid color to use for outline, 0..1
    * \param fBlend blending factor, 0..1
    * \param nSize width and height in pixels of source and destination pixel buffers
    */
   void copyOutline (const unsigned int *lpSrcPixel, long x1, long y1, long x2, long y2, unsigned int *lpDstPixel, long dx, long dy, float r, float g, float b, float fBlend, long nSize);

   /**
    * Blur font outline in the specified pixel buffer
    *
    * \param nSize width and height in pixels of pixel buffer
    */
   void blurOutline (unsigned int *lpDstPixel, long nSize);

   /**
    * Get index of specified Unicode character, into the character table
    *
    * \param nCharacter character to get chartable index of
    *
    * \return character table index, -1 if not found
    */
   long getCharTableIndex (long nCharacter) const;

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
    * Callback method used by FreeType to read from a data stream, when decoding from a resource
    *
    * \param lpStream source stream
    * \param nOffset byte offset to read at
    * \param lpBuffer buffer where to store the read contents
    * \param nBytes number of bytes to be read
    *
    * \return number of bytes effectively read
    */
   static unsigned long userReadResource (FT_Stream lpStream, unsigned long nOffset, unsigned char *lpBuffer, unsigned long nBytes);

   /**
    * Callback method used by FreeType to close an open data stream, when decoding from a resource
    *
    * \param lpStream source stream
    */
   static void userCloseResource (FT_Stream lpStream);

   /**
    * Callback method used by FreeType to read from a data stream, when decoding from a memory buffer
    *
    * \param lpStream source stream
    * \param nOffset byte offset to read at
    * \param lpBuffer buffer where to store the read contents
    * \param nBytes number of bytes to be read
    *
    * \return number of bytes effectively read
    */
   static unsigned long userReadMemory (FT_Stream lpStream, unsigned long nOffset, unsigned char *lpBuffer, unsigned long nBytes);

   /**
    * Callback method used by FreeType to close an open data stream, when decoding from a memory buffer
    *
    * \param lpStream source stream
    */
   static void userCloseMemory (FT_Stream lpStream);

   /** Free all resources used by vector text rendering - internal */
   static void cleanup (void);

   /** Count the maximum number of character pages that we need to create, for fast lookups */
   static void countMaxCharPages (void);

   /** FreeType stream, used for reading the font bytes through KResource */
   FT_Stream _lpStream;
   
   /** FreeType font face */
   FT_Face _lpFontFace;

   /** KResource for reading font file */
   KResource _resFontFile;

   /** Font data, when reading from a memory buffer and not a KResource */
   unsigned char *_lpFontData;

   /** Size of font data, when reading from a memory buffer */
   unsigned long _nFontDataSize;

   /** Compatibility level for this font; enables certain features and bugfixes if the game supports them */
   long _nCompatibilityLevel;
   
   /** true to avoid reserving space for characters that go outside the top of the font box, both for alignment and for height reporting purposes */
   bool _bIgnoreTopSpacing;

   /** true to generate font graphics as truecolor, false generate load as 15 or 16 bits/pixel */
   bool _bHiQuality;

   /** true to generate mipmaps for the font graphics, false not to */
   bool _bGenerateMipMaps;

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

   /** Height set for this font (> 0 = height in pixels, < 0 = height in points, 0 = not set yet) */
   long _nSetHeight;

   /** true if re-rendering is deferred to when a draw operation occurs */
   bool _bDeferredRendering;

   /** true if this font's characters need to be rendered for a new size or new effects */
   bool _bNeedRender;
   
   /** true if this font's face needs to be reloaded */
   bool _bNeedReload;

   /** Additional vertical spacing at the top */
   long _nYTopSpacing;

   /** Graphics holding the bitmap characters for this font */
   std::vector<KGraphic *>_graphic;

   /** Number of characters in table */
   long _nChars;

   /** Character table */
   KTrueTextChar *_lpCharTable;

   /** true to use kerning pairs if the information is available, false not to */
   bool _bUseKerningPairs;

   /** Number of slots allocated in the character table */
   long _nCharTableSize;

   /** Character table's slot index for the box displayed for missing chars (U+FFFD) */
   long _nMissingCharSlot;

   /** Number of character pages */
   long _nCharPages;

   /** For each character page, bits 11-31 of the character number, for fast lookups */
   long *_nPageStart;

   /** Character pages, for fast lookups */
   long **_nCharPage;

   /** Font filename, used to log warnings and errors */
   char _szFileName[K_MAXPATH];

   /** Extra margin around characters, in order to apply effects */
   float _fMargin;

   /** true when rendering seperate channels to be drawn with a shader */
   bool _bShaderRenderingMode;

   /** When rendering seperate channels for a shader, outline thickness */
   float _fShaderOutlineThickness;

   /** Number of colors in gradient to apply to this font */
   long _nGradientColors;

   /** Color gradient to apply to this font */
   KTrueTextGradientColor *_gradientColor;

   /** Size of the outline to apply to this font, in pixels (0 for none) */
   long _nOutlineSize;

   /** Color of the outline to apply to this font */
   float _fOutlineColor[4];

   /** Size of the drop shadow to apply to this font, in pixels (0 for none) */
   long _nDropShadowSize;

   /** X offset of the drop shadow to apply to this font, in pixels */
   long _nDropShadowX;

   /** Y offset of the drop shadow to apply to this font, in pixels */
   long _nDropShadowY;

   /** Color of the drop shadow to apply to this font */
   float _fDropShadowColor[4];

   /** Kerning pairs hashtable */
   KTrueTextKerningEntry **_lpKerningHashFirst;

   /** Number of hashed kerning pairs */
   long _nKerningEntries;

   /** Array of all hashed kerning pairs */
   KTrueTextKerningEntry *_lpKerningEntries;

   /** FreeType library */
   static FT_Library g_lpLibrary;

   /** Encoding of all strings provided to KTrueText for drawing */
   static long g_nKTrueTextEncoding;

   /** Number of characters in char map; -1 if map hasn't been initialized yet */
   static long g_nCharsInMap;

   /** Array of characters that are present in the game's string table */
   static unsigned long *g_nCharMap;
};

#endif   /* _KANJI_KTRUETEXT_H */
