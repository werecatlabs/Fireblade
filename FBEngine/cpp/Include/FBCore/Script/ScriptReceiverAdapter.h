#ifndef ScriptReceiverAdapter_h__
#define ScriptReceiverAdapter_h__

#include <FBCore/Interface/Script/IScriptReceiver.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    template <class T>
    class ScriptReceiverAdapter : public CSharedObject<IScriptReceiver>
    {
    public:
        ScriptReceiverAdapter( T *listener ) : m_listener( listener )
        {
        }

        s32 setProperty( hash_type hash, const String &value )
        {
            return m_listener->setProperty( hash, value );
        }

        s32 getProperty( hash_type hash, String &value ) const
        {
            return m_listener->getProperty( hash, value );
        }

        s32 setProperty( hash_type hash, const Parameter &param )
        {
            return m_listener->setProperty( hash, param );
        }

        s32 setProperty( hash_type hash, const Parameters &params )
        {
            return m_listener->setProperty( hash, params );
        }

        s32 setProperty( hash_type hash, void *param )
        {
            return m_listener->setProperty( hash, param );
        }

        s32 getProperty( hash_type hash, Parameter &param ) const
        {
            return m_listener->getProperty( hash, param );
        }

        s32 getProperty( hash_type hash, Parameters &params ) const
        {
            return m_listener->getProperty( hash, params );
        }

        s32 getProperty( hash_type hash, void *param ) const
        {
            return m_listener->getProperty( hash, param );
        }

        s32 callFunction( u32 hash, const Parameters &params, Parameters &results )
        {
            return m_listener->callFunction( hash, params, results );
        }

        s32 callFunction( u32 hash, SmartPtr<ISharedObject> object, Parameters &results )
        {
            return m_listener->callFunction( hash, object, results );
        }

    private:
        T *m_listener = nullptr;
    };
}  // end namespace fb

#endif  // ScriptReceiverAdapter_h__
