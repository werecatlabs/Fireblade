#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/Animation.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{

    Animation::~Animation() = default;

    Animation::Animation() = default;

    auto Animation::getName() const -> String
    {
        return StringUtil::EmptyString;
    }

    void Animation::setName( const String &name )
    {
    }

    auto Animation::getLength() const -> f32
    {
        return 0.0f;
    }

    void Animation::setLength( f32 len )
    {
    }

}  // end namespace fb
