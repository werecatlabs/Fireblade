#ifndef AnimationUtility_h__
#define AnimationUtility_h__

#include <FBCore/Interface/Animation/IAnimator.h>


namespace fb
{

    class AnimationUtility
    {
    public:
        enum class TangentMode
        {
            Free,
            Auto,
            Linear,
            Constant,
            ClampedAuto
        };
    };

}  // end namespace fb

#endif  // AnimationUtility_h__
