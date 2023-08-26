#ifndef NullScriptObject_h__
#define NullScriptObject_h__

#include <FBCore/Core/Singleton.h>
#include <FBCore/Core/Parameter.h>
#include <FBCore/Interface/Script/IScriptObject.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{

    class NullScriptObject : public SharedObject<IScriptObject>, public Singleton<NullScriptObject>
    {
    public:
        NullScriptObject();
        ~NullScriptObject() override;

        virtual void update( const s32 &task, const time_interval &t, const time_interval &dt );
        s32 getFSM( u32 hash, SmartPtr<IFSM> &fsm );

        //
        // IScriptObject functions
        //

        /** Gets an object call script functions. */
        SmartPtr<IScriptInvoker> &getInvoker() override;

        /** Gets an object call script functions. */
        const SmartPtr<IScriptInvoker> &getInvoker() const override;

        /** Sets an object call script functions. */
        void setInvoker( SmartPtr<IScriptInvoker> invoker ) override;

        /** Gets an object to receive script calls. */
        SmartPtr<IScriptReceiver> &getReceiver() override;

        /** Gets an object to receive script calls. */
        const SmartPtr<IScriptReceiver> &getReceiver() const override;

        /** Sets an object to receive script calls. */
        void setReceiver( SmartPtr<IScriptReceiver> receiver ) override;

        virtual s32 setProperty( hash32 hash, const String &value );
        virtual s32 getProperty( hash32 hash, String &value ) const;
        virtual s32 setProperty( hash32 hash, const Parameter &param );
        virtual s32 setProperty( hash32 hash, const Parameters &params );
        virtual s32 setProperty( hash32 hash, void *param );
        virtual s32 getProperty( hash32 hash, Parameter &param ) const;
        virtual s32 getProperty( hash32 hash, Parameters &params ) const;
        virtual s32 getProperty( hash32 hash, void *param ) const;

        virtual s32 callFunction( u32 hash, const Parameters &params, Parameters &results );
        virtual s32 callFunction( u32 hash, SmartPtr<IScriptObject> object, Parameters &results );

        virtual void setObject( hash32 hash, SmartPtr<IScriptObject> object );
        virtual void getObject( hash32 hash, SmartPtr<IScriptObject> &object ) const;

    protected:
        /// Used to call script functions.
        SmartPtr<IScriptInvoker> m_scriptInvoker;

        /// Used to receive script calls.
        SmartPtr<IScriptReceiver> m_scriptReceiver;
    };
} // namespace fb

#endif  // NullScriptObject_h__
