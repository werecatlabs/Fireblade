#ifndef IAnimationState_h__
#define IAnimationState_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        
        class IAnimationState : public ISharedObject
        {
        public:
            ~IAnimationState() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IAnimationState_h__
