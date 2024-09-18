/**
 * Kanji rendering and I/O engine - base class for GPU buffer, definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KBATCH_H
#define  _KANJI_KBATCH_H

/* Include definitions */
#include "KWindow.h"
#include "KGraphic.h"
#include "KList.h"
#include <GLES/gl.h>
#include <GLES/glext.h>

/** Vertex type */
enum {
   K_BATCH_DYNTL = 0,            /**< Ignore setWorldView() on renderers that support skipping the transform & lighting stage, otherwise same as K_BATCH_DYNL */
   K_BATCH_DYNL  = 1,            /**< Follow setWorldView(), slightly slower than K_BATCH_DYNTL */
   K_BATCH_STAT3D = 2,           /**< Static vertex buffer, with 3d vertices */
};

/**
 * Class modeling OpenGL GPU buffers
 */

class KBatch : public KObjectListable {
public:
   /** Destructor */
   virtual ~KBatch () { ; }

   /**
    * Allocate vertex buffer using the K_BATCH_DYNTL vertex type
    *
    * \param nQuads maximum number of quads that the buffer holds. The buffer is circular; when more quads than the maximum amount are
    *               drawn, the buffer's contents are rendered and then the buffer is filled up again
    *
    * \return true for success, false for failure
    */
   virtual bool allocateBuffer (long nQuads) = 0;

   /**
    * Allocate vertex buffer using vertices of the specified type
    *
    * \param nVertices maximum number of vertices that the buffer holds. The buffer is circular; when more quads than the maximum amount are
    *                  drawn, the buffer's contents are rendered and then the buffer is filled up again
    * \param nType vertex type (K_BATCH_xxx)
    *
    * \return true for success, false for failure
    */
   virtual bool allocateVertexBuffer (long nVertices, long nType) = 0;

   /** Free all GPU buffers for this instance */
   virtual void freeBuffer (void) = 0;

   /**
    * Start drawing primitives into this vertex buffer
    *
    * \param lpGraphic graphic to use for drawing
    */
   virtual void beginBatch (KGraphic *lpGraphic) = 0;

   /**
    * Draw all primitives in this vertex buffer and stop drawing to it
    */
   virtual void endBatch (void) = 0;

   /**
    * Batch draw line
    *
    * \param dx1 screen X coordinate of first point
    * \param dy1 screen Y coordinate of first point
    * \param dx2 screen X coordinate of second point
    * \param dy2 screen Y coordinate of second point
    * \param r red component of line color
    * \param g green component of line color
    * \param b blue component of line color
    * \param fBlend blending factor
    */
   virtual void drawLine (float dx1, float dy1, float dx2, float dy2, float r, float g, float b, float fBlend) = 0;
   
   /**
    * Batch draw triangle
    *
    * \param dx1 screen X coordinate of first point
    * \param dy1 screen Y coordinate of first point
    * \param dx2 screen X coordinate of second point
    * \param dy2 screen Y coordinate of second point
    * \param dx3 screen X coordinate of third point
    * \param dy3 screen Y coordinate of third point
    * \param r red component of rectangle color
    * \param g green component of rectangle color
    * \param b blue component of rectangle color
    * \param fBlend blending factor
    */
   virtual void drawTriangle (float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float r, float g, float b, float fBlend) = 0;
   
   /**
    * Batch draw rectangle
    *
    * \param dx1 screen X coordinate of top, left pixel
    * \param dy1 screen Y coordinate of top, left pixel
    * \param dx2 screen X coordinate + 1 of bottom, right pixel
    * \param dy2 screen Y coordinate + 1 of bottom, right pixel
    * \param r red component of rectangle color
    * \param g green component of rectangle color
    * \param b blue component of rectangle color
    * \param fBlend blending factor
    */
   virtual void drawRect (float dx1, float dy1, float dx2, float dy2, float r, float g, float b, float fBlend) = 0;

   /**
    * Batch draw color gradient
    *
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param r1 red component of color at first point
    * \param g1 green component of color at first point
    * \param b1 blue component of color at first point
    * \param fBlend1 blending factor at first point
    * \param r2 red component of color at second point
    * \param g2 green component of color at second point
    * \param b2 blue component of color at second point
    * \param fBlend2 blending factor at second point
    * \param r3 red component of color at third point
    * \param g3 green component of color at third point
    * \param b3 blue component of color at third point
    * \param fBlend3 blending factor at third point
    * \param r4 red component of color at fourth point
    * \param g4 green component of color at fourth point
    * \param b4 blue component of color at fourth point
    * \param fBlend4 blending factor at fourth point
    */
   virtual void drawGradient (float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4,
                              float r1, float g1, float b1, float fBlend1, float r2, float g2, float b2, float fBlend2,
                              float r3, float g3, float b3, float fBlend3, float r4, float g4, float b4, float fBlend4) = 0;
   
