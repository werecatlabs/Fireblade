#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptVariable.h>

namespace fb
{

    ScriptVariable::ScriptVariable() = default;

    ScriptVariable::~ScriptVariable() = default;

    auto ScriptVariable::getType() const -> String
    {
        return m_type;
    }

    void ScriptVariable::setType( const String &type )
    {
        m_type = type;
    }

    auto ScriptVariable::getName() const -> String
    {
        return m_name;
    }

    void ScriptVariable::setName( const String &name )
    {
        m_name = name;
    }

}  // namespace fb
