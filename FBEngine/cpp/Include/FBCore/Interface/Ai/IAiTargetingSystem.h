#ifndef IAiTargetingSystem_h__
#define IAiTargetingSystem_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IAiTargetingSystem : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAiTargetingSystem() override = default;
    };

}  // namespace fb

#endif  // IAiTargetingSystem_h__
