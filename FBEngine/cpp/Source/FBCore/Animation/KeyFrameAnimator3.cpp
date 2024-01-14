#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/KeyFrameAnimator3.h>

namespace fb
{
    auto KeyFrameAnimator3::getCurrentKeyFrame() const -> SmartPtr<KeyFrameTransform3>
    {
        return m_currentKeyFrame;
    }

}  // namespace fb
