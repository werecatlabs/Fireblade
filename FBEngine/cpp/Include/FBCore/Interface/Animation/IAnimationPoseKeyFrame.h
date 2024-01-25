#ifndef IAnimationPoseKeyFrame_h__
#define IAnimationPoseKeyFrame_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Animation/IAnimationKeyFrame.h>

namespace fb
{

    class IAnimationPoseKeyFrame : public IAnimationKeyFrame
    {
    public:
        ~IAnimationPoseKeyFrame() override = default;
    };

}  // end namespace fb

#endif  // IAnimationPoseKeyFrame_h__
