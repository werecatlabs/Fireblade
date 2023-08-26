#ifndef ScriptInvokerStandard_h__
#define ScriptInvokerStandard_h__

#include <FBCore/FBCorePrerequisites.h>

#include <FBCore/Interface/Script/IScriptInvoker.h>
#include <FBCore/Core/HashMap.h>
#include <FBCore/System/ThreadDebug.h>

namespace fb
{
    /** Implementation of IScriptInvoker the interface. */
    class ScriptInvoker : public IScriptInvoker
    {
    public:
        ScriptInvoker();
        ScriptInvoker( SmartPtr<ISharedObject> scriptObject );
        ~ScriptInvoker() override;

        /** Overridden from IScriptInvoker. */
        SmartPtr<ISharedObject> getOwner() const override;

        /** Overridden from IScriptInvoker. */
        void setOwner( SmartPtr<ISharedObject> owner ) override;

        /** @copydoc IScriptInvoker::callObjectMember */
        void callObjectMember( const String &functionName );

        /** @copydoc IScriptInvoker::callObjectMember */
        void callObjectMember( const String &functionName, const Parameters &params );

        /** @copydoc IScriptInvoker::callObjectMember */
        void callObjectMember( const String &functionName, const Parameters &params,
                               Parameters &results );

        /** Overridden from IScriptInvoker. */
        void event( hash_type hash ) override;

        /** Overridden from IScriptInvoker. */
        void event( hash_type hash, const Parameters &params ) override;

        /** Overridden from IScriptInvoker. */
        void event( hash_type hash, const Parameters &params, Parameters &results ) override;

        bool hasEvent( hash_type hash ) const override;

        /** Overridden from IScriptInvoker. */
        void setEventFunction( hash_type hash, SmartPtr<IEvent> event ) override;

        /** Overridden from IScriptInvoker. */
        SmartPtr<IEvent> getEventFunction( hash_type hash ) const override;

        /** Overridden from IScriptInvoker. */
        u32 getNumEvents() const override;

        /** Overridden from IScriptInvoker. */
        void set( hash_type hash, const Parameter &param ) override;

        /** Overridden from IScriptInvoker. */
        void set( const String &id, const Parameter &param ) override;

        /** Overridden from IScriptInvoker. */
        Parameter get( hash_type hash ) override;

        /** Overridden from IScriptInvoker. */
        Parameter get( const String &id ) override;

    protected:
        /// The owner object.
        SmartPtr<ISharedObject> m_object;

        /// A type definition for an event hash map.
        using Events = HashMap<hash_type, SmartPtr<IScriptEvent>>;

        /// Stores the events.
        Events m_events;

        THREAD_DEBUG_MUTABLE( m_threadDebug );
    };

}  // end namespace fb

#endif  // ScriptInvokerStandard_h__
