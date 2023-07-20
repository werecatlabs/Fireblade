#ifndef IAnimationKeyFrame_h__
#define IAnimationKeyFrame_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IAnimationKeyFrame : public ISharedObject
    {
    public:
        ~IAnimationKeyFrame() override = default;

        virtual f32 getTime() const = 0;
        virtual void setTime( f32 val ) = 0;
    };

}  // end namespace fb

#endif  // IAnimationKeyFrame_h__
