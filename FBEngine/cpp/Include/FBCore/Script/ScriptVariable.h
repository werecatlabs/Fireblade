#ifndef ScriptVariable_h__
#define ScriptVariable_h__

#include <FBCore/FBCorePrerequisites.h>

#include <FBCore/Interface/Script/IScriptVariable.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    class ScriptVariable : public IScriptVariable
    {
    public:
        ScriptVariable();
        ~ScriptVariable();

        String getType() const;

        void setType( const String &type );

        String getName() const;

        void setName( const String &name );

    protected:
        String m_type;
        String m_name;
    };
}  // namespace fb

#endif  // ScriptVariable_h__
