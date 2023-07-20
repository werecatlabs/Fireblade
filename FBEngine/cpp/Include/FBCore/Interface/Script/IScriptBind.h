#ifndef IScriptBind_h__
#define IScriptBind_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IScriptBind : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IScriptBind() override = default;

        /** */
        virtual void bind( void *data ) = 0;
    };
}  // end namespace fb

#endif  // IScriptBind_h__
