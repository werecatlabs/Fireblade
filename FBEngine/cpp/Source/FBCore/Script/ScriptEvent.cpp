#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptEvent.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    ScriptEvent::ScriptEvent() = default;

    ScriptEvent::ScriptEvent( const String &function ) : m_function( function )
    {
    }

    ScriptEvent::~ScriptEvent() = default;

    auto ScriptEvent::getEventType() const -> hash_type
    {
        return m_hashType;
    }

    void ScriptEvent::setEventType( hash_type type )
    {
        m_hashType = type;
    }

    auto ScriptEvent::getClassName() const -> const String &
    {
        return m_className;
    }

    auto ScriptEvent::getClassName() -> String &
    {
        return m_className;
    }

    void ScriptEvent::setClassName( const String &className )
    {
        m_className = className;
    }

    auto ScriptEvent::getFunction() const -> const String &
    {
        return m_function;
    }

    auto ScriptEvent::getFunction() -> String &
    {
        return m_function;
    }

    void ScriptEvent::setFunction( const String &function )
    {
        m_function = function;
    }
}  // end namespace fb
