#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/Animation.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{

    Animation::~Animation()
    {
    }

    Animation::Animation()
    {
    }

    String Animation::getName() const
    {
        return StringUtil::EmptyString;
    }

    void Animation::setName( const String &val )
    {
    }

    f32 Animation::getLength() const
    {
        return 0.0f;
    }

    void Animation::setLength( f32 len )
    {
    }

}  // end namespace fb
