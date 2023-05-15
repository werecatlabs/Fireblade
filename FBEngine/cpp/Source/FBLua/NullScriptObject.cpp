#include "FBLua/NullScriptObject.h"
#include <FBCore/FBCore.h>

namespace fb
{
    template <>
    NullScriptObject *Singleton<NullScriptObject>::m_singleton = nullptr;

    NullScriptObject::NullScriptObject()
    {
        //setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiverAdapter<NullScriptObject>(this), true));
    }

    NullScriptObject::~NullScriptObject()
    {
    }

    void NullScriptObject::update( const s32 &task, const time_interval &t, const time_interval &dt )
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
    }

    s32 NullScriptObject::getFSM( u32 hash, SmartPtr<IFSM> &fsm )
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    const SmartPtr<IScriptInvoker> &NullScriptObject::getInvoker() const
    {
        return m_scriptInvoker;
    }

    SmartPtr<IScriptInvoker> &NullScriptObject::getInvoker()
    {
        return m_scriptInvoker;
    }

    void NullScriptObject::setInvoker( SmartPtr<IScriptInvoker> invoker )
    {
        m_scriptInvoker = invoker;
    }

    const SmartPtr<IScriptReceiver> &NullScriptObject::getReceiver() const
    {
        return m_scriptReceiver;
    }

    SmartPtr<IScriptReceiver> &NullScriptObject::getReceiver()
    {
        return m_scriptReceiver;
    }

    void NullScriptObject::setReceiver( SmartPtr<IScriptReceiver> receiver )
    {
        m_scriptReceiver = receiver;
    }

    s32 NullScriptObject::setProperty( hash32 hash, const String &value )
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::getProperty( hash32 hash, String &value ) const
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::callFunction( u32 hash, SmartPtr<IScriptObject> object, Parameters &results )
    {
        return 0;
    }

    s32 NullScriptObject::callFunction( u32 hash, const Parameters &params, Parameters &results )
    {
        return 0;
    }

    void NullScriptObject::setObject( hash32 hash, SmartPtr<IScriptObject> object )
    {
    }

    void NullScriptObject::getObject( hash32 hash, SmartPtr<IScriptObject> &object ) const
    {
    }

    s32 NullScriptObject::setProperty( hash32 hash, const Parameter &param )
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::setProperty( hash32 hash, const Parameters &params )
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::setProperty( hash32 hash, void *param )
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::getProperty( hash32 hash, Parameter &param ) const
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::getProperty( hash32 hash, Parameters &params ) const
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }

    s32 NullScriptObject::getProperty( hash32 hash, void *param ) const
    {
        FB_LOG_MESSAGE( "LuaScriptMgr", "Null object." );
        return 0;
    }
} // end namespace fb
