/**
 * Kanji rendering and I/O engine - Android debugging definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSYSDEBUG_H
#define  _KANJI_KSYSDEBUG_H

/* Include definitions */
#include "KObjectBase.h"

/* Forward declarations */
class KPTK;

/**
 * System-independent exception handler
 *
 * \param bIsAssertion true for an assertion failure, false for an exception
 * \param nExceptionCode for an exception, OS exception code
 * \param lpszExceptionName for an exception, textual name of exception code
 * \param bHasParam for an exception, true if optional exception parameter is provided
 * \param lpParam for an exception, optional parameter
 * \param lpszAssertionFileName for an assertion failure, sourcecode filename where the assertion failed
 * \param nAssertionLine for an assertion failure, line number in filename where the assertion failed
 * \param nStackDepth number of entries in stack trace
 * \param lpStackReturnAddr array of return addresses in stack trace
 * \param lpszStackModule array of module names in stack trace
 */
typedef void (*KSysDebugHandler) (bool bIsAssertion,
                                  unsigned long nExceptionCode, const char *lpszExceptionName, bool bHasParam, void *lpParam,
                                  const char *lpszAssertionFileName, long nAssertionLine,
                                  long nStackDepth, void *lpStackReturnAddr[], char *lpszStackModule[]);

/**
 * Class for stopping the game gracefully on crashes and assertion failures, and reporting the errors
 */

class KSysDebug : public KObjectBase {
   friend class KPTK;
   
public:
   /** Constructor */
   KSysDebug ();

   /** Destructor */
   ~KSysDebug ();

   /**
    * Enable or disable catching unhandled processor exceptions (crashes) such as division by zero or accessing invalid memory
    *
    * \param lpHandler system-independent handler that catches unhandled processor exceptions, or NULL to let the operating system handle it
    */
   static void catchExceptions (KSysDebugHandler lpHandler);

   /**
    * Report assertion failure
    *
    * \param lpszFileName sourcecode filename where the assertion failed
    * \param nLine line number in filename where the assertion failed
    */
   static void assertionFailed (const char *lpszFileName, unsigned long nLine);
   
private:
   /** Free all resources */
   static void cleanup (void);
};

#endif   /* _KANJI_KSYSDEBUG_H */
