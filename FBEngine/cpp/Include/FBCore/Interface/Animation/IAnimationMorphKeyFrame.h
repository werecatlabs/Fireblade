#ifndef IAnimationMorphKeyFrame_h__
#define IAnimationMorphKeyFrame_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Animation/IAnimationKeyFrame.h>

namespace fb
{

    class IAnimationMorphKeyFrame : public IAnimationKeyFrame
    {
    public:
        ~IAnimationMorphKeyFrame() override = default;
    };

}  // end namespace fb

#endif  // IAnimationMorphKeyFrame_h__