   /**
    * Blit portion of this graphic, at subpixel screen coordinates
    *
    * \param sx1 source X coordinate of top, left pixel in selected graphic
    * \param sy1 source Y coordinate of top, left pixel in selected graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in selected graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in selected graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    */
   virtual void blitAlphaRect (float sx1, float sy1, float sx2, float sy2, float dx, float dy, bool bXFlip = false, bool bYFlip = false) = 0;

   /**
    * Batch blit portion of selected graphic to this vertex buffer
    *
    * \param sx1 source X coordinate of top, left pixel in selected graphic
    * \param sy1 source Y coordinate of top, left pixel in selected graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in selected graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in selected graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitAlphaRectFx (float sx1, float sy1, float sx2, float sy2, float dx, float dy, float fZAngle, float fScale, float fBlend,
                                 bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;

   /**
    * Batch blit portion of selected graphic to this vertex buffer, using matrix transforms
    *
    * \param sx1 source X coordinate of top, left pixel in graphic
    * \param sy1 source Y coordinate of top, left pixel in graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in graphic
    * \param dx1 screen X coordinate of top, left pixel
    * \param dy1 screen Y coordinate of top, left pixel
    * \param dx2 screen X coordinate + 1 of bottom, right pixel
    * \param dy2 screen Y coordinate + 1 of bottom, right pixel
    * \param lpParentMatrix matrix to apply for this blit before adding the specified rotation, scaling and shear transforms
    * \param fZAngle rotation angle over the Z axis, in degrees
    * \param fScaleX X scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fScaleY Y scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fShearX X shearing factor
    * \param fShearY Y shearing factor
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param bXFlip true to mirror over the X axis when drawing
    * \param bYFlip true to mirror over the Y axis when drawing
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitAlphaRectMatrixF (float sx1, float sy1, float sx2, float sy2, float dx1, float dy1, float dx2, float dy2,
                                      KMatrix *lpParentMatrix, float fZAngle, float fScaleX, float fScaleY, float fShearX, float fShearY, float fBlend,
                                      bool bXFlip = false, bool bYFlip = false, float cx = 0, float cy = 0) = 0;
   
   /**
    * Batch blit portion of selected graphic to this vertex buffer, using a fast subset of transforms
    *
    * \param sx1 source X coordinate of top, left pixel in selected graphic
    * \param sy1 source Y coordinate of top, left pixel in selected graphic
    * \param sx2 source X coordinate + 1 of bottom, right pixel in selected graphic
    * \param sy2 source Y coordinate + 1 of bottom, right pixel in selected graphic
    * \param dx screen X coordinate of top, left pixel
    * \param dy screen Y coordinate of top, left pixel
    * \param fScale scaling factor (1.0 for 1:1 texel to pixel mapping)
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    * \param cx X offset of the center of rotation and scaling from the middle of the source rectangle
    * \param cy Y offset of the center of rotation and scaling from the middle of the source rectangle
    */
   virtual void blitFastRectFxF (float sx1, float sy1, float sx2, float sy2, float dx, float dy, float fScale, float fBlend,
                                 float cx = 0, float cy = 0) = 0;

   /**
    * Batch blit portion of selected graphic as a quad
    *
    * \param sx1 source X coordinate of first point in graphic, in clockwise order
    * \param sy1 source Y coordinate of first point in graphic, in clockwise order
    * \param sx2 source X coordinate of second point in graphic, in clockwise order
    * \param sy2 source Y coordinate of second point in graphic, in clockwise order
    * \param sx3 source X coordinate of third point in graphic, in clockwise order
    * \param sy3 source Y coordinate of third point in graphic, in clockwise order
    * \param sx4 source X coordinate of fourth point in graphic, in clockwise order
    * \param sy4 source Y coordinate of fourth point in graphic, in clockwise order
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param fBlend blending factor, multiplied with the graphic's tint color alpha component
    */
   virtual void blitArbitraryQuad (float sx1, float sy1, float sx2, float sy2, float sx3, float sy3, float sx4, float sy4,
                                   float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4,
                                   float fBlend = 1.0f) = 0;

