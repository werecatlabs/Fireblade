#ifndef CObjectYield_h__
#define CObjectYield_h__

#include <FBCore/Interface/Memory/IObjectYield.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Memory/RawPtr.h>

namespace fb
{

    class ObjectYield : public SharedObject<IObjectYield>
    {
    public:
        ObjectYield();
        ObjectYield( RawPtr<IObject> pObject );
        ~ObjectYield() override;

        void operator()();

        s32 getLineNumber() const override;
        void setLineNumber( s32 lineNumber ) override;

        void stop() override;

        void yield() override;

    protected:
        atomic_s32 m_lineNumber;
        RawPtr<IObject> m_object;
    };
}  // end namespace fb

#endif  // CObjectYield_h__
