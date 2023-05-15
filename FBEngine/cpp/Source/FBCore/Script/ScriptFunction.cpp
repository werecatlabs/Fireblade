#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptFunction.h>
#include <FBCore/Script/ScriptVariable.h>

namespace fb
{

    ScriptFunction::ScriptFunction()
    {
    }

    ScriptFunction::~ScriptFunction()
    {
    }

    String ScriptFunction::getClassName() const
    {
        return m_className;
    }

    void ScriptFunction::setClassName( const String &className )
    {
        m_className = className;
    }

    String ScriptFunction::getFunctionName() const
    {
        return m_functionName;
    }

    void ScriptFunction::setFunctionName( const String &functionName )
    {
        m_functionName = functionName;
    }

    String ScriptFunction::getReturnType() const
    {
        return m_returnType;
    }

    void ScriptFunction::setReturnType( const String &returnType )
    {
        m_returnType = returnType;
    }

    Array<SmartPtr<IScriptVariable>> ScriptFunction::getArguments() const
    {
        return m_arguments;
    }

    void ScriptFunction::setArguments( const Array<SmartPtr<IScriptVariable>> &arguments )
    {
        m_arguments = arguments;
    }

    bool ScriptFunction::isConstructor() const
    {
        return m_isConstructor;
    }

    void ScriptFunction::setConstructor( bool constructor )
    {
        m_isConstructor = constructor;
    }

    bool ScriptFunction::isDestructor() const
    {
        return m_isDestructor;
    }

    void ScriptFunction::setDestructor( bool destructor )
    {
        m_isDestructor = destructor;
    }

    String ScriptFunction::getFunctionBodyCPP() const
    {
        if( m_returnType != "void" )
        {
            return "return " + m_returnType + "();";
        }

        return "";
    }
}  // end namespace fb
