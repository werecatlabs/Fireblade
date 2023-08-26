#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptClass.h>
#include <FBCore/Script/ScriptFunction.h>

namespace fb
{
    ScriptClass::ScriptClass()
    {
    }

    ScriptClass::~ScriptClass()
    {
    }

    String ScriptClass::getClassName() const
    {
        return m_className;
    }

    void ScriptClass::setClassName( const String &className )
    {
        m_className = className;
    }

    Array<String> ScriptClass::getNamespaceNames() const
    {
        return m_namespaceNames;
    }

    void ScriptClass::setNamespaceNames( const Array<String> &namespaceNames )
    {
        m_namespaceNames = namespaceNames;
    }

    Array<SmartPtr<IScriptFunction>> ScriptClass::getFunctions() const
    {
        return m_functions;
    }

    void ScriptClass::setFunctions( const Array<SmartPtr<IScriptFunction>> &functions )
    {
        m_functions = functions;
    }

    Array<String> ScriptClass::getParentClasses() const
    {
        return m_parentClasses;
    }

    void ScriptClass::setParentClasses( const Array<String> &parentClasses )
    {
        m_parentClasses = parentClasses;
    }

    Array<String> ScriptClass::getHeaderIncludes() const
    {
        return m_headerIncludes;
    }

    void ScriptClass::setHeaderIncludes( const Array<String> &headers )
    {
        m_headerIncludes = headers;
    }

    Array<String> ScriptClass::getSourceIncludes() const
    {
        return m_sourceIncludes;
    }

    void ScriptClass::setSourceIncludes( const Array<String> &headers )
    {
        m_sourceIncludes = headers;
    }
}  // namespace fb
