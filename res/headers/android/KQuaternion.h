/**
 * Kanji game engine - quaternion definitions
 *
 * (c) 2010
 */

#ifndef  _K_QUATERNION_H
#define  _K_QUATERNION_H

/* Include definitions */
#include "KTypes.h"
#include "KObjectBase.h"
#include "KMath3d.h"

/**
 * Class modeling a quaternion for 3d model rotations
 */

class KQuaternion {
public:
   /**
    * Constructor
    *
    * \param x x value
    * \param y y value
    * \param z z value
    * \param w w value
    */
   KQuaternion (float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);

   /** Destructor */
   ~KQuaternion ();

   /**
    * Create from angle and axis
    *
    * \param fAngle angle
    * \param fAx X axis
    * \param fAy Y axis
    * \param fAz Z axis
    */
   void fromAngleAxis (float fAngle, float fAx, float fAy, float fAz);
   
   /**
    * Convert this quaternion to an angle and axis
    *
    * \param fAngle returned angle
    * \param fAx returned X axis
    * \param fAy returned Y axis
    * \param fAz returned Z axis
    */
   void toAngleAxis (float &fAngle, float &fAx, float &fAy, float &fAz);

   /**
    * Execute spherical linear interpolation
    *
    * \param q1 first quaternion to interpolate
    * \param q2 second quaternion to interpolate
    * \param fWeight weight of q2 wrt. q1
    */
   void slerp (KQuaternion &q1, KQuaternion &q2, float fWeight);

   /**
    * "*" operator
    *
    * \param q second quaternion
    *
    * \return this quaternion * second quaternion
    */
   const KQuaternion operator* (KQuaternion& q); 

   /**
    * "*=" operator
    *
    * \param q second quaternion
    *
    * \return this quaternion * second quaternion
    */
   const KQuaternion& operator *= (KQuaternion& q);

   /**
    * Convert this quaternion to a matrix
    *
    * \return matrix
    */
   KMatrix toMatrix (void);

   /** X value */
   float _x;

   /** Y value */
   float _y;

   /** Z value */
   float _z;
   
   /** W value */
   float _w;
};

#endif   /* _K_QUATERNION_H */
