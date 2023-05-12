#ifndef IScriptUserData_h__
#define IScriptUserData_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IScriptUserData : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IScriptUserData() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IScriptUserData_h__
