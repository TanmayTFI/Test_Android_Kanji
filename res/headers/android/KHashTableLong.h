/**
 * Kanji rendering and I/O engine - long integer hashtable definition
 *
 * (c) 2011
 */

#ifndef  _KANJI_KHASHTABLENUM_H
#define  _KANJI_KHASHTABLENUM_H

/** \cond private */

/* Include definitions */
#include "KObjectListable.h"

/** Number of hashtable buckets */
#define  _KHASHLONG_SIZE   64

/** Base class of objects that can be inserted in a KHashTableLong */

class KObjectHashableLong : public KObjectListable {
public:
   /** Constructor */
   KObjectHashableLong ();

   /** Destructor */
   ~KObjectHashableLong ();

   /**
    * Set hashing key
    *
    * \param nKey hashing key
    */
   void setHashKey (long nKey);

   /** Hash key */
   long _nKey;

   /** Next object in the same hash bucket */
   KObjectHashableLong *_lpHashNext;

   /** Index of hashtable bucket that this object is in */
   long _nHash;
};

/** Numerical ID hashtable */

class KHashTableLong : public KObjectBase {
public:
   /** Constructor */
   KHashTableLong ();

   /** Destructor */
   ~KHashTableLong ();

   /**
    * Insert object into its hashtable bucket
    *
    * \param lpObject object to be inserted
    */
   void hashInsert (KObjectHashableLong *lpObject);

   /**
    * Remove object from its hashtable bucket
    *
    * \param lpObject object to be removed
    */
   void hashRemove (KObjectHashableLong *lpObject);

   /**
    * Find object corresponding to the specified key
    *
    * \param nKey key to find object for
    *
    * \return object, NULL if not found
    */
   KObjectHashableLong *hashFind (long nKey) const;

private:
   /** Hashtable buckets */
   KObjectHashableLong *_lpHashFirst[_KHASHLONG_SIZE];
};

/** \endcond */

#endif   /* _KANJI_KHASHTABLENUM_H */
