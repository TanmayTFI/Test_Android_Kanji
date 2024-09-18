#include "KPTK.h"
#include "KGame.h"

#ifdef K_ANDROID

class CAndroidUIEditField : public KUIEditField {
public:
    /**
     * Check if the supplied element is a button, and cast as CAndroidUIEditField if it is
     *
     * \return CAndroidUIEditField if element is a button, NULL otherwise
     */
    bool isKeyboardShowing;

    static CAndroidUIEditField *isCAndroidUIEditField (KUIElement *lpElement) {
        if (lpElement->isOfClass ("CAndroidUIEditField"))
            return (CAndroidUIEditField *) lpElement;
        else
            return NULL;
    }

    /**
     * Constructor
     *
     * \param lpParentElem parent element (the root or another element)
     * \param lpGame game instance
     */

    CAndroidUIEditField (KUIElement *lpParentElem) :
            KUIEditField (lpParentElem) {
        addClass ("CAndroidUIEditField");
    }

protected:
    /**
     * Handle keyboard focus
     *
     * \param bAcquire true if acquiring it, false if losing it
     *
     * \return if acquiring the focus, true if focus was accepted, false if it wasn't (element doesn't handle the keyboard) ; ignored when losing the focus
     */
    virtual bool handleFocus (bool bAcquire) {
        if (bAcquire) {
            /* Display virtual keyboard if needed (Touchpad) */
            KSysAndroid::setVirtualKeyboardStatus (true);
            isKeyboardShowing = true;
        }
        else {
            /* Hide virtual keyboard if needed */
            KSysAndroid::setVirtualKeyboardStatus (false);
            isKeyboardShowing = false;
        }

        return KUIEditField::handleFocus (bAcquire);
    }
};

#endif