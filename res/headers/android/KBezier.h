/**
 * Kanji game engine - Bezier path definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KBEZIER_H
#define  _KANJI_KBEZIER_H

/* Include definitions */
#include "KTypes.h"
#include "KObjectBase.h"
#include "KMath2d.h"

/** Number of arc lengths per segment */
#define  NLENSPERSEGMENT   80

/** Segment of a path */
typedef struct {
   /** Starting point */
   KVector2 vStart;

   /** First control point */
   KVector2 vCtrl1;

   /** Second control point */
   KVector2 vCtrl2;

   /** End point */
   KVector2 vEnd;

   /** Arc lengths at fixed intervals along the segment */
   float fArcLen[NLENSPERSEGMENT];
} KBezierSegment;

/** Curve interval for arc length calculation */
typedef struct {
   /** Starting weight */
	float u1;
 
   /** Ending weight */
   float u2;

   /** Current length (updated by methods) */
	float length;
} KBezierInterval;

/** Quadric function for arc length calculation */
typedef struct {
   /** Coefficients */
	float coeff[5];

   /** Degree */
	long degree;
} KBezierPolynomial;

/* Forward declarations */
class KTiXmlDocument;

/**
 * Class for computing Bezier paths.
 *
 * The KBezier class provides methods for computing Bezier paths.
 */

class KBezier : public KObjectBase {
public:
   /** Constructor */
   KBezier ();

   /** Destructor */
   ~KBezier ();

   /** Free memory used by this path and delete all segments that were set */
   void freeBezier (void);

   /**
    * Get number of segments in path
    *
    * \return path length
    */
   long getSegmentCount (void);

   /**
    * Get specified segment in specified path
    *
    * \param nSegment segment to be obtained
    *
    * \return segment
    */
   KBezierSegment getSegment (long nSegment);

   /**
    * Set segments for this path
    *
    * \param nSegmentCount number of segments in path
    * \param lpPoint array of points, listed as: start_seg1/control1_seg1/control2_seg1/start_seg2/control1_seg2/control2_seg2/.../start_segN/control1_segN/control2_segN/end
    * \param bDisableArcLengths true to disable arc lengths computations (getTotalLength() and computeFromLength() will not be available if true)
    */
   void setSegments (long nSegmentCount, const KVector2 *lpPoint, bool bDisableArcLengths);

   /**
    * Load path from SVG file
    *
    * \param lpszFilename name of file to load path from
    *
    * \return true for success, false for failure
    */
   bool loadFromFile (const char *lpszFilename);

   /**
    * Get total arc length of the whole path
    *
    * \return total length
    */
   float getTotalLength (void);

   /**
    * Get arc length at the start of the specified segment
    *
    * \param nSegment segment number (0.. getSegmentCount() - 1)
    *
    * \return arc length at the start of the specified segment
    */
   float getLengthFromSegment (long nSegment);

   /**
    * Get segment number and weight from arc length
    *
    * \param fArcLength arc length from starting point of first segment in path (0..getTotalLength())
    * \param nSegment returned segment number
    * \param fWeight returned weight inside segment (0.0f at origin, 1.0f at destination)
    *
    * \return true if segment and weight were found, false if not
    */
   bool getWeightFromLength (float fArcLength, long &nSegment, float &fWeight);

   /**
    * Evaluate a point on this path by segment number and weight
    *
    * \param nSegment segment number (0.. getSegmentCount() - 1)
    * \param fWeight weight inside segment (0.0f at origin, 1.0f at destination)
    * \param vDest returned point
    * \param vTangent returned tangent
    */
   void computeFromWeight (long nSegment, float fWeight, KVector2 &vDest, KVector2 &vTangent);

   /**
    * Evaluate a point on this path by arc length
    *
    * \param fArcLength arc length from starting point of first segment in path (0..getTotalLength())
    * \param vDest returned point
    * \param vTangent returned tangent
    * \param nSegment returned segment number
    */
   void computeFromLength (float fArcLength, KVector2 &vDest, KVector2 &vTangent, long &nSegment);

   /** Number of segments */
   long _nSegmentCount;

   /** Array of segments defining each path */
   KBezierSegment *_segment;

private:
   /** Precompute arc lengths across the path */
   void createArcLengthTables (void);

   /**
    * Compute arc length of a section of a path segment
    *
    * \param nSegment segment to compute section's arc length of
    * \param fStartWeight starting weight for arc
    * \param fEndWeight ending weight for arc
    *
    * \return arc length
    */
   float computeArcLength (long nSegment, float fStartWeight, float fEndWeight);

   /**
    * Load and optionally decompress SVG file
    *
    * \param lpszFilename name of SVG file to load
    * \param lpDoc parsed xml document with svg file's contents, if successful
    *
    * \return true for success, false for failure
    */
   static bool loadSvg (const char *lpszFilename, KTiXmlDocument *lpDoc);

   /**
    * Parse path data extracted from a SVG file and create segments in this path instance
    *
    * \param lpszPathData path data to parse
    *
    * \return true for success, false for failure
    */
   bool parseSvgPathData (const char *lpszPathData);

   /**
    * Skip over whitespace in SVG path data
    *
    * \param lpszPathData current path data string location, updated by this method
    */
   static void skipSvgWhitespace (const char *&lpszPathData);

   /**
    * Parse number in SVG path data
    *
    * \param lpszPathData current path data string location, updated by this method
    *
    * \return parsed number
    */
   static float parseSvgNumber (const char *&lpszPathData);

   /**
    * Evaluate a point on a Bezier path using two control points.
    *
    * \param dest computed point
    * \param vTangent returned tangent vector
    * \param a origin
    * \param b control point 1
    * \param c control point 2
    * \param d destination
    * \param t weight (0.0f at origin, 1.0f at destination)
    */
   static void computeCubic (KVector2 &dest, KVector2 &vTangent, const KVector2 &a, const KVector2 &b, const KVector2 &c, const KVector2 &d, float t);

   /**
    * Compute a linear interpolation between two points.
    *
    * \param dest computed point
    * \param a origin
    * \param b destination
    * \param t weight (0.0f at origin, 1.0f at destination)
    */
   static void computeLinear (KVector2 &dest, const KVector2 &a, const KVector2 &b, float t);

   /**
    * Recursively subdivide Bezier curve in order to compute its arc length
    *
    * \param full_interval interval to be calculated
    * \param func polynomial
    * \param total_length current total length
    * \param tolerance error tolerance
    *
    * \return total length
    */
   static float subdivide (const KBezierInterval *full_interval, const KBezierPolynomial *func, float total_length, float tolerance);

   /**
    * Integrate square root of function in given interval using Gaussian quadrature
    *
    * \param func function
    * \param interval interval
    *
    * \return square root (length for interval)
    */
   static float doIntegrate (const KBezierPolynomial *func, const KBezierInterval *interval);

   /**
    * Evaluate polynomial
    *
    * \param poly polynomial
    * \param u coefficient
    *
    * \return evaluated result
    */
   static float evaluatePolynomial (const KBezierPolynomial *poly, float u);

   /** true if arc lengths have been calculated */
   bool _bArcLengthsAvailable;
};

#endif   /* _KANJI_KBEZIER_H */
