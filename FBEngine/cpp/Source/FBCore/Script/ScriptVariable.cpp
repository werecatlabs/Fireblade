#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptVariable.h>

namespace fb
{

    ScriptVariable::ScriptVariable()
    {
    }

    ScriptVariable::~ScriptVariable()
    {
    }

    String ScriptVariable::getType() const
    {
        return m_type;
    }

    void ScriptVariable::setType( const String &type )
    {
        m_type = type;
    }

    String ScriptVariable::getName() const
    {
        return m_name;
    }

    void ScriptVariable::setName( const String &name )
    {
        m_name = name;
    }

}  // namespace fb
