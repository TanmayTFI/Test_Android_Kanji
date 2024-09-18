/**
 * Kanji game engine - scripting language VM base class definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSCRIPT_H
#define  _KANJI_KSCRIPT_H

/** \cond private */

/* Include definitions */
#include "KTypes.h"
#include "KObjectBase.h"

/**
 * Scripting language VM base class
 */

class KScript : public KObjectBase {
public:
   /** Constructor */
   KScript ();

   /** Destructor */
   virtual ~KScript ();

   /** Add a reference to this script */
   void addReference (void);

   /** Remove a reference to this script */
   void delReference (void);

   /**
    * Execute command in the current instance
    *
    * \param lpszCommand script command to be executed
    * \param bSilenceErrors true to avoid logging errors, false to log them
    *
    * \return true for success, false for failure
    */
   virtual bool execute (const char *lpszCommand, bool bSilenceErrors = false) = 0;

   /**
    * Call function in this script, taking and returning a variable set of parameters
    *
    * \param lpszFuncName name of function to be called
    * \param nParams number of input parameters to pass to the lua function
    * \param lpszParamType string containing the type of each input parameter, in order; 's' for string, 'i' for integer and 'f' for float. A function taking a float as first parameter and string as second would be coded as "fs"
    * \param param array of input parameters
    * \param nRetVals number of output values returned by the lua function
    * \param lpszRetValType string containing the type of each output value, in order; a function returning two integers would be coded as "ii"
    * \param retVal array of output values
    *
    * \return true for success, false for failure
    */
   virtual bool callVar (const char *lpszFuncName, long nParams, const char *lpszParamType, void *param[], long nRetVals, const char *lpszRetValType, void *retVal[]) = 0;

private:
   /** Current number of references to this script */
   long _nReferences;
};

/** \endcond */

#endif   /* _KANJI_KSCRIPT_H */
