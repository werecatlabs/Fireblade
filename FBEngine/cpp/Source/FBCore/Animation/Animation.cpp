#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/Animation.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{

    Animation::~Animation() = default;

    Animation::Animation() = default;

    auto Animation::getLength() const -> f32
    {
        return 0.0f;
    }

    void Animation::setLength( f32 len )
    {
    }

}  // end namespace fb
