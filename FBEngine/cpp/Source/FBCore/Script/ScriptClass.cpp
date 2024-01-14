#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptClass.h>
#include <FBCore/Script/ScriptFunction.h>

namespace fb
{
    ScriptClass::ScriptClass() = default;

    ScriptClass::~ScriptClass() = default;

    auto ScriptClass::getClassName() const -> String
    {
        return m_className;
    }

    void ScriptClass::setClassName( const String &className )
    {
        m_className = className;
    }

    auto ScriptClass::getNamespaceNames() const -> Array<String>
    {
        return m_namespaceNames;
    }

    void ScriptClass::setNamespaceNames( const Array<String> &namespaceNames )
    {
        m_namespaceNames = namespaceNames;
    }

    auto ScriptClass::getFunctions() const -> Array<SmartPtr<IScriptFunction>>
    {
        return m_functions;
    }

    void ScriptClass::setFunctions( const Array<SmartPtr<IScriptFunction>> &functions )
    {
        m_functions = functions;
    }

    auto ScriptClass::getParentClasses() const -> Array<String>
    {
        return m_parentClasses;
    }

    void ScriptClass::setParentClasses( const Array<String> &parentClasses )
    {
        m_parentClasses = parentClasses;
    }

    auto ScriptClass::getHeaderIncludes() const -> Array<String>
    {
        return m_headerIncludes;
    }

    void ScriptClass::setHeaderIncludes( const Array<String> &headers )
    {
        m_headerIncludes = headers;
    }

    auto ScriptClass::getSourceIncludes() const -> Array<String>
    {
        return m_sourceIncludes;
    }

    void ScriptClass::setSourceIncludes( const Array<String> &headers )
    {
        m_sourceIncludes = headers;
    }
}  // namespace fb