   /**
    * Batch blit portion of selected graphic as a quad and interpolate color between the quad points
    *
    * \param sx1 source X coordinate of first point in graphic, in clockwise order
    * \param sy1 source Y coordinate of first point in graphic, in clockwise order
    * \param sx2 source X coordinate of second point in graphic, in clockwise order
    * \param sy2 source Y coordinate of second point in graphic, in clockwise order
    * \param sx3 source X coordinate of third point in graphic, in clockwise order
    * \param sy3 source Y coordinate of third point in graphic, in clockwise order
    * \param sx4 source X coordinate of fourth point in graphic, in clockwise order
    * \param sy4 source Y coordinate of fourth point in graphic, in clockwise order
    * \param dx1 screen X coordinate of first point in clockwise order
    * \param dy1 screen Y coordinate of first point in clockwise order
    * \param dx2 screen X coordinate of second point in clockwise order
    * \param dy2 screen Y coordinate of second point in clockwise order
    * \param dx3 screen X coordinate of third point in clockwise order
    * \param dy3 screen Y coordinate of third point in clockwise order
    * \param dx4 screen X coordinate of fourth point in clockwise order
    * \param dy4 screen Y coordinate of fourth point in clockwise order
    * \param r1 red component of color at first point
    * \param g1 green component of color at first point
    * \param b1 blue component of color at first point
    * \param fBlend1 blending factor at first point
    * \param r2 red component of color at second point
    * \param g2 green component of color at second point
    * \param b2 blue component of color at second point
    * \param fBlend2 blending factor at second point
    * \param r3 red component of color at third point
    * \param g3 green component of color at third point
    * \param b3 blue component of color at third point
    * \param fBlend3 blending factor at third point
    * \param r4 red component of color at fourth point
    * \param g4 green component of color at fourth point
    * \param b4 blue component of color at fourth point
    * \param fBlend4 blending factor at fourth point
    */
   virtual void blitArbitraryQuadGradient (float sx1, float sy1, float sx2, float sy2, float sx3, float sy3, float sx4, float sy4,
                                           float dx1, float dy1, float dx2, float dy2, float dx3, float dy3, float dx4, float dy4,
                                           float r1, float g1, float b1, float fBlend1, float r2, float g2, float b2, float fBlend2,
                                           float r3, float g3, float b3, float fBlend3, float r4, float g4, float b4, float fBlend4) = 0;
   
   /**
    * Batch blit perspective-correct triangle of selected graphic. This method requires a K_BATCH_DYNTL buffer
    *
    * \param sx1 source X coordinate of first point in graphic
    * \param sy1 source Y coordinate of first point in graphic
    * \param sx2 source X coordinate of second point in graphic
    * \param sy2 source Y coordinate of second point in graphic
    * \param sx3 source X coordinate of third point in graphic
    * \param sy3 source Y coordinate of third point in graphic
    * \param dx1 screen X coordinate of first point
    * \param dy1 screen Y coordinate of first point
    * \param dz1 transformed Z coordinate of first point
    * \param dw1 transformed W coordinate of first point
    * \param dx2 screen X coordinate of second point
    * \param dy2 screen Y coordinate of second point
    * \param dz2 transformed Z coordinate of second point
    * \param dw2 transformed W coordinate of second point
    * \param dx3 screen X coordinate of third point
    * \param dy3 screen Y coordinate of third point
    * \param dz3 transformed Z coordinate of third point
    * \param dw3 transformed W coordinate of third point
    */
	virtual void blitPerspectiveTriangle (float sx1, float sy1, float sx2, float sy2, float sx3, float sy3,
                                         float dx1, float dy1, float dz1, float dw1,
                                         float dx2, float dy2, float dz2, float dw2,
                                         float dx3, float dy3, float dz3, float dw3) = 0;

   /**
    * Batch blit perspective-correct lit triangle of selected graphic. This method requires a K_BATCH_DYNTL buffer
    *
    * \param sx1 source X coordinate of first point in graphic
    * \param sy1 source Y coordinate of first point in graphic
    * \param sx2 source X coordinate of second point in graphic
    * \param sy2 source Y coordinate of second point in graphic
    * \param sx3 source X coordinate of third point in graphic
    * \param sy3 source Y coordinate of third point in graphic
    * \param dx1 screen X coordinate of first point
    * \param dy1 screen Y coordinate of first point
    * \param dz1 transformed Z coordinate of first point
    * \param dw1 transformed W coordinate of first point
    * \param dl1 lighting value of first point (0..1, multiplied with the rgb components of the tinting color)
    * \param dx2 screen X coordinate of second point
    * \param dy2 screen Y coordinate of second point
    * \param dz2 transformed Z coordinate of second point
    * \param dw2 transformed W coordinate of second point
    * \param dl2 lighting value of third point (0..1, multiplied with the rgb components of the tinting color)
    * \param dx3 screen X coordinate of third point
    * \param dy3 screen Y coordinate of third point
    * \param dz3 transformed Z coordinate of third point
    * \param dw3 transformed W coordinate of third point
    * \param dl3 lighting value of third point (0..1, multiplied with the rgb components of the tinting color)
    */
	virtual void blitPerspectiveTriangleLit (float sx1, float sy1, float sx2, float sy2, float sx3, float sy3,
                                            float dx1, float dy1, float dz1, float dw1, float dl1,
                                            float dx2, float dy2, float dz2, float dw2, float dl2,
                                            float dx3, float dy3, float dz3, float dw3, float dl3) = 0;

