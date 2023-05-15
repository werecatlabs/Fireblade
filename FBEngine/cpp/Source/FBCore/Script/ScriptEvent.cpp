#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptEvent.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    ScriptEvent::ScriptEvent()
    {
    }

    ScriptEvent::ScriptEvent( const String &function ) : m_function( function )
    {
    }

    ScriptEvent::~ScriptEvent()
    {
    }

    hash32 ScriptEvent::getEventType() const
    {
        return m_hashType;
    }

    void ScriptEvent::setEventType( hash32 type )
    {
        m_hashType = type;
    }

    const String &ScriptEvent::getClassName() const
    {
        return m_className;
    }

    String &ScriptEvent::getClassName()
    {
        return m_className;
    }

    void ScriptEvent::setClassName( const String &className )
    {
        m_className = className;
    }

    const String &ScriptEvent::getFunction() const
    {
        return m_function;
    }

    String &ScriptEvent::getFunction()
    {
        return m_function;
    }

    void ScriptEvent::setFunction( const String &function )
    {
        m_function = function;
    }
}  // end namespace fb
