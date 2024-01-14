#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptFunction.h>
#include <FBCore/Script/ScriptVariable.h>

namespace fb
{

    ScriptFunction::ScriptFunction() = default;

    ScriptFunction::~ScriptFunction() = default;

    auto ScriptFunction::getClassName() const -> String
    {
        return m_className;
    }

    void ScriptFunction::setClassName( const String &className )
    {
        m_className = className;
    }

    auto ScriptFunction::getFunctionName() const -> String
    {
        return m_functionName;
    }

    void ScriptFunction::setFunctionName( const String &functionName )
    {
        m_functionName = functionName;
    }

    auto ScriptFunction::getReturnType() const -> String
    {
        return m_returnType;
    }

    void ScriptFunction::setReturnType( const String &returnType )
    {
        m_returnType = returnType;
    }

    auto ScriptFunction::getArguments() const -> Array<SmartPtr<IScriptVariable>>
    {
        return m_arguments;
    }

    void ScriptFunction::setArguments( const Array<SmartPtr<IScriptVariable>> &arguments )
    {
        m_arguments = arguments;
    }

    auto ScriptFunction::isConstructor() const -> bool
    {
        return m_isConstructor;
    }

    void ScriptFunction::setConstructor( bool constructor )
    {
        m_isConstructor = constructor;
    }

    auto ScriptFunction::isDestructor() const -> bool
    {
        return m_isDestructor;
    }

    void ScriptFunction::setDestructor( bool destructor )
    {
        m_isDestructor = destructor;
    }

    auto ScriptFunction::getFunctionBodyCPP() const -> String
    {
        if( m_returnType != "void" )
        {
            return "return " + m_returnType + "();";
        }

        return "";
    }
}  // end namespace fb
