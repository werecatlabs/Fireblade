#ifndef ScriptFunction_h__
#define ScriptFunction_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/Script/IScriptFunction.h>

namespace fb
{

    /** */
    class ScriptFunction : public CSharedObject<IScriptFunction>
    {
    public:
        ScriptFunction();
        ~ScriptFunction() override;

        String getClassName() const;
        void setClassName( const String &className );

        String getFunctionName() const;
        void setFunctionName( const String &functionName );

        String getReturnType() const;
        void setReturnType( const String &returnType );

        Array<SmartPtr<IScriptVariable>> getArguments() const;
        void setArguments( const Array<SmartPtr<IScriptVariable>> &arguments );

        bool isConstructor() const;
        void setConstructor( bool constructor );

        bool isDestructor() const;
        void setDestructor( bool destructor );

        String getFunctionBodyCPP() const;

    protected:
        bool m_isConstructor = false;
        bool m_isDestructor = false;

        String m_returnType;
        String m_className;
        String m_functionName;
        Array<SmartPtr<IScriptVariable>> m_arguments;
    };
}  // end namespace fb

#endif  // ScriptFunction_h__
