#ifndef IScriptEvent_h__
#define IScriptEvent_h__

#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    
    /** A class used to store info for a scripted event. */
    class IScriptEvent : public IEvent
    {
    public:
        /** Destructor. */
        ~IScriptEvent() override = default;

        /** Gets the class name. */
        virtual String &getClassName() = 0;

        /** Gets the class name. */
        virtual const String &getClassName() const = 0;

        /** Sets the class name. */
        virtual void setClassName( const String &className ) = 0;

        /** Gets the function name. */
        virtual String &getFunction() = 0;

        /** Gets the function name. */
        virtual const String &getFunction() const = 0;

        /** Sets the function name. */
        virtual void setFunction( const String &function ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IScriptEvent_h__
