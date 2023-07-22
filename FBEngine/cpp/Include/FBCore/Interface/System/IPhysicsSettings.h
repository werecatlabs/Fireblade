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
    };

}  // namespace fb

#endif // IPhysicsSettings_h__
