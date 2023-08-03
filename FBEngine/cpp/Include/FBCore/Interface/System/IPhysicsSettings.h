#ifndef IPhysicsSettings_h__
#define IPhysicsSettings_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IPhysicsSettings : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IPhysicsSettings() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif // IPhysicsSettings_h__
