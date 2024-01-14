#ifndef ScriptClass_h__
#define ScriptClass_h__

#include <FBCore/FBCorePrerequisites.h>

#include <FBCore/Interface/Script/IScriptClass.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** ScriptClass implementation. */
    class ScriptClass : public IScriptClass
    {
    public:
        ScriptClass();
        ~ScriptClass() override;

        String getClassName() const;
        void setClassName( const String &className );

        Array<String> getNamespaceNames() const;
        void setNamespaceNames( const Array<String> &namespaceNames );

        Array<SmartPtr<IScriptFunction>> getFunctions() const;
        void setFunctions( const Array<SmartPtr<IScriptFunction>> &functions );

        Array<String> getParentClasses() const;
        void setParentClasses( const Array<String> &parentClasses );

        Array<String> getHeaderIncludes() const;
        void setHeaderIncludes( const Array<String> &headers );

        Array<String> getSourceIncludes() const;
        void setSourceIncludes( const Array<String> &headers );

    protected:
        String m_className;
        Array<String> m_namespaceNames;
        Array<String> m_parentClasses;

        Array<String> m_headerIncludes;
        Array<String> m_sourceIncludes;

        Array<SmartPtr<IScriptFunction>> m_functions;
    };
}  // end namespace fb

#endif  // ScriptClass_h__