   /**
    * Batch blit perspective-correct lit and tinted triangle of selected graphic. This method requires a K_BATCH_DYNTL buffer
    *
    * \param sx1 source X coordinate of first point in graphic
    * \param sy1 source Y coordinate of first point in graphic
    * \param sx2 source X coordinate of second point in graphic
    * \param sy2 source Y coordinate of second point in graphic
    * \param sx3 source X coordinate of third point in graphic
    * \param sy3 source Y coordinate of third point in graphic
    * \param dx1 screen X coordinate of first point
    * \param dy1 screen Y coordinate of first point
    * \param dz1 transformed Z coordinate of first point
    * \param dw1 transformed W coordinate of first point
    * \param dr1 red lighting value of first point (0..1, multiplied with the r component of the tinting color)
    * \param dg1 red lighting value of first point (0..1, multiplied with the g component of the tinting color)
    * \param db1 red lighting value of first point (0..1, multiplied with the b component of the tinting color)
    * \param dx2 screen X coordinate of second point
    * \param dy2 screen Y coordinate of second point
    * \param dz2 transformed Z coordinate of second point
    * \param dw2 transformed W coordinate of second point
    * \param dr2 red lighting value of first point (0..1, multiplied with the r component of the tinting color)
    * \param dg2 red lighting value of first point (0..1, multiplied with the g component of the tinting color)
    * \param db2 red lighting value of first point (0..1, multiplied with the b component of the tinting color)
    * \param dx3 screen X coordinate of third point
    * \param dy3 screen Y coordinate of third point
    * \param dz3 transformed Z coordinate of third point
    * \param dw3 transformed W coordinate of third point
    * \param dr3 red lighting value of first point (0..1, multiplied with the r component of the tinting color)
    * \param dg3 red lighting value of first point (0..1, multiplied with the g component of the tinting color)
    * \param db3 red lighting value of first point (0..1, multiplied with the b component of the tinting color)
    */
	virtual void blitPerspectiveTriangleColor (float sx1, float sy1, float sx2, float sy2, float sx3, float sy3,
                                              float dx1, float dy1, float dz1, float dw1, float dr1, float dg1, float db1,
                                              float dx2, float dy2, float dz2, float dw2, float dr2, float dg2, float db2,
                                              float dx3, float dy3, float dz3, float dw3, float dr3, float dg3, float db3) = 0;

   /**
    * Lock this buffer containing 3d vertices, so it can be filled with set3dVertex()
    */
   virtual void lock3dBuffer (void) = 0;
   
   /**
    * Set the contents of a 3d vertex in this buffer
    *
    * \param nIndex index of vertex to be set, 0..nVertices-1
    * \param vPos 3d position
    * \param vNormal normal vector
    * \param vTexCoord UV coordinates in texture
    */
   virtual void set3dVertex (long nIndex, KVector3 vPos, KVector3 vNormal, KVector2 vTexCoord) = 0;
   
   /**
    * Unlock this buffer containing 3d vertices, after filling it
    */
   virtual void unlock3dBuffer (void) = 0;
   
   /**
    * Draw triangles by taking 3d vertices in this buffer three by three
    *
    * \param nFirstVertexIdx index of first vertex to draw triangles from
    * \param nVertexCount number of vertices to draw triangles from
    * \param lpGraphic graphic to use for drawing
    */
   virtual void blit3dBuffer (long nFirstVertexIdx, long nVertexCount, KGraphic *lpGraphic) = 0;
   
   /**
    * Check if the contents of this static buffer since the last time we checked
    *
    * \return true if buffer was lost, false if not
    */
   virtual bool isBufferLost (void) = 0;   
};

#endif   /* _KANJI_KBATCH_H */
