#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessage, IStateMessage );

    const hash_type StateMessage::SET_OBJECT = StringUtil::getHash( "set_object" );
    const hash_type StateMessage::SET_MESH = StringUtil::getHash( "set_mesh" );
    const hash_type StateMessage::SET_CUBEMAP = StringUtil::getHash( "set_cubemap" );
    const hash_type StateMessage::SET_TEXTURES = StringUtil::getHash( "set_textures" );

    StateMessage::StateMessage() = default;
    StateMessage::~StateMessage() = default;

    void StateMessage::unload( SmartPtr<ISharedObject> data )
    {
        m_sender = nullptr;
        m_stateContext = nullptr;
    }

    auto StateMessage::getType() const -> hash_type
    {
        return m_type;
    }

    void StateMessage::setType( hash_type type )
    {
        m_type = type;
    }

    auto StateMessage::getSender() const -> SmartPtr<ISharedObject>
    {
        auto p = m_sender.load();
        return p.lock();
    }

    void StateMessage::setSender( SmartPtr<ISharedObject> object )
    {
        m_sender = object;
    }

    auto StateMessage::getStateContext() const -> SmartPtr<IStateContext>
    {
        auto p = m_stateContext.load();
        return p.lock();
    }

    void StateMessage::setStateContext( SmartPtr<IStateContext> object )
    {
        m_stateContext = object;
    }
}  // end namespace fb
