/**
 * Kanji rendering and I/O engine - definition of an object that can be inserted in a list
 *
 * (c) 2010
 */

#ifndef  _KANJI_KOBJECTLISTABLE_H
#define  _KANJI_KOBJECTLISTABLE_H

/* Include definitions */
#include "KObjectBase.h"

/**
 * Base class of objects that can be inserted in a KList
 */

class KObjectListable : public KObjectBase {
public:
   /** Constructor */
   KObjectListable ();

   /** Destructor */
   virtual ~KObjectListable ();

   /** Next sibling in list */
   KObjectListable *_lpNext;

   /** Previous sibling in list */
   KObjectListable *_lpPrev;
};

#endif   /* _KANJI_KOBJECTLISTABLE_H */
