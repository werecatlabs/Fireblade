#ifndef ScriptEvent_h__
#define ScriptEvent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/Script/IScriptEvent.h>

namespace fb
{
    
    /** A class used to store info for a scripted event. */
    class ScriptEvent : public CSharedObject<IScriptEvent>
    {
    public:
        ScriptEvent();
        ScriptEvent( const String &function );
        ~ScriptEvent() override;

        /** Gets the type of event. */
        hash_type getEventType() const;

        void setEventType( hash_type type );

        /** Gets the class name. */
        String &getClassName() override;

        /** Gets the class name. */
        const String &getClassName() const override;

        /** sets the class name. */
        void setClassName( const String &className ) override;

        /** Gets the function name. */
        String &getFunction() override;

        /** Gets the function name. */
        const String &getFunction() const override;

        /** sets the function name. */
        void setFunction( const String &function ) override;

    protected:
        hash_type m_hashType = 0;

        /// The name of the script class.
        String m_className;

        /// The name of the function.
        String m_function;
    };
}  // end namespace fb

#endif  // ScriptEvent_h__
