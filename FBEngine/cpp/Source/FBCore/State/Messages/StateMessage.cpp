#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessage, IStateMessage );

    const hash_type StateMessage::SET_OBJECT = StringUtil::getHash( "set_object" );
    const hash_type StateMessage::SET_MESH = StringUtil::getHash( "set_mesh" );
    const hash_type StateMessage::SET_CUBEMAP = StringUtil::getHash( "set_cubemap" );
    const hash_type StateMessage::SET_TEXTURES = StringUtil::getHash( "set_textures" );

    void StateMessage::unload( SmartPtr<ISharedObject> data )
    {
        m_sender = nullptr;
        m_stateObject = nullptr;
    }

    hash_type StateMessage::getType() const
    {
        return m_type;
    }

    void StateMessage::setType( hash_type type )
    {
        m_type = type;
    }

    SmartPtr<ISharedObject> StateMessage::getSender() const
    {
        auto p = m_sender.load();
        return p.lock();
    }

    void StateMessage::setSender( SmartPtr<ISharedObject> object )
    {
        m_sender = object;
    }

    SmartPtr<IStateContext> StateMessage::getStateObject() const
    {
        auto p = m_stateObject.load();
        return p.lock();
    }

    void StateMessage::setStateObject( SmartPtr<IStateContext> object )
    {
        m_stateObject = object;
    }
}  // end namespace fb
